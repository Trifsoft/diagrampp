#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QWidget>

enum class NodeType {
    Class,
    Struct,
    Enum
};

namespace Ui {
class NodeFactory;
}

class NodeFactory : public QWidget
{
    Q_OBJECT

public:
    NodeFactory(NodeType node_type, QWidget *parent = nullptr);
    ~NodeFactory();
signals:
    void generateClicked(const QString&, NodeType node_type);
private slots:
    void onGenerateClick();
private:
    Ui::NodeFactory *ui;
    NodeType m_node_type;
};

#endif // NODEFACTORY_H
