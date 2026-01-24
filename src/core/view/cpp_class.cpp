#include "view/cpp_class.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <widget/class_element_text.h>
#include <QGraphicsPixmapItem>
#include <QTextDocument>
#include <methodeditor.h>
#include <memory>

CppClass::CppClass(Board* board,std::shared_ptr<Composition> composition, QGraphicsItem* parent)
    : NodeView(parent)
    , m_board(board)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptedMouseButtons(Qt::LeftButton);
    updateBoundingRect();
}


CppClass::~CppClass(){
    for (const PNGConnection& conn : m_pngConnections) {
        if (conn.imageItem && conn.imageItem->scene()) {
            conn.imageItem->scene()->removeItem(conn.imageItem);
            delete conn.imageItem;
        }
    }
    m_pngConnections.clear();
}

QRectF CppClass::boundingRect() const
{
    return QRect(0,0,m_width,m_height);
}

void CppClass::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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

void CppClass::change_composition(std::function<void(std::shared_ptr<Composition>)> change)
{
    change(m_composition);
    updateBoundingRect();
    update();
}

void CppClass::updateBoundingRect()
{
    QFont font;
    QFontMetrics metrics(font);

    m_line_height = metrics.height() + 10;
    int fields_separator = !m_composition->fields.isEmpty();
    int methods_separator = !m_composition->methods.isEmpty();
    m_height = m_line_height * (1 + m_composition->fields.count() + m_composition->methods.count()) + fields_separator + methods_separator;
    m_width = metrics.horizontalAdvance(m_composition->get_name());
    for(auto& field : m_composition->fields) {
        auto tmp_width = metrics.horizontalAdvance(field->get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    for(auto& method : m_composition->methods) {
        auto tmp_width = metrics.horizontalAdvance(method->get_declaration());
        if(tmp_width > m_width) {
            m_width = tmp_width;
        }
    }
    m_width += 20;

    updateTextItems();
}

void CppClass::updateTextItems()
{
    // Clear old items
    qDeleteAll(m_textItems);
    m_textItems.clear();

    // Create title
    add_text(m_composition->get_name(), 0, [this](){
        qDebug() << "Edit button (needs to be implemented)";
    });

    int y_offset = m_line_height;
    if(!m_composition->fields.isEmpty()) {
        y_offset += 1;
    }

    for(auto& field : m_composition->fields) {
        add_text(field->get_declaration(), y_offset);
        y_offset += m_line_height;
    }

    if(!m_composition->methods.isEmpty()) {
        y_offset += 1;
    }

    for(auto& method : m_composition->methods) {
        add_text(method->get_declaration(), y_offset, [this, method](){
            auto method_editor = new MethodEditor(method.get(), m_composition->get_name());
            method_editor->show();
        });
        y_offset += m_line_height;
    }
}

void CppClass::add_text(const QString& text, int y_offset, std::optional<std::function<void()>> on_right_click) {
    auto* item = new ClassElementText(text, m_width, on_right_click, this);
    item->setPos(0, y_offset);
    m_textItems.append(item);
}

void CppClass::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (m_board->linkageMode) {
        m_composition->Clicked = true;
        m_board->ValidateAndLink(this);
    }

    QGraphicsItem::mousePressEvent(event);
}

QPointF CppClass::getTopCenter() const{
    QRectF rect = boundingRect();
    QPointF topCenterLocal(rect.width() / 2, 0);
    return mapToScene(topCenterLocal);
}

QPointF CppClass::getBottomCenter() const{
    QRectF rect = boundingRect();
    QPointF bottomCenterLocal(rect.width() / 2, rect.height());
    return mapToScene(bottomCenterLocal);
}

void CppClass::PNGConnection::updatePosition(CppClass* source){
    if(!imageItem || !source || !targetClass){
        return;
    }
    QPointF imagePoint = targetClass->getTopCenter();
    QPointF otherPoint = source->getBottomCenter();

    imageItem->setPos(imagePoint);

    // place center of the picture on class edge
    QPixmap pixmap = imageItem->pixmap();
    imageItem->setOffset(-pixmap.width() / 2, -pixmap.height() / 2);

    // rotates to point toward target class
    QPointF direction = otherPoint - imagePoint;
    qreal angle = qRadiansToDegrees(qAtan2(direction.y(), direction.x()));
    imageItem->setRotation(angle);

    // scaling
    qreal distance = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    qreal scale = qBound(0.5, distance / 200.0, 2.0);
    imageItem->setScale(scale);
}

void CppClass::addPNGConnection(const QString& imagePath, CppClass* target, bool imageOnTarget){
    if(!target || imagePath.isEmpty()){
        qDebug() << "addPngConnection";
        return;
    }

    QPixmap pixmap(imagePath);
    if(pixmap.isNull()){
        qDebug() << "Failed to load PNG:" << imagePath;
        return;
    }

    QGraphicsPixmapItem* imageItem = new QGraphicsPixmapItem(pixmap);
    imageItem->setTransformationMode(Qt::SmoothTransformation);
    scene()->addItem(imageItem);

    PNGConnection connection;
    connection.imageItem = imageItem;
    connection.targetClass = target;
    connection.imageOnTarget = imageOnTarget;
    connection.imagePath = imagePath;
    connection.updatePosition(this);
    m_pngConnections.append(connection);

    qDebug() << "Added PNG connection from" << m_composition->get_name() << "with image:" << imagePath;

}

void CppClass::updatePNGConnections(){
    for(PNGConnection& conn : m_pngConnections){
        conn.updatePosition(this);
    }
}


// called when objects is moved in board
QVariant CppClass::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == ItemPositionHasChanged){
        updatePNGConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}

Composition* CppClass::get_uml_class_diagram_node() {
    return m_composition.get();
}
