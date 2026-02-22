#ifndef EDITABLE_TEXT_ITEM_H
#define EDITABLE_TEXT_ITEM_H

#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

class EditableTextItem : public QGraphicsTextItem {
    Q_OBJECT
public:
    explicit EditableTextItem(const QString& text, bool clickable = true, QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void clicked();
    void doubleClicked();
private:
    bool mClickable;
};

#endif // EDITABLE_TEXT_ITEM_H
