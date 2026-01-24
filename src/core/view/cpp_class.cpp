#include "view/cpp_class.h"
#include "model/elements/composition/composition.h"
#include "model/elements/composition/cpp_class.h"
#include "model/elements/composition/cpp_enum.h"
#include "model/elements/composition/cpp_struct.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QTextOption>
#include <QTextDocument>

CppClass::CppClass(Board* board,std::shared_ptr<Composition> composition, QGraphicsItem* parent)
    : NodeView(parent)
    , m_board(board)
    , m_composition(composition)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    //setup button
    m_add_button = new QPushButton("+");
    m_button_proxy = new QGraphicsProxyWidget(this);
    m_button_proxy->setWidget(m_add_button);
    connect(m_add_button, &QPushButton::clicked, this, &CppClass::on_add_button_clicked);


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
        y_offset += 1 + m_line_height * m_composition->methods.count();
    }

    painter->drawRect(QRect(0, y_offset, m_width, 1));
}

void CppClass::change_composition(std::function<void(std::shared_ptr<Composition>)> change)
{
    change(m_composition);
    updateBoundingRect();
    update();
}

void CppClass::updateBoundingRect()
{
    prepareGeometryChange();

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


    m_height += m_add_button->height() + 4;

    updateTextItems();
    update_button();
}

void CppClass::update_button()
{
    if (m_button_proxy && m_add_button) {
        const int margin = 2;
        const int button_height = 24;

        m_add_button->setFixedSize(m_width-margin, button_height-margin);
        m_button_proxy->setPos(margin, m_height - button_height);
    }
}

//[TODO] verovatno postoji bolje resenje od iscrtavanja i brisanja svaki put
void CppClass::updateTextItems()
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

void CppClass::add_text(const QString text, int y_offset, bool is_selectable) {
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

void CppClass::on_add_button_clicked()
{
    qDebug() << "Button clicked";
    emit  add_element_request(m_composition.get(), nullptr);
}

void CppClass::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (event->button() == Qt::LeftButton && m_board->linkageMode) {
        m_composition->Clicked = true;
        m_board->ValidateAndLink(this);
    } else if (event->button() == Qt::RightButton) {
        qDebug() << "Edit button (needs to be implemented)";
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
