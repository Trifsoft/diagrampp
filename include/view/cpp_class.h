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
    CppClass(Board *board, std::shared_ptr<Composition> composition, QGraphicsItem* parent = nullptr);
    ~CppClass();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);

    QPointF getTopCenter() const;
    QPointF getBottomCenter() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void updateConnections();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void addPNGConnection(const QString& imagePath, CppClass* target, bool imageOnTarget = true);
    void removeAllPNGConnections();

    virtual Composition* get_uml_class_diagram_node() override;

private:
    struct PNGConnection {
        QGraphicsPixmapItem* imageItem;
        CppClass* targetClass;
        bool imageOnTarget;
        QString imagePath;

        void updatePosition(CppClass* source);
    };
    void updatePNGConnections();
    QVector<PNGConnection> m_pngConnections;

    BranchType branch;

    Board* m_board;
    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;

    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();

    void add_text(const QString& text, int y_offset, std::optional<std::function<void()>> on_right_click = std::nullopt);
};

#endif // _CPP_CLASS_H
