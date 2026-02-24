#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H

#include <QGraphicsTextItem>
#include <QVector>
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "graph/diagram_graph.h"
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <view/arrow.h>
#include <view/editable_text_item.h>

class DiagramGraph;

class CppClassView: public QGraphicsObject, public ISelectable {
    Q_OBJECT
public:
    CppClassView(SharedNodePtr composition, QGraphicsObject* parent = nullptr);
    ~CppClassView();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    QPointF get_top_center() const;
    QPointF get_bottom_center() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    SharedNodePtr get_uml_class_diagram_node();

    void select(bool) override;

public slots:
    void composition_changed();

private:

    SharedNodePtr mComposition;
    int mWidth;
    int mHeight;
    int mLineHeight;

    QVector<EditableTextItem*> mTextItems;

    void updateBoundingRect();
    void updateTextItems();

    EditableTextItem* addText(const QString& text, int y_offset, bool clickable = true);
    void editField(std::weak_ptr<Field> weak_field);
    void editMethod(std::weak_ptr<Method> weak_method);

    QGraphicsRectItem *mSelectedFrame;

    QPointF mAccumulatedChange = QPointF(0,0);

signals:
    void edit_field_request(Composition* node, std::weak_ptr<Field> old_field, std::shared_ptr<Field> new_field);
    void edit_method_request(Composition* node, std::weak_ptr<Method> old_method,std::shared_ptr<Method> new_method);

    void objectLeftClicked(CppClassView*);
    void objectRightClicked(CppClassView*);
    void objectMovedByUser(CppClassView* item, QPointF delta);

    void height_changed_by(double);
    void moved_by(const QPointF& delta);
};

#endif // _CPP_CLASS_H
