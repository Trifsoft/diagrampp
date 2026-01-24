#include "widget/class_element_text.h"

#include <QTextDocument>
#include <QTextOption>

ClassElementText::ClassElementText(const QString& text, int text_width, std::optional<std::function<void()>> on_right_click, QGraphicsItem* parent)
    : QGraphicsTextItem(text, parent)
    , m_on_right_click(on_right_click)
{
    if(on_right_click != std::nullopt) {
        setAcceptedMouseButtons(Qt::RightButton);
    }
    setDefaultTextColor(Qt::white);
    setTextWidth(text_width);
    document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
}

void ClassElementText::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if(m_on_right_click != std::nullopt) {
        m_on_right_click.value()(); // Nije greska, prve zagrade su od value() a druge od funkcije koju value() vraca
    }
    QGraphicsTextItem::mousePressEvent(event);
}
