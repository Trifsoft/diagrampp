#ifndef CPPCLASSITEM_H
#define CPPCLASSITEM_H

#include <QGraphicsItem>
#include <QString>

class CppClassItem : public QGraphicsItem
{
public:
    enum ItemType {
        ClassType,
        InterfaceType,
        EnumType
    };

    CppClassItem(const QString& name, ItemType type, QGraphicsItem* parent = nullptr);
    ~CppClassItem() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QString getName() const;
    void setName(const QString& name);
    ItemType getType() const;

private:
    QString m_name;
    ItemType m_type;
    QRectF m_boundingRect;

    void updateBoundingRect();
};

#endif // CPPCLASSITEM_H
