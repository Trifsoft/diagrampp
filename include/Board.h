#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "validator.h"
#include <QWidget>
#include <QGraphicsScene>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <nodefactory.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class DiagramGraph;
class IClassElement;

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

    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);
private:
    DiagramGraph* diagram;
    Ui::Board *ui;

    std::pair<CppClass*, CppClass*> last;
    std::vector<std::pair<CppClass*, CppClass*>> operations;
    void add_item(std::shared_ptr<Composition> item);
    void openNodeFactory(NodeType node_type);
};
#endif // BOARD_H
