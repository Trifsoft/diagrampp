#ifndef CLASS_ELEMENT_TEXT_H
#define CLASS_ELEMENT_TEXT_H

#include <QGraphicsTextItem>
#include <model/base/class_element.h>

class ClassElementText: public QGraphicsTextItem {
    Q_OBJECT
public:
    ClassElementText(const QString& text, int text_width, std::optional<std::function<void()>> on_right_click = std::nullopt, QGraphicsItem* parent = nullptr);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
private:
    std::optional<std::function<void()>> m_on_right_click;
};

#endif // CLASS_ELEMENT_TEXT_H
