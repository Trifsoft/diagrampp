#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H

#include <functional>
#include <view/node_view.h>
#include <QGraphicsTextItem>
#include <QVector>
#include "Board.h"
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "model/base/branches.h"
#include "graph/diagram_graph.h"

class DiagramGraph;
class Board;

class CppClass : public NodeView {

public:
    CppClass(Board *board, std::shared_ptr<Composition> composition, QGraphicsObject* parent = nullptr);
    ~CppClass() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);

    QPointF getTopCenter() const;
    QPointF getBottomCenter() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void updateConnections();
    void addLineConnection(CppClass* target, BranchType branch);
    void addConnectionInfo(QGraphicsLineItem* line, bool isStart);
    void removeLink(CppClass* target);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual Composition* get_uml_class_diagram_node() override;

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

    qreal size = 15.0;
    void updateConnectionLine(Connection& conn);
    void updateConnectionArrow(Connection& conn);
    void updateAllConnections();
    void updateOffsets();
    QGraphicsPolygonItem* getArrow(BranchType branchType);

    Board* m_board;
    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;
    int number_of_connecitons = 0;
    int local_offset = 15;

    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();

    void add_text(const QString& text, int y_offset, std::optional<std::function<void()>> on_right_click = std::nullopt);
};

#endif // _CPP_CLASS_H
