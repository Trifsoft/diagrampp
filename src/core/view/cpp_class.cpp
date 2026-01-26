#include "view/cpp_class.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <widget/class_element_text.h>
#include <QGraphicsPixmapItem>
#include <QTextDocument>
#include <methodeditor.h>
#include <memory>
#include <utility>

CppClass::CppClass(Board* board,std::shared_ptr<Composition> composition, QGraphicsObject* parent)
    : NodeView(parent)
    , m_board(board)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    updateBoundingRect();
}

QRectF CppClass::boundingRect() const
{
    return QRect(0,0,m_width,m_height);
}

void CppClass::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto bounding_rect = boundingRect();

    painter->fillRect(bounding_rect, Qt::black);
    painter->drawRect(bounding_rect);


    auto y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        painter->drawRect(QRect(0, y_offset, m_width, 1));
        y_offset += 1 + m_line_height * m_composition->fields.count();
    }
    if(!m_composition->methods.isEmpty()) {
        painter->drawRect(QRect(0, y_offset, m_width, 1));
    }
}

void CppClass::change_composition(std::function<void(std::shared_ptr<Composition>)> change)
{
    change(m_composition);
    updateBoundingRect();
    update();
}

void CppClass::updateBoundingRect()
{
    QFont font;
    QFontMetrics metrics(font);

    m_line_height = metrics.height() + 10;
    int fields_separator = !m_composition->fields.isEmpty();
    int methods_separator = !m_composition->methods.isEmpty();
    m_height = m_line_height * (1 + m_composition->fields.count() + m_composition->methods.count()) + fields_separator + methods_separator;
    m_width = metrics.horizontalAdvance(m_composition->get_name());
    for(auto& field : m_composition->fields) {
        auto tmp_width = metrics.horizontalAdvance(field->get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    for(auto& method : m_composition->methods) {
        auto tmp_width = metrics.horizontalAdvance(method->get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    m_width += 20;

    updateTextItems();
}

void CppClass::updateTextItems()
{
    // Clear old items
    qDeleteAll(m_textItems);
    m_textItems.clear();

    // Create title
    add_text(m_composition->get_name(), 0, [this](){
        qDebug() << "Edit button (needs to be implemented)";
    });

    int y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        y_offset += 1;
    }

    for(auto& field : m_composition->fields) {
        add_text(field->get_declaration(), y_offset);
        y_offset += m_line_height;
    }

    if(!m_composition->methods.isEmpty()) {
        y_offset += 1;
    }

    for(auto& method : m_composition->methods) {
        add_text(method->get_declaration(), y_offset, [this, method](){
            auto method_editor = new MethodEditor(method.get(), m_composition->get_name());
            method_editor->show();
        });
        y_offset += m_line_height;
    }
}

void CppClass::add_text(const QString& text, int y_offset, std::optional<std::function<void()>> on_right_click) {
    auto* item = new ClassElementText(text, m_width, on_right_click, this);
    item->setPos(0, y_offset);
    m_textItems.append(item);
}

void CppClass::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (m_board->linkageMode) {
        m_composition->Clicked = true;
        m_board->ValidateAndLink(this);
    }else if(event->button() == Qt::RightButton){
        m_board->onUndo();
    }

    QGraphicsItem::mousePressEvent(event);
}

QPointF CppClass::getTopCenter() const{
    QRectF rect = boundingRect();
    QPointF topCenterLocal(rect.width() / 2, 0);
    return mapToScene(topCenterLocal);
}

QPointF CppClass::getBottomCenter() const{
    QRectF rect = boundingRect();
    QPointF bottomCenterLocal(rect.width() / 2, rect.height());
    return mapToScene(bottomCenterLocal);
}

void CppClass::addLineConnection(CppClass* target, BranchType branchType){
    QPointF startPoint = this->getTopCenter();
    QPointF endPoint = target->getBottomCenter();


    int offset = target->number_of_connecitons*local_offset;
    QGraphicsPolygonItem* arrow = getArrow(branchType);
    if(branchType != BranchType::ASSOCIATION){
        QPointF arrowPos = target->getBottomCenter();
        arrowPos.setX(arrowPos.x() + offset);
        arrow->setPos(arrowPos);
        scene()->addItem(arrow);
        endPoint = arrowPos;
        endPoint.setY(endPoint.y() + size); // on bottom of arrow
    }

    QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(startPoint, endPoint));
    QPen pen(Qt::black, 4);
    if(branchType == BranchType::DEPENDENCY){
        pen.setStyle(Qt::DashLine);
    }
    line->setPen(pen);
    scene()->addItem(line);

    Connection sourceConn;
    sourceConn.line = line;
    sourceConn.arrow = arrow;
    sourceConn.otherClass = target;
    sourceConn.isSource = true;
    sourceConn.endLine = false;
    sourceConn.type = branchType;
    sourceConn.offset = offset;
    m_connections.append(sourceConn);

    Connection targetConn;
    targetConn.line = line;
    targetConn.arrow = arrow;
    targetConn.otherClass = this;
    targetConn.isSource = arrow == nullptr;
    targetConn.endLine = true;
    targetConn.type = branchType;
    targetConn.offset = offset;
    target->m_connections.append(targetConn);
    target->number_of_connecitons++;
}

QGraphicsPolygonItem* CppClass::getArrow(BranchType branchType){
    QGraphicsPolygonItem* arrow = new QGraphicsPolygonItem();
    QPolygonF polygon;

    switch(branchType) {
    case BranchType::INHERITANCE:
        polygon << QPointF(-size/2, size) << QPointF(size/2, size) << QPointF(0, 0);
        arrow->setBrush(Qt::white);
        break;
    case BranchType::COMPOSITION:
        polygon << QPointF(0, 0) << QPointF(-size/2, size/2) << QPointF(0, size) << QPointF(size/2, size/2);
        arrow->setBrush(Qt::black);
        break;
    case BranchType::AGGREGATION:
        polygon << QPointF(0, 0) << QPointF(-size/2, size/2) << QPointF(0, size) << QPointF(size/2, size/2);
        arrow->setBrush(Qt::white);
        break;
    case BranchType::ASSOCIATION:
        return nullptr;
    case BranchType::DEPENDENCY:
        polygon << QPointF(0, 0) << QPointF(-size/3, size) << QPointF(size/3, size);
        arrow->setBrush(Qt::white);
        break;
    }

    arrow->setPolygon(polygon);
    arrow->setPen(QPen(Qt::black, 2));
    return arrow;
}

void CppClass::updateAllConnections(){
    for(Connection& conn : m_connections){
        if(!conn.line || !conn.otherClass){
            continue;
        }
        if(conn.isSource){
            updateConnectionLine(conn);
        }else{
            updateConnectionArrow(conn);
        }
    }
}


void CppClass::updateConnectionLine(Connection& conn){
    if(!conn.line || !conn.otherClass){
        return;
    }
    if(conn.type == BranchType::ASSOCIATION){
        QLineF currentLine = conn.line->line();
        if(conn.endLine){
            QPointF newStart = getBottomCenter();
            conn.line->setLine(QLineF(newStart, currentLine.p2()));
        }else{
            QPointF newEnd = getTopCenter();
            conn.line->setLine(QLineF(currentLine.p1(), newEnd));
        }
        return;
    }
    QPointF newStart = getTopCenter();
    QPointF lineEnd = conn.otherClass->getBottomCenter();
    QPointF arrowPos = conn.arrow->pos();
    lineEnd.setX(arrowPos.x());
    lineEnd.setY(lineEnd.y() + size);

    conn.line->setLine(QLineF(newStart, lineEnd));
}

void CppClass::updateConnectionArrow(Connection& conn){
    if(!conn.arrow || !conn.line || !conn.otherClass){
        return;
    }
    QPointF arrowPos = getBottomCenter();
    conn.arrow->setPos(arrowPos + QPointF(conn.offset,0));

    QLineF currentLine = conn.line->line();
    QPointF newEnd = conn.arrow->pos();
    newEnd.setY(newEnd.y() + size);

    conn.line->setLine(QLineF(currentLine.p1(), newEnd));
}

QVariant CppClass::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == ItemPositionHasChanged){
        updateAllConnections();

        for(Connection& conn : m_connections){
            if(conn.otherClass){
                conn.otherClass->updateAllConnections();
            }
        }

    }
    return QGraphicsItem::itemChange(change, value);
}

void CppClass::removeLink(CppClass* target){
    if(!target){
        return;
    }

    Connection *source_connection = nullptr, *target_connection = nullptr;
    for(int i = 0; i < m_connections.size(); i++){
        if(m_connections[i].otherClass == target){
            source_connection = &m_connections[i];
            m_connections.removeAt(i);
            break;
        }
    }
    for(int i = 0; i < target->m_connections.size(); i++){
        if(target->m_connections[i].otherClass == this){
            target_connection = &target->m_connections[i];
            target->m_connections.removeAt(i);
            break;
        }
    }

    if(source_connection->isSource == false){
        std::swap(source_connection, target_connection);
    }

    if(!source_connection || !target_connection){
        qDebug() << "failed to find connections";
        return;
    }

    // both target and source points to same line and arrow
    if(source_connection->line){
        scene()->removeItem(source_connection->line);
        delete source_connection->line;
    }
    if(source_connection->arrow){
        scene()->removeItem(source_connection->arrow);
        delete source_connection->arrow;
    }

    target->number_of_connecitons--;

    target->updateOffsets();

    target->updateAllConnections();
    updateAllConnections();

}

void CppClass::updateOffsets(){
    int offset = 0;
    for(Connection& conn : m_connections){
        if(conn.isSource == false){
            conn.offset = offset;
            offset += local_offset;
        }
    }
}

Composition* CppClass::get_uml_class_diagram_node() {
    return m_composition.get();
}
