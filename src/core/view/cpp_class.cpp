#include "view/cpp_class.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_struct.h"
#include "view/field_dialog.h"
#include "view/method_dialog.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <widget/class_element_text.h>
#include <QGraphicsPixmapItem>
#include <QTextDocument>
#include <QMenu>
#include <QInputDialog>
#include <qgraphicsview.h>
#include <methodeditor.h>
#include <memory>
#include <utility>

// EditableTextItem implementation
EditableTextItem::EditableTextItem(const QString& text, ItemType type, std::weak_ptr<IClassElement> element_weak, QGraphicsItem* parent)
    : QGraphicsTextItem(text, parent)
    , m_type(type)
    , m_element_weak(element_weak)
{
    setDefaultTextColor(Qt::white);
    if (type != TitleType) {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
}

void EditableTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    
    if (m_type == TitleType) {
        return; // Can't edit title this way
    }
    
    // Find parent CppClass
    CppClass* parent = dynamic_cast<CppClass*>(this->parentItem());

    
    if (parent) {
        auto base = m_element_weak.lock();
        if(!base){
            qDebug() << "No element to edit!";
            return;
        }

        if (m_type == FieldType){
            if(auto field = std::dynamic_pointer_cast<Field>(base)) {
                parent->edit_field(field);
            }
        } else if (m_type == MethodType){
            if(auto method = std::dynamic_pointer_cast<Method>(base)) {
                parent->edit_method(method);
            }
        }
    } else {
        qDebug() << "Parent not found";
    }


}

void EditableTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    Q_UNUSED(event);

    // Find parent CppClass
    CppClass* parent = dynamic_cast<CppClass*>(this->parentItem());
    if(!parent){
        qDebug() << "Parent not found";
        return;
    }

    auto base = m_element_weak.lock();
    if(!base){
        qDebug() << "No element to edit!";
        return;
    }

    if(m_type == MethodType){
        if(auto method = std::dynamic_pointer_cast<Method>(base)){
            auto method_editor = new MethodEditor(method, parent->m_composition->get_name());
            method_editor->show();
        }
    }else{
        qDebug() << "Only methods are editable this way";
    }

}

// CppClass implementation
CppClass::CppClass(std::shared_ptr<Composition> composition, QGraphicsObject* parent)
    : QGraphicsObject(parent)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    connect(composition.get(), &Composition::changed, this, &CppClass::composition_changed);

    //setup button
    m_add_button = new QPushButton("+");
    m_button_proxy = new QGraphicsProxyWidget(this);
    m_button_proxy->setWidget(m_add_button);
    connect(m_add_button, &QPushButton::clicked, this, &CppClass::on_add_button_clicked);


    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    updateBoundingRect();
}

CppClass::~CppClass(){
    for(auto& connection : m_connections){
        if(connection.line){
            delete connection.line;
        }
        if(connection.arrow){
            delete connection.arrow;
        }
    }
    m_connections.clear();
    qDeleteAll(m_textItems);
    m_textItems.clear();
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
        y_offset += 1 + m_line_height * m_composition->methods.count();
    }

    painter->drawRect(QRect(0, y_offset, m_width, 1));
}

void CppClass::composition_changed() {
    updateBoundingRect();
    update();
}

void CppClass::updateBoundingRect()
{
    prepareGeometryChange();

    QFont font;
    QFontMetrics metrics(font);

    m_line_height = metrics.height() + 10;
    int fields_separator = !m_composition->fields.isEmpty();
    int methods_separator = !m_composition->methods.isEmpty();
    m_height = m_line_height * (1 + m_composition->fields.count() + m_composition->methods.count()) + fields_separator + methods_separator;
    m_width = metrics.horizontalAdvance(m_composition->get_name());
    for(auto& field : m_composition->fields) {
        auto tmp_width = metrics.horizontalAdvance(field->declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    for(auto& method : m_composition->methods) {
        auto tmp_width = metrics.horizontalAdvance(method->declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    m_width += 20;


    m_height += m_add_button->height() + 4;

    updateTextItems();
    update_button();
}

void CppClass::update_button()
{
    if (m_button_proxy && m_add_button) {
        const int margin = 0;//2;
        const int button_height = 24;

        m_add_button->setFixedSize(m_width-margin, button_height-margin);
        m_button_proxy->setPos(margin, m_height - button_height);
    }
}

//[TODO] verovatno postoji bolje resenje od iscrtavanja i brisanja svaki put
void CppClass::updateTextItems()
{
    // Clear old items
    qDeleteAll(m_textItems);
    m_textItems.clear();

    // Create title

    auto* titleItem = new EditableTextItem(m_composition->get_name(), EditableTextItem::TitleType, {}, this);
    titleItem->setPos(0, 0);
    titleItem->setTextWidth(m_width);
    titleItem->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    m_textItems.append(titleItem);


    int y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        y_offset += 1;
    }

    for(int i = 0; i < m_composition->fields.count(); ++i) {
        auto& field = m_composition->fields[i];
        add_text(field->declaration(), y_offset, EditableTextItem::FieldType, field);
        y_offset += m_line_height;
    }

    if(!m_composition->methods.isEmpty()) {
        y_offset += 1;
    }

    for(int i = 0; i < m_composition->methods.count(); ++i) {
        auto& method = m_composition->methods[i];
        add_text(method->declaration(), y_offset, EditableTextItem::MethodType, method);
        y_offset += m_line_height;
    }
    update_all_connections();
}

void CppClass::add_text(const QString text, int y_offset, EditableTextItem::ItemType type, std::weak_ptr<IClassElement> element_weak) {
    auto* item = new EditableTextItem(text, type, element_weak, this);
    item->setPos(0, y_offset);
    item->setTextWidth(m_width);
    item->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));

    m_textItems.append(item);
}

void CppClass::on_add_button_clicked()
{
    qDebug() << "Button clicked";

    QMenu context_menu;

    QAction* add_field_action = context_menu.addAction("Add Field");
    QAction* add_method_action = context_menu.addAction("Add Method");

    QPoint global_pos = QCursor::pos();
    QAction* selected_action = context_menu.exec(global_pos);

    if (selected_action == add_field_action) {
        add_new_field();
    } else if (selected_action == add_method_action) {
        add_new_method();
    }

}

void CppClass::add_new_field(){

    auto new_field = FieldDialog::create_field(scene()->views().first());
    if (new_field) {
        emit  add_field_request(m_composition.get(), new_field);
        updateBoundingRect();
        update();
    }
}

void CppClass::add_new_method(){
    auto new_method = MethodDialog::create_method(scene()->views().first());
    if (new_method) {
        emit  add_method_request(m_composition.get(), new_method);
        updateBoundingRect();
        update();
    }
}

void CppClass::edit_field(std::weak_ptr<Field> old_field_weak)
{
    if(auto old_field = old_field_weak.lock()){

        std::shared_ptr<Field> edited_field = FieldDialog::edit_field(*old_field, scene()->views().first());
        if (edited_field) {
            emit edit_field_request(m_composition.get(), old_field_weak, edited_field);
            updateBoundingRect();
            update();
        }
    }
}

void CppClass::edit_method(std::weak_ptr<Method> old_method_weak)
{
     if(auto old_method = old_method_weak.lock()){

        std::shared_ptr<Method> edited_method = MethodDialog::edit_method(*old_method, scene()->views().first());
        if (edited_method) {
            emit edit_method_request(m_composition.get(), old_method_weak, edited_method);
            updateBoundingRect();
            update();
        }
    }
}

void CppClass::mousePressEvent(QGraphicsSceneMouseEvent* event){
    emit objectClicked(m_composition.get());
    QGraphicsItem::mousePressEvent(event);
}

QPointF CppClass::get_top_center() const{
    QRectF rect = boundingRect();
    QPointF topCenterLocal(rect.width() / 2, 0);
    return mapToScene(topCenterLocal);
}

QPointF CppClass::get_bottom_center() const{
    QRectF rect = boundingRect();
    QPointF bottomCenterLocal(rect.width() / 2, rect.height());
    return mapToScene(bottomCenterLocal);
}

void CppClass::add_line_connection(CppClass* target, const BranchType branchType){
    QPointF startPoint = this->get_top_center();
    QPointF endPoint = target->get_bottom_center();

    int offset = target->number_of_connecitons*local_offset;
    QGraphicsPolygonItem* arrow = get_arrow(branchType);

    QPointF arrowPos = target->get_bottom_center();
    arrowPos.setX(arrowPos.x() + offset);
    arrow->setPos(arrowPos);
    scene()->addItem(arrow);
    endPoint = arrowPos;
    endPoint.setY(endPoint.y() + ((branchType == BranchType::ASSOCIATION) ? 0 : arrow_size)); // on bottom of arrow

    QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(startPoint, endPoint));
    QPen pen(Qt::black, 4);
    if(branchType == BranchType::DEPENDENCY || branchType == BranchType::REALIZATION){
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
    targetConn.isSource = false;
    targetConn.endLine = true;
    targetConn.type = branchType;
    targetConn.offset = offset;
    target->m_connections.append(targetConn);
    target->number_of_connecitons++;
}

QGraphicsPolygonItem* CppClass::get_arrow(const BranchType branchType) const {
    QGraphicsPolygonItem* arrow = new QGraphicsPolygonItem();
    QPolygonF polygon;

    switch(branchType) {
    case BranchType::INHERITANCE:
        polygon << QPointF(-arrow_size/2, arrow_size) << QPointF(arrow_size/2, arrow_size) << QPointF(0, 0);
        arrow->setBrush(Qt::white);
        break;
    case BranchType::COMPOSITION:
        polygon << QPointF(0, 0) << QPointF(-arrow_size/2, arrow_size/2) << QPointF(0, arrow_size) << QPointF(arrow_size/2, arrow_size/2);
        arrow->setBrush(Qt::black);
        break;
    case BranchType::AGGREGATION:
        polygon << QPointF(0, 0) << QPointF(-arrow_size/2, arrow_size/2) << QPointF(0, arrow_size) << QPointF(arrow_size/2, arrow_size/2);
        arrow->setBrush(Qt::white);
        break;
    case BranchType::DEPENDENCY:
        polygon << QPointF(0, 0) << QPointF(-arrow_size/3, arrow_size) << QPointF(arrow_size/3, arrow_size);
        arrow->setBrush(Qt::blue);
        break;
    case BranchType::REALIZATION:
        polygon << QPointF(-arrow_size/2, arrow_size) << QPointF(arrow_size/2, arrow_size) << QPointF(0, 0);
        arrow->setBrush(Qt::white);
        break;
    case BranchType::ASSOCIATION:
        polygon << QPointF(-arrow_size/2, arrow_size) << QPointF(arrow_size/2, arrow_size) << QPointF(0, 0);
        arrow->setBrush(Qt::white);
        arrow->setPolygon(polygon);
        arrow->setPen(QPen(Qt::white, 2));
        arrow->setVisible(false);
        return arrow;
    }

    arrow->setPolygon(polygon);
    arrow->setPen(QPen(Qt::black, 2));
    return arrow;
}

void CppClass::update_all_connections(){
    for(Connection& conn : m_connections){
        if(!conn.line || !conn.otherClass){
            continue;
        }
        if(conn.isSource){
            update_connection_line(conn);
        }else{
            update_connection_arrow(conn);
        }
    }
}


void CppClass::update_connection_line(Connection& conn){
    if(!conn.line || !conn.otherClass){
        return;
    }
    QPointF newStart = get_top_center();
    QPointF arrowPos = conn.arrow->pos();
    QPointF lineEnd = conn.otherClass->get_bottom_center();
    lineEnd.setX(arrowPos.x());
    lineEnd.setY(lineEnd.y() + ((conn.type == BranchType::ASSOCIATION) ? 0 : arrow_size));

    conn.line->setLine(QLineF(newStart, lineEnd));
}

void CppClass::remove_node(){
    disconnect_all_connections();
    if(scene()){
        scene()->removeItem(this);
    }
}

void CppClass::disconnect_all_connections(){
    for(Connection& conn : m_connections){
        if(conn.otherClass){
            conn.otherClass->remove_connection_to(this);
            if(conn.line && conn.arrow && scene()){
                scene()->removeItem(conn.line);
                scene()->removeItem(conn.arrow);
                delete conn.line;
                delete conn.arrow;
            }
        }
    }
    m_connections.clear();
}

void CppClass::remove_connection_to(CppClass* target){
    for(int iterator = m_connections.size()-1; iterator >= 0; iterator--){
        if(m_connections[iterator].otherClass == target){
            if(m_connections[iterator].isSource == false){
                this->number_of_connecitons--;
            }
            // safe operation because target alreardy freed all allocations
            m_connections.removeAt(iterator);

            update_offsets();
            update_all_connections();
            // because maximum number of connections is one (even different branch types) aren't allowed
            break;
        }
    }
}

void CppClass::set_object_visible(bool visible){
    this->setVisible(visible);
    for(Connection& conn : m_connections){
        if(conn.line){
            conn.line->setVisible(visible);
        }
        if(conn.arrow){
            conn.arrow->setVisible(visible);
        }
    }
}


void CppClass::update_connection_arrow(Connection& conn){
    if(!conn.arrow || !conn.line || !conn.otherClass){
        return;
    }
    QPointF arrowPos = get_bottom_center();
    conn.arrow->setPos(arrowPos + QPointF(conn.offset,0));

    QLineF currentLine = conn.line->line();
    QPointF newEnd = conn.arrow->pos();
    newEnd.setY(newEnd.y() + ((conn.type == BranchType::ASSOCIATION) ? 0 : arrow_size));

    conn.line->setLine(QLineF(currentLine.p1(), newEnd));
}

QVariant CppClass::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == ItemPositionHasChanged){
        update_all_connections();
    }
    return QGraphicsItem::itemChange(change, value);
}

void CppClass::remove_link_connection(CppClass* target, BranchType branch_type){
    if(!target){
        return;
    }

    Connection *source_connection = nullptr, *target_connection = nullptr;
    for(int i = m_connections.size()-1; i >= 0; i--){
        if(m_connections[i].otherClass == target && m_connections[i].type == branch_type){
            source_connection = &m_connections[i];
            m_connections.removeAt(i);
            break;
        }
    }
    for(int i = target->m_connections.size()-1; i >= 0; i--){
        if(target->m_connections[i].otherClass == this && target->m_connections[i].type == branch_type){
            target_connection = &target->m_connections[i];
            target->m_connections.removeAt(i);
            break;
        }
    }

    if(!source_connection || !target_connection){
#if DEBUG>=1
        qDebug() << "failed to find connections";
#endif
        return;
    }

    if(source_connection->isSource == false){
        std::swap(source_connection, target_connection);
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
    target->update_offsets();
    target->update_all_connections();

}

void CppClass::update_offsets(){
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


