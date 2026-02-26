#include "Board.h"
#include "view/cpp_class_view.h"
#include "model/elements/composition/cpp_struct.h"
#include "model/elements/composition/cpp_class.h"
#include <QGraphicsView>
#include "src/ui/ui_Board.h"
#include "model/elements/type/regular_type.h"
#include <QMessageBox>
#include <QPainter>
#include <set>

namespace {

void connectArrow(Arrow* newArrow, const QList<Connection*>& incomingConnections) {
    for(auto& connection : incomingConnections) {
        QObject::connect(connection, &Connection::hidden, newArrow, &Arrow::move_back);
        QObject::connect(connection, &Connection::shown, newArrow, &Arrow::move_forward);
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
    ui->board->setDragMode(QGraphicsView::RubberBandDrag);

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

Connection* Board::get_connection_from_nodes(CppClassView* from,
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

QList<Connection*> Board::getIncomingConnections(CppClassView* node) const
{
    QList<Connection*> result;
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

void Board::processRemovalRequest()
{
    if(!scene->selectedItems().isEmpty()) {
        QList<NodePtr> nodesToRemove;
        std::set<std::tuple<NodePtr, NodePtr, BranchType>> branchesToRemove;
        for(auto it : scene->selectedItems()) {
            auto itAsNodeView = dynamic_cast<NodeViewPtr>(it);
            auto itAsConnection = dynamic_cast<Connection*>(it);
            if(itAsNodeView) {
                nodesToRemove.append(itAsNodeView->get_uml_class_diagram_node().get());
                for(auto mapIt = nodeMap.begin(); mapIt != nodeMap.end(); mapIt++) {
                    for(auto& [iTo, iConn] : mapIt.value()) {
                        if(mapIt.key() == itAsNodeView || iTo == itAsNodeView) {
                            branchesToRemove.insert({
                                iConn->from(),
                                iConn->to(),
                                iConn->branchType()
                            });
                        }
                    }
                }
            }
            else if(itAsConnection) {
                branchesToRemove.insert({
                    itAsConnection->from(),
                    itAsConnection->to(),
                    itAsConnection->branchType()
                });
            }
            else {
                emit error("Unknown selected item type");
                return;
            }
        }
        QList<std::tuple<NodePtr, NodePtr, BranchType>> resultBranches;
        for(auto it = branchesToRemove.begin(); it != branchesToRemove.end(); it++) {
            resultBranches.append(*it);
        }
        scene->clearSelection();
        emit removalRequested(nodesToRemove, resultBranches);
    }
}

void Board::handleNodeRightClick(NodeViewPtr item)
{
    if(scene->selectedItems().size() == 1) {
        auto selectedAsNode = dynamic_cast<NodeViewPtr>(scene->selectedItems().first());
        if(selectedAsNode) {
            emit branchMenuRequested(selectedAsNode, item);
            return;
        }
    }
    emit changeNodeRequested(item);
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

        connect(item, &CppClassView::edit_field_request, this, &Board::on_edit_field_requested);
        connect(item, &CppClassView::edit_method_request, this, &Board::on_edit_method_requested);
        connect(item, &CppClassView::objectRightClicked, this, &Board::handleNodeRightClick);

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
        connection = new Connection(child, parent, incomingConnections.size(), branch);
        connectArrow(connection->arrow, incomingConnections);
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

void Board::onAddBranch(CppClassView*,
                        CppClassView*,
                        Connection* connection)
{
    scene->addItem(connection);
    emit connection->shown();
}

void Board::onRemoveBranch(CppClassView*,
                           CppClassView*,
                           Connection* connection)
{
    scene->removeItem(connection);
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
