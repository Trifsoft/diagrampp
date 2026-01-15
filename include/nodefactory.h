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
    NodeFactory(NodeType node_type, std::function<void(const QString)> generate_class, QWidget *parent = nullptr);
    ~NodeFactory();
private slots:
    void onGenerateClick();
private:
    Ui::NodeFactory *ui;

    std::function<void(const QString)> m_generate_class;
};

#endif // NODEFACTORY_H
