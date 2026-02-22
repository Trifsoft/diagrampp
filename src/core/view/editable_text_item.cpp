#include "view/editable_text_item.h"

EditableTextItem::EditableTextItem(const QString& text, QGraphicsItem* parent)
    : QGraphicsTextItem(text, parent)
{
    setDefaultTextColor(Qt::white);
}

void EditableTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsTextItem::mousePressEvent(event);
    event->accept();
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
