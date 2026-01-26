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
#include "validator.h"

class DiagramGraph;
class Validator;
class Board;

class CppClass : public NodeView {
Q_OBJECT

public:
    CppClass(Board *board, std::shared_ptr<Composition> composition, QGraphicsItem* parent = nullptr);
    ~CppClass() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);

    QPointF getTopCenter() const;
    QPointF getBottomCenter() const;
    void createConnection(CppClass* second);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void updateConnections();
    void addConnection(QGraphicsLineItem* line, bool isStart);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    virtual Composition* get_uml_class_diagram_node() override;

private:
    struct ConnectionInfo {
        QGraphicsLineItem* line;
        bool isStart;
    };

    BranchType branch;
    QVector<ConnectionInfo> m_connections;

    Board* m_board;
    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;

    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();
    void update_button();

    void add_text(QString text, int y_offset, bool is_selectable = false);
    void add_new_field();
    void add_new_method();

    QPushButton* m_add_button;
    QGraphicsProxyWidget* m_button_proxy;
    void on_add_button_clicked();
signals:
    void add_element_request(IUMLClassDiagramNode* node, IClassElement* element);
    void edit_element_request(IUMLClassDiagramNode* node, IClassElement* element);
};

#endif // _CPP_CLASS_H
