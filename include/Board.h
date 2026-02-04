#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "view/signal_processor.h"
#include "recoveyLog.h"
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

class signalProcessor;
class DiagramGraph;
class IClassElement;

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();

    DiagramGraph* get_diagram() const;
    CppClass* get_view_from_node(SharedNodePtr);

    BranchType branch_type = BranchType::INHERITANCE;
    bool linkageMode = false;
    bool removeMode = false;

    void add_item(std::shared_ptr<Composition> item, const double coord_x = 0.0, const double coord_y = 0.0);
    void add_node_relationship(SharedNodePtr node, SharedNodePtr neighbour, BranchType branch_type);
public slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();
    void onCheckRadioButtonClicked();
    void onModeClicked();
    void exportPNG();

    void onGenerateClicked(const QString& class_name, NodeType node_type);
    void on_object_clicked(Composition* clickedClass);
    void on_generate_project();

    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);
private:
    DiagramGraph *diagram;
    QList<CppClass*> views;

    Ui::Board *ui;
    signalProcessor *signal_processor;
    recoveryLog *recovery_log;

    SharedNodePtr first_activated = nullptr;
    SharedNodePtr second_activated = nullptr;

    void openNodeFactory(NodeType node_type);
};
#endif // BOARD_H
