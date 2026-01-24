#ifndef CPPCLASSITEM_H
#define CPPCLASSITEM_H

#include <QGraphicsItem>
#include <QString>

class CppClassItem : public QGraphicsItem
{
public:

    CppClassItem(const QString& name, QGraphicsItem* parent = nullptr);
    ~CppClassItem() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QString getName() const;
    void setName(const QString& name);

private:
    QString m_name;
    QRectF m_boundingRect;

    void updateBoundingRect();
};

#endif // CPPCLASSITEM_H
