#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "validator.h"
#include <QWidget>
#include <QGraphicsScene>
#include <nodefactory.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class DiagramGraph;

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();

    void ValidateAndLink(SharedNodePtr activator);
    const DiagramGraph* get_diagram() const;
    void onUndo();

    BranchType branchType = BranchType::INHERITANCE;
    bool linkageMode = false;

public slots:
    void onClassClicked(SharedNodePtr clickedClass);

private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();
    void onCheckRadioButtonToggled();
    void onLinkageToggled();
    void onGenerateClicked(const QString& class_name, NodeType node_type);

private:
    DiagramGraph* diagram;
    Ui::Board *ui;

    std::pair<CppClass*, CppClass*> last;
    std::vector<std::pair<CppClass*, CppClass*>> operations;
    void add_item(std::shared_ptr<Composition> item);
    void openNodeFactory(NodeType node_type);
};
#endif // BOARD_H
