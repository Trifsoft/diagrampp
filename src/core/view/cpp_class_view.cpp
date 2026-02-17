#include "view/cpp_class_view.h"
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
    
    // Find parent CppClassView
    CppClassView* parent = dynamic_cast<CppClassView*>(this->parentItem());

    
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

    // Find parent CppClassView
    CppClassView* parent = dynamic_cast<CppClassView*>(this->parentItem());
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

// CppClassView implementation
CppClassView::CppClassView(SharedNodePtr composition, QGraphicsObject* parent)
    : QGraphicsObject(parent)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    connect(composition.get(), &Composition::changed, this, &CppClassView::composition_changed);

    //setup button
    m_add_button = new QPushButton("+");
    m_button_proxy = new QGraphicsProxyWidget(this);
    m_button_proxy->setWidget(m_add_button);
    connect(m_add_button, &QPushButton::clicked, this, &CppClassView::on_add_button_clicked);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    updateBoundingRect();
}

CppClassView::~CppClassView(){
    m_textItems.clear();    
}

QRectF CppClassView::boundingRect() const
{
    return QRect(0,0,m_width,m_height);
}

void CppClassView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto bounding_rect = boundingRect();
    painter->fillRect(bounding_rect, QColor("#2b2b2b"));


    auto y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
        painter->drawRect(QRect(0, y_offset, m_width, 1));

        y_offset += 1 + m_line_height * m_composition->fields.count();
    }
    if(!m_composition->methods.isEmpty()) {
        painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
        painter->drawRect(QRect(0, y_offset, m_width, 1));

        y_offset += 1 + m_line_height * m_composition->methods.count();
    }

    painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
    painter->drawRect(QRect(0, y_offset, m_width, 1));

}

void CppClassView::composition_changed() {
    updateBoundingRect();
    update();
}

void CppClassView::updateBoundingRect()
{
    prepareGeometryChange();

    QFont font;
    QFontMetrics metrics(font);

    double old_height = m_height;

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
    m_width = fmax(m_width+20, 150);

    m_height += m_add_button->height() + 4;

    emit height_changed_by(m_height - old_height);

    updateTextItems();
    update_button();
}

void CppClassView::update_button()
{
    if (m_button_proxy && m_add_button) {
        const int margin = 0;//2;
        const int button_height = 24;

        m_add_button->setFixedSize(m_width-margin, button_height-margin);
        m_button_proxy->setPos(margin, m_height - button_height);
    }
}

void CppClassView::updateTextItems()
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
    //update_all_connections();
}

void CppClassView::add_text(const QString text, int y_offset, EditableTextItem::ItemType type, std::weak_ptr<IClassElement> element_weak) {
    auto* item = new EditableTextItem(text, type, element_weak, this);
    item->setPos(0, y_offset);
    item->setTextWidth(m_width);
    item->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));

    m_textItems.append(item);
}

void CppClassView::on_add_button_clicked()
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

void CppClassView::add_new_field(){

    auto new_field = FieldDialog::create_field(scene()->views().first());
    if (new_field) {
        emit  add_field_request(m_composition.get(), new_field);
        updateBoundingRect();
        update();
    }
}

void CppClassView::add_new_method(){
    auto new_method = MethodDialog::create_method(scene()->views().first());
    if (new_method) {
        emit  add_method_request(m_composition.get(), new_method);
        updateBoundingRect();
        update();
    }
}

void CppClassView::edit_field(std::weak_ptr<Field> old_field_weak)
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

void CppClassView::edit_method(std::weak_ptr<Method> old_method_weak)
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

void CppClassView::mousePressEvent(QGraphicsSceneMouseEvent* event){
    QGraphicsItem::mousePressEvent(event);
    emit objectClicked(m_composition.get());
}

QPointF CppClassView::get_top_center() const{
    QRectF rect = boundingRect();
    QPointF topCenterLocal(rect.width() / 2, 0);
    return mapToScene(topCenterLocal);
}

QPointF CppClassView::get_bottom_center() const{
    QRectF rect = boundingRect();
    QPointF bottomCenterLocal(rect.width() / 2, rect.height());
    return mapToScene(bottomCenterLocal);
}

QVariant CppClassView::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == ItemPositionChange){
        QPointF delta = value.toPointF() - pos();
        emit moved_by(delta);
    }
    return QGraphicsObject::itemChange(change, value);
}

SharedNodePtr CppClassView::get_uml_class_diagram_node() {
    return m_composition;
}


