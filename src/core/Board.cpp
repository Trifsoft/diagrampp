#include "Board.h"
#include "view/cpp_class_view.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include <QMessageBox>
#include <QPainter>

namespace {

void connectArrow(Arrow* newArrow, const QList<std::shared_ptr<Connection>>& incomingConnections) {
    for(auto& connection : incomingConnections) {
        QObject::connect(connection.get(), &Connection::hidden, newArrow, &Arrow::move_back);
        QObject::connect(connection.get(), &Connection::shown, newArrow, &Arrow::move_forward);
    }
}

}

Board::Board(const QString& project_name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Board)
{
    // ----------- Board ------------
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(project_name);

    scene = new QGraphicsScene();
    ui->board->setScene(scene);
    ui->board->setStyleSheet("background-color: #1a1a1a;");
    ui->side_menu->setStyleSheet("background-color: #252526;");

    // Emit upgraded signal - node creation
    connect(ui->add_class, &QPushButton::clicked, this, &Board::onClassClicked);
    connect(ui->add_struct, &QPushButton::clicked, this, &Board::onStructClicked);
}

CppClassView* Board::get_view_from_node(SharedNodePtr node) const {
    for(auto& addedNode : nodes) {
        if(addedNode->get_uml_class_diagram_node() == node) {
            return addedNode;
        }
    }
    for(auto& removedNode : removedNodes) {
        if(removedNode->get_uml_class_diagram_node() == node) {
            return removedNode;
        }
    }
    return nullptr;
}

std::shared_ptr<Connection> Board::get_connection_from_nodes(CppClassView* from,
                                                             CppClassView* to,
                                                             BranchType branchType) const
{
    const auto& connectionList = nodeMap[from];
    for(auto it = connectionList.begin(); it != connectionList.end(); it++) {
        if(it->first == to && it->second->branchType() == branchType) {
            return it->second;
        }
    }
    for(auto& [iFrom, iTo, iConn] : removedBranches) {
        if(iFrom == from && iTo == to && iConn->branchType() == branchType) {
            return iConn;
        }
    }
    return nullptr;
}

QList<std::shared_ptr<Connection>> Board::getIncomingConnections(CppClassView* node) const
{
    QList<std::shared_ptr<Connection>> result;
    for(auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
        if(it.key() == node) {
            continue;
        }
        for(auto& [iTo, iConn] : it.value()) {
            if(iTo == node) {
                result.append(iConn);
            }
        }
    }
    return result;
}

Board::~Board()
{
    qDebug() << "Deleted Board";
    delete scene;
    delete ui;
}

void Board::onClassClicked()
{
    emit nodeCreationRequest(NodeType::Class);
}

void Board::onStructClicked()
{
    emit nodeCreationRequest(NodeType::Struct);
}

void Board::on_add_field_requested(Composition *node, std::shared_ptr<Field> field)
{
    if(node)
    {
        node->add_field(field);
    }

    qDebug() << "Recieved signal add field from: " << node->get_label();
}

void Board::on_add_method_requested(Composition *node, std::shared_ptr<Method> method)
{
    if(node)
    {
        node->add_method(method);
    }

    qDebug() << "Recieved signal add method from: " << node->get_label();
}

void Board::on_edit_field_requested(Composition *node, std::weak_ptr<Field> old_field_weak,  std::shared_ptr<Field> new_field)
{
    if(node){
        // old_field is now shared_ptr
        if(auto old_field = old_field_weak.lock()){
            *old_field = *new_field;
        }
    }
    qDebug() << "Received signal edit field from: " << node->get_label();
}

void Board::on_edit_method_requested(Composition *node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>new_method)
{
    if(node){
        // old_field is now shared_ptr
        if(auto old_method = old_method_weak.lock()){
            *old_method = *new_method;
        }
    }

    qDebug() << "Received signal edit method from: " << node->get_label();
}

void Board::add_item(SharedNodePtr node, double x, double y) {
    auto item = get_view_from_node(node);
    if(!item) {
        item = new CppClassView(node);

        connect(item, &CppClassView::add_field_request, this, &Board::on_add_field_requested);
        connect(item, &CppClassView::add_method_request, this, &Board::on_add_method_requested);
        connect(item, &CppClassView::edit_field_request, this, &Board::on_edit_field_requested);
        connect(item, &CppClassView::edit_method_request, this, &Board::on_edit_method_requested);

        item->setX(x);
        item->setY(y);

        emit itemCreated(item);
    }

    mAddNode(item);
}

void Board::addItem(SharedNodePtr node) {
    add_item(node, 0, 0);
}

void Board::set_title(const QString& title) {
    setWindowTitle(title);
}

void Board::exportPNG(const QString& filePath)
{
    QRectF items_rect = scene->itemsBoundingRect();
    if(items_rect.isEmpty()){
        emit error("No items to export!");
        return;
    }

    // margins [item] -> [  item  ]
    const qreal margin = 50;
    items_rect.adjust(-margin, -margin, margin, margin);

    QImage image(items_rect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    scene->render(&painter, QRectF(0, 0, image.width(), image.height()), items_rect);
    painter.end();

    if(image.save(filePath, "PNG", 100)){
        emit message("Success", QString("Diagram exported to:\n%1\n\nSize: %2x%3 pixels")
                                        .arg(filePath).arg(image.width()).arg(image.height()));
    } else {
        emit error("Failed to save image. Check write permissions.");
    }
}

void Board::setSelected(SharedNodePtr node, bool isSelected)
{
    auto item = get_view_from_node(node);
    if(item == nullptr) {
        emit error("Fatal error - passed nullptr to select");
    }
    item->setSelected(isSelected);
}

void Board::addLink(SharedNodePtr from, SharedNodePtr to, BranchType branch) {
    auto child = get_view_from_node(from);
    auto parent = get_view_from_node(to);
    auto connection = get_connection_from_nodes(child, parent, branch);
    if(child && parent && !connection){
        auto incomingConnections = getIncomingConnections(parent);
        connection = std::make_shared<Connection>(child, parent, incomingConnections.size(), branch);
        connectArrow(connection->arrow, incomingConnections);

        emit connectionCreated(connection.get());
    }
    mAddBranch(child, parent, connection);
}

void Board::removeLink(SharedNodePtr from, SharedNodePtr to, BranchType branch) {
    auto child = get_view_from_node(from);
    auto parent = get_view_from_node(to);
    auto connection = get_connection_from_nodes(child, parent, branch);
    mRemoveBranch(child, parent, connection);
}

void Board::removeItem(SharedNodePtr target) {
    auto node = get_view_from_node(target);
    mRemoveNode(node);
}

// Arrow* Board::get_arrow(CppClassView* view, BranchType branch) {
//     const auto& incomingConnections = getIncomingConnections(view);
//     Arrow* new_arrow = new Arrow(branch, view->pos(), view->boundingRect().height(), incomingConnections.size());
//     for(auto& connection : incomingConnections) {
//         connect(connection.get(), &QObject::destroyed, new_arrow, &Arrow::move_back);
//     }
//     connect(view, &CppClassView::moved_by, new_arrow, &Arrow::move_by);
//     connect(view, &CppClassView::height_changed_by, new_arrow, &Arrow::move_y);
//     return new_arrow;
// }

void Board::onAddBranch(CppClassView*,
                        CppClassView*,
                        std::shared_ptr<Connection> connection)
{
    scene->addItem(connection->arrow);
    scene->addItem(connection->line);
    emit connection->shown();
}

void Board::onRemoveBranch(CppClassView*,
                           CppClassView*,
                           std::shared_ptr<Connection> connection)
{
    scene->removeItem(connection->arrow);
    scene->removeItem(connection->line);
    emit connection->hidden();
}

void Board::onAddNode(CppClassView* node)
{
    scene->addItem(node);
}

void Board::onRemoveNode(CppClassView* node)
{
    scene->removeItem(node);
}
