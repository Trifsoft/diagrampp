#include "cppclassitem.h"
#include <QPainter>
#include <QFontMetrics>

CppClassItem::CppClassItem(const QString& name, ItemType type, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_name(name)
    , m_type(type)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    updateBoundingRect();
}

CppClassItem::~CppClassItem()
{
}

QRectF CppClassItem::boundingRect() const
{
    return m_boundingRect;
}

void CppClassItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillRect(m_boundingRect, Qt::black);
    painter->drawRect(m_boundingRect);

    painter->drawText(m_boundingRect, Qt::AlignCenter, m_name);
}

QString CppClassItem::getName() const
{
    return m_name;
}

void CppClassItem::setName(const QString& name)
{
    m_name = name;
    updateBoundingRect();
    update();
}

CppClassItem::ItemType CppClassItem::getType() const
{
    return m_type;
}

void CppClassItem::updateBoundingRect()
{
    QFont font;
    QFontMetrics metrics(font);
    int width = metrics.horizontalAdvance(m_name) + 20;
    int height = metrics.height() + 10;
    m_boundingRect = QRectF(0, 0, width, height);
}
