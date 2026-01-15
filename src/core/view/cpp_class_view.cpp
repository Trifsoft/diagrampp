#include "view/cpp_class_view.h"
#include <QPainter>
#include <QFontMetrics>
#include <QTextOption>
#include <QTextDocument>

CppClassView::CppClassView(std::shared_ptr<Composition> composition, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    updateBoundingRect();
}

QRectF CppClassView::boundingRect() const
{
    return QRect(0,0,m_width,m_height);
}

void CppClassView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto bounding_rect = boundingRect();

    painter->fillRect(bounding_rect, Qt::black);
    painter->drawRect(bounding_rect);


    auto y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        painter->drawRect(QRect(0, y_offset, m_width, 1));
        y_offset += 1 + m_line_height * m_composition->fields.count();
    }
    if(!m_composition->methods.isEmpty()) {
        painter->drawRect(QRect(0, y_offset, m_width, 1));
    }
}

void CppClassView::change_composition(std::function<void(std::shared_ptr<Composition>)> change)
{
    change(m_composition);
    updateBoundingRect();
    update();
}

void CppClassView::updateBoundingRect()
{
    QFont font;
    QFontMetrics metrics(font);

    m_line_height = metrics.height() + 10;
    int fields_separator = !m_composition->fields.isEmpty();
    int methods_separator = !m_composition->methods.isEmpty();
    m_height = m_line_height * (1 + m_composition->fields.count() + m_composition->methods.count()) + fields_separator + methods_separator;
    m_width = metrics.horizontalAdvance(m_composition->get_name());
    for(auto& field : m_composition->fields) {
        auto tmp_width = metrics.horizontalAdvance(field.get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    for(auto& method : m_composition->methods) {
        auto tmp_width = metrics.horizontalAdvance(method.get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    m_width += 20;

    updateTextItems();
}

void CppClassView::updateTextItems()
{
    // Clear old items
    qDeleteAll(m_textItems);
    m_textItems.clear();

    // Create title
    auto* titleItem = new QGraphicsTextItem(m_composition->get_name(), this);
    titleItem->setPos(0, 0);
    titleItem->setDefaultTextColor(Qt::white);
    titleItem->setTextWidth(m_width);
    titleItem->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    m_textItems.append(titleItem);

    int y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        y_offset += 1;
    }

    for(auto& field : m_composition->fields) {
        add_text(field.get_declaration(), y_offset);
        y_offset += m_line_height;
    }

    if(!m_composition->methods.isEmpty()) {
        y_offset += 1;
    }

    for(auto& method : m_composition->methods) {
        add_text(method.get_declaration(), y_offset, true);
        y_offset += m_line_height;
    }
}

void CppClassView::add_text(const QString text, int y_offset, bool is_selectable) {
    auto* item = new QGraphicsTextItem(text, this);
    item->setPos(0, y_offset);
    item->setDefaultTextColor(Qt::white);
    if(is_selectable) {
        item->setFlag(QGraphicsItem::ItemIsSelectable);
    }
    item->setTextWidth(m_width);
    item->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    m_textItems.append(item);
}
