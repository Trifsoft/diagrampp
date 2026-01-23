#include "nodefactory.h"
#include "ui_nodefactory.h"

QString get_label(NodeType node_type) {
    switch(node_type) {
    case NodeType::Class:
        return "class";
    case NodeType::Struct:
        return "struct";
    case NodeType::Enum:
        return "enum class";
    }
}

NodeFactory::NodeFactory(NodeType node_type, std::function<void(const QString)> generate_class ,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NodeFactory)
    , m_generate_class(generate_class)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("Create new " + get_label(node_type));
    connect(ui->generate, &QPushButton::clicked, this, &NodeFactory::onGenerateClick);
}

void NodeFactory::onGenerateClick() {
    m_generate_class(ui->className->toPlainText());
    close();
}

NodeFactory::~NodeFactory()
{
    delete ui;

    qDebug() << "Deleted NodeFactory";
}
