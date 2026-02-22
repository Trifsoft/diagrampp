#include "view/cpp_class_view.h"
#include "view/editable_text_item.h"
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

// CppClassView implementation
CppClassView::CppClassView(SharedNodePtr composition, QGraphicsObject* parent)
    : QGraphicsObject(parent)
    , mComposition(composition)
    , mSelectedFrame(new QGraphicsRectItem(0,0,0,0,this))
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    connect(composition.get(), &Composition::changed, this, &CppClassView::composition_changed);

    QPen selectedFramePen(Qt::red);
    selectedFramePen.setWidth(1);
    mSelectedFrame->setPen(selectedFramePen);
    mSelectedFrame->setVisible(false);

    mAddButton = new EditableTextItem("+", this);
    mAddButton->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    connect(mAddButton, &EditableTextItem::clicked, this, &CppClassView::onAddButtonClicked);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    updateBoundingRect();
}

CppClassView::~CppClassView(){
    mTextItems.clear();
}

QRectF CppClassView::boundingRect() const
{
    return QRect(0,0,mWidth,mHeight);
}

void CppClassView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto bounding_rect = boundingRect();
    painter->fillRect(bounding_rect, QColor("#2b2b2b"));


    auto y_offset = mLineHeight;
    if(!mComposition->fields.isEmpty()) {
        painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
        painter->drawRect(QRect(0, y_offset, mWidth, 1));

        y_offset += 1 + mLineHeight * mComposition->fields.count();
    }
    if(!mComposition->methods.isEmpty()) {
        painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
        painter->drawRect(QRect(0, y_offset, mWidth, 1));

        y_offset += 1 + mLineHeight * mComposition->methods.count();
    }

    painter->setPen(QPen(QColor("#404040"), 1));  // tamna siva
    painter->drawRect(QRect(0, y_offset, mWidth, 1));

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

    double old_height = mHeight;

    mLineHeight = metrics.height() + 10;
    int fields_separator = !mComposition->fields.isEmpty();
    int methods_separator = !mComposition->methods.isEmpty();
    mHeight = mLineHeight * (2 + mComposition->fields.count() + mComposition->methods.count()) + fields_separator + methods_separator;
    mWidth = metrics.horizontalAdvance(mComposition->get_name());
    for(auto& field : mComposition->fields) {
        auto tmp_width = metrics.horizontalAdvance(field->declaration());
        if(tmp_width > mWidth) {
            mWidth = tmp_width;
        }
    }
    for(auto& method : mComposition->methods) {
        auto tmp_width = metrics.horizontalAdvance(method->declaration());
        if(tmp_width > mWidth) {
            mWidth = tmp_width;
        }
    }
    mWidth = fmax(mWidth+20, 150);

    mSelectedFrame->setRect(0,0,mWidth,mHeight);

    emit height_changed_by(mHeight - old_height);

    updateTextItems();
    update_button();
}

void CppClassView::update_button()
{
    mAddButton->setTextWidth(mWidth);
    mAddButton->setPos(0, mHeight - mLineHeight);
}

void CppClassView::updateTextItems()
{
    qDeleteAll(mTextItems);
    mTextItems.clear();

    addText(mComposition->get_name(), 0);

    int y_offset = mLineHeight;
    if(!mComposition->fields.isEmpty()) {
        y_offset += 1;
    }

    for(auto& field : mComposition->fields) {
        auto* item = addText(field->declaration(), y_offset);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        connect(item, &EditableTextItem::doubleClicked, this, [this, field]() {
            editField(field);
        });
        y_offset += mLineHeight;
    }

    if(!mComposition->methods.isEmpty()) {
        y_offset += 1;
    }

    for(auto& method : mComposition->methods) {
        auto* item = addText(method->declaration(), y_offset);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        connect(item, &EditableTextItem::doubleClicked, this, [this, method]() {
            editMethod(method);
        });
        y_offset += mLineHeight;
    }
}

EditableTextItem* CppClassView::addText(const QString& text, int y_offset) {
    auto* item = new EditableTextItem(text, this);
    item->setPos(0, y_offset);
    item->setTextWidth(mWidth);
    item->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    mTextItems.append(item);
    return item;
}

void CppClassView::onAddButtonClicked()
{
    QMenu context_menu;

    QAction* add_field_action = context_menu.addAction("Add Field");
    QAction* add_method_action = context_menu.addAction("Add Method");

    QPoint global_pos = QCursor::pos();
    QAction* selected_action = context_menu.exec(global_pos);

    if (selected_action == add_field_action) {
        addNewField();
    } else if (selected_action == add_method_action) {
        addNewMethod();
    }

}

void CppClassView::addNewField(){

    auto new_field = FieldDialog::create_field(scene()->views().first());
    if (new_field) {
        emit  add_field_request(mComposition.get(), new_field);
        updateBoundingRect();
        update();
    }
}

void CppClassView::addNewMethod(){
    auto new_method = MethodDialog::create_method(scene()->views().first());
    if (new_method) {
        emit  add_method_request(mComposition.get(), new_method);
        updateBoundingRect();
        update();
    }
}

void CppClassView::editField(std::weak_ptr<Field> old_field_weak)
{
    if(auto old_field = old_field_weak.lock()){

        std::shared_ptr<Field> editedField = FieldDialog::edit_field(*old_field, scene()->views().first());
        if (editedField) {
            emit edit_field_request(mComposition.get(), old_field_weak, editedField);
            updateBoundingRect();
            update();
        }
    }
}

void CppClassView::editMethod(std::weak_ptr<Method> old_method_weak)
{
     if(auto old_method = old_method_weak.lock()){

        std::shared_ptr<Method> editedMethod = MethodDialog::edit_method(*old_method, scene()->views().first());
        if (editedMethod) {
            emit edit_method_request(mComposition.get(), old_method_weak, editedMethod);
            updateBoundingRect();
            update();
        }
    }
}

void CppClassView::mousePressEvent(QGraphicsSceneMouseEvent* event){
    QGraphicsItem::mousePressEvent(event);
    emit objectClicked(mComposition.get());
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
    return mComposition;
}

void CppClassView::setSelected(bool isSelected)
{
    mSelectedFrame->setVisible(isSelected);
}
