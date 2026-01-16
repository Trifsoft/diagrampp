#ifndef _CPP_CLASS_H
#define _CPP_CLASS_H


#include <functional>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QVector>
#include <QGraphicsSceneMouseEvent>
#include <model/elements/composition/composition.h>
#include "graph/diagram_graph.h"
#include "umllinker.h"

class CppClass : public QGraphicsItem {
public:
    CppClass(std::shared_ptr<Composition> composition, DiagramGraph* Diagram, QGraphicsItem* parent = nullptr);
    ~CppClass() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);
    inline std::shared_ptr<Composition> getComposition() { return m_composition; }
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;

    QVector<QGraphicsTextItem*> m_textItems;
    DiagramGraph* diagram;
    UMLLinker* umllinker;

    void updateBoundingRect();
    void updateTextItems();

    void add_text(QString text, int y_offset, bool is_selectable = false);
};

#endif // _CPP_CLASS_H
