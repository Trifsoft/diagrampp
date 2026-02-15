#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "recoveyLog.h"
#include <QWidget>
#include <QGraphicsScene>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include "view/cpp_class_view.h"
#include <connection_handler.h>

#include "commandManager/command_manager.h"

#include <view/connection.h>

#include <dialog_factory.h>

using ConnectionInfoType = QMap<CppClassView*, QList<std::tuple<CppClassView*, CppClassView*, BranchType>>>;


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
    Board(const QString& project_name = "Project name", QWidget *parent = nullptr);
    ~Board();

    DiagramGraph* get_diagram() const;
    CppClassView* get_view_from_node(SharedNodePtr);

    //void add_item(std::shared_ptr<Composition> item, const double coord_x = 0.0, const double coord_y = 0.0);
    void set_title(const QString&);

    //void execute_add_node(SharedNodePtr node);
    void execute_remove_node_with_branches(SharedNodePtr node);
    void execute_add_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
    void execute_remove_branch(SharedNodePtr from, SharedNodePtr to, BranchType branch_type);
signals:
    void radioButtonChecked(BranchType);
    void modeChanged(NodeModification);
public slots:
    void onCheckRadioButtonClicked();
    void onModeClicked();
    void exportPNG();
    void exportJSON();

    //void on_object_clicked(Composition* clickedClass);
    void on_generate_project();

    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);

    void on_link_added(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void on_link_removed(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void on_node_removed(SharedNodePtr node);

    void addItem(SharedNodePtr);
    void add_item(SharedNodePtr, double, double);
    void connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command);

    void removeNode(NodePtr);
    void addBranch(NodePtr, NodePtr, BranchType);
    void removeBranch(NodePtr, NodePtr, BranchType);
private:

    DiagramGraph *diagram;
    CommandManager *m_command_manager;
    DialogFactory *mDialogFactory;
    ConnectionHandler *mConnectionHandler;

    QList<CppClassView*> views;
    QMap<std::tuple<CppClassView*, CppClassView*, BranchType>, Connection*> connections;
    ConnectionInfoType incomingConnectionInfo;
    ConnectionInfoType outgoingConnectionInfo;
    Arrow* get_arrow(CppClassView*, BranchType);

    Ui::Board *ui;
    recoveryLog *recovery_log;

    void setup_actions();

    QString& get_file_name();

};
#endif // BOARD_H
