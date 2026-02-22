#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H

#include <QGraphicsTextItem>
#include <QVector>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "graph/diagram_graph.h"
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <view/arrow.h>

class DiagramGraph;

// Custom text item to track field/method info
class EditableTextItem : public QGraphicsTextItem {
public:
    enum ItemType { TitleType, FieldType, MethodType };
    
    EditableTextItem(const QString& text, ItemType type, std::weak_ptr<IClassElement> element_weak, QGraphicsItem* parent);
    
    ItemType get_type() const { return m_type; }
    // int get_element_id() const { return m_element_id; }
    std::weak_ptr<IClassElement> get_element() const {return m_element_weak;}
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
private:
    ItemType m_type;
    // int m_element_id;  // Unique ID
    std::weak_ptr<IClassElement> m_element_weak;
};

class CppClassView: public QGraphicsObject {
    Q_OBJECT
public:
    CppClassView(SharedNodePtr composition, QGraphicsObject* parent = nullptr);
    ~CppClassView();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    QPointF get_top_center() const;
    QPointF get_bottom_center() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    SharedNodePtr get_uml_class_diagram_node();

    void setSelected(bool);

public slots:
    void composition_changed();

private:

    SharedNodePtr m_composition;
    int m_width;
    int m_height;
    int m_line_height;

    // Editable text items for now
    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();
    void update_button();

    void add_text(const QString text, int y_offset, EditableTextItem::ItemType type, std::weak_ptr<IClassElement> element_weak);
    void add_new_field();
    void add_new_method();
    void edit_field(std::weak_ptr<Field> weak_method);
    void edit_method(std::weak_ptr<Method> weak_method);

    QPushButton* m_add_button;
    QGraphicsProxyWidget* m_button_proxy;
    QGraphicsRectItem *mSelectedFrame;
    void on_add_button_clicked();

    friend class EditableTextItem;

signals:
    void add_field_request(Composition* node, std::shared_ptr<Field> field);
    void add_method_request(Composition* node, std::shared_ptr<Method> method);
    void edit_field_request(Composition* node, std::weak_ptr<Field> old_field, std::shared_ptr<Field> new_field);
    void edit_method_request(Composition* node, std::weak_ptr<Method> old_method,std::shared_ptr<Method> new_method);
    void objectClicked(Composition* node);

    void height_changed_by(double);
    void moved_by(const QPointF& delta);
};

#endif // _CPP_CLASS_H
