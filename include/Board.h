#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "recoveyLog.h"
#include <QWidget>
#include <QGraphicsScene>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include "view/cpp_class_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class signalProcessor;
class DiagramGraph;
class IClassElement;

enum class NodeType {
    Class,
    Struct
};

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(const QString& project_name = "Project name", QWidget *parent = nullptr);
    ~Board();

    DiagramGraph* get_diagram() const;
    CppClassView* get_view_from_node(SharedNodePtr);

    //BranchType branch_type = BranchType::INHERITANCE;
    bool linkageMode = false;
    bool removeMode = false;

    void add_item(std::shared_ptr<Composition> item, const double coord_x = 0.0, const double coord_y = 0.0);
    void add_branch(SharedNodePtr node, SharedNodePtr neighbour, BranchType branch_type);
    void remove_branch(SharedNodePtr node, SharedNodePtr neighbour, BranchType branch_type);
    void set_title(const QString&);
signals:
    void link_added(CppClassView* child, CppClassView* parent, BranchType branch) const;
    void link_removed(CppClassView* child, CppClassView* parent, BranchType branch) const;
    void node_removed(CppClassView* node) const;
public slots:
    void onAddClassClicked();
    void onAddStructClicked();
    //void onCheckRadioButtonClicked();
    void onModeClicked();
    void exportPNG();
    void exportJSON();

    void on_object_clicked(Composition* clickedClass);
    void on_generate_project();

    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);

    void on_link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void on_link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void on_node_removed(SharedNodePtr node);
private:
    DiagramGraph *diagram;
    QList<CppClassView*> views;

    Ui::Board *ui;
    recoveryLog *recovery_log;

    SharedNodePtr first_activated = nullptr;
    SharedNodePtr second_activated = nullptr;

    void openNodeFactory(NodeType node_type);

    void onGenerateClicked(const QString& class_name, NodeType node_type);

    QString& get_file_name();
    BranchType get_branch_type();
};
#endif // BOARD_H
