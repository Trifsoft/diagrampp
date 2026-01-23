#include "view/cpp_class.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <widget/class_element_text.h>
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

    QGraphicsItem::mousePressEvent(event); // keep default behavior
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

// called when objects is moved in board
QVariant CppClass::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionHasChanged) {
        updateConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}

void CppClass::updateConnections(){
    for (const ConnectionInfo& info : m_connections) {
        QLineF currentLine = info.line->line();
        if (info.isStart) {
            QPointF newStart = getBottomCenter();
            info.line->setLine(QLineF(newStart, currentLine.p2()));
        } else {
            QPointF newEnd = getTopCenter();
            info.line->setLine(QLineF(currentLine.p1(), newEnd));
        }
    }
}

void CppClass::addConnection(QGraphicsLineItem* line, bool isStart){
    ConnectionInfo info{line, isStart};
    m_connections.append(info);
}

void CppClass::createConnection(CppClass* second){
    QPointF parentPoint = this->getBottomCenter();
    QPointF childPoint = second->getTopCenter();
    QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(parentPoint, childPoint));

    QGraphicsScene* sc = this->scene();
    line->setPen(QPen(Qt::blue, 2));
    sc->addItem(line);

    // saving parent and child relation
    this->addConnection(line, true);
    second->addConnection(line, false);
}

Composition* CppClass::get_uml_class_diagram_node() {
    return m_composition.get();
}
