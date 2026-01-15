#ifndef CPP_CLASS_VIEW_H
#define CPP_CLASS_VIEW_H

#include <functional>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QVector>
#include <model/elements/composition/composition.h>

class CppClassView: public QGraphicsItem {
public:
    CppClassView(std::shared_ptr<Composition> composition, QGraphicsItem* parent = nullptr);
    ~CppClassView() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void change_composition(std::function<void(std::shared_ptr<Composition>)> change);

private:
    std::shared_ptr<Composition> m_composition;
    int m_width;
    int m_height;
    int m_line_height;

    QVector<QGraphicsTextItem*> m_textItems;

    void updateBoundingRect();
    void updateTextItems();

    void add_text(QString text, int y_offset, bool is_selectable = false);
};

#endif // CPP_CLASS_VIEW_H
