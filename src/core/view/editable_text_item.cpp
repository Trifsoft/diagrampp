#include "view/editable_text_item.h"

EditableTextItem::EditableTextItem(const QString& text, bool clickable, QGraphicsItem* parent)
    : QGraphicsTextItem(text, parent), mClickable(clickable)
{
    setDefaultTextColor(Qt::white);
}

void EditableTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsTextItem::mousePressEvent(event);
    if(mClickable) {
        event->accept();
    }
}

void EditableTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (boundingRect().contains(event->pos())) {
        emit clicked();
    }
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void EditableTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    emit doubleClicked();
}
