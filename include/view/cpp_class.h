#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H


#include <functional>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QVector>
#include "Board.h"
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "model/base/branches.h"
#include "graph/diagram_graph.h"
#include "validator.h"

class DiagramGraph;
class Validator;
class Board;

class CppClass : public QGraphicsItem {

public:
    CppClass(Board *board, std::shared_ptr<Composition> composition, QGraphicsItem* parent = nullptr);
    ~CppClass() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);
    inline std::shared_ptr<IUMLClassDiagramNode> getClassDiagramNode() { return m_ClassDiagramNode; }

    QPointF getTopCenter() const;
    QPointF getBottomCenter() const;
    void createConnection(CppClass* second);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void updateConnections();
    void addConnection(QGraphicsLineItem* line, bool isStart);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    struct ConnectionInfo {
        QGraphicsLineItem* line;
        bool isStart;
    };

    BranchType branch;
    QVector<ConnectionInfo> m_connections;

    Board* m_board;
    std::shared_ptr<Composition> m_composition;
    std::shared_ptr<IUMLClassDiagramNode> m_ClassDiagramNode;
    int m_width;
    int m_height;
    int m_line_height;

    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();

    void add_text(QString text, int y_offset, bool is_selectable = false);
};

#endif // _CPP_CLASS_H
