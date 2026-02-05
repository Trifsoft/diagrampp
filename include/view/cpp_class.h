#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H

#include <functional>
#include <view/node_view.h>
#include <QGraphicsTextItem>
#include <QVector>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "Board.h"
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "model/base/branches.h"
#include "graph/diagram_graph.h"
#include <model/elements/field.h>
#include <model/elements/method.h>

class DiagramGraph;
class Board;

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
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
private:
    ItemType m_type;
    // int m_element_id;  // Unique ID
    std::weak_ptr<IClassElement> m_element_weak;
};

class CppClass : public NodeView {
    Q_OBJECT
public:
    CppClass(Board *board, std::shared_ptr<Composition> composition, QGraphicsObject* parent = nullptr);
    ~CppClass();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void addConnectionInfo(QGraphicsLineItem* line, bool isStart);

    QPointF get_top_center() const;
    QPointF get_bottom_center() const;
    void update_connections();
    void add_line_connection(CppClass* target, const BranchType branch);
    void remove_link_connection(CppClass* target, const BranchType branch);
    void set_object_visible(bool visible);
    void remove_node();
    void add_node();


    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual Composition* get_uml_class_diagram_node() override;

public slots:
    void composition_changed();

private:
    struct Connection {
        QGraphicsLineItem* line = nullptr;
        QGraphicsPolygonItem* arrow = nullptr;
        CppClass* otherClass = nullptr;
        bool isSource;
        bool endLine;
        BranchType type;
        int offset;
    };
    QList<Connection> m_connections;

    qreal arrow_size = 15.0;
    void update_connection_line(Connection& conn);
    void update_connection_arrow(Connection& conn);
    void update_all_connections();
    void update_offsets();
    void disconnect_all_connections();
    void remove_connection_to(CppClass* target);
    QGraphicsPolygonItem* get_arrow(const BranchType branchType) const;

    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;
    int number_of_connecitons = 0;
    int local_offset = 15;

    // Editable text items for now
    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();
    void update_button();

    // void add_text(QString text, int y_offset, EditableTextItem::ItemType type = EditableTextItem::TitleType, int element_id = -1);
    void add_text(const QString text, int y_offset, EditableTextItem::ItemType type, std::weak_ptr<IClassElement> element_weak);
    void add_new_field();
    void add_new_method();
    void edit_field(std::weak_ptr<Field> weak_method);
    void edit_method(std::weak_ptr<Method> weak_method);

    QPushButton* m_add_button;
    QGraphicsProxyWidget* m_button_proxy;
    void on_add_button_clicked();

    friend class EditableTextItem;

signals:
    void add_field_request(Composition* node, std::shared_ptr<Field> field);
    void add_method_request(Composition* node, std::shared_ptr<Method> method);
    void edit_field_request(Composition* node, std::weak_ptr<Field> old_field, std::shared_ptr<Field> new_field);
    void edit_method_request(Composition* node, std::weak_ptr<Method> old_method,std::shared_ptr<Method> new_method);
    void objectClicked(Composition* node);

};

#endif // _CPP_CLASS_H
