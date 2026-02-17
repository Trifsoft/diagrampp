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
#include <graph/graph.h>

#include "commandManager/command_manager.h"

#include <view/connection.h>

#include <dialog_factory.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class Board : public QWidget, public Graph<CppClassView*, std::shared_ptr<Connection>>
{
    Q_OBJECT

public:
    Board(const QString& project_name = "Project name", QWidget *parent = nullptr);
    ~Board();

    DiagramGraph* get_diagram() const;
    CppClassView* get_view_from_node(SharedNodePtr) const;
    std::shared_ptr<Connection> get_connection_from_nodes(CppClassView*,
                                                          CppClassView*,
                                                          BranchType) const;
    QList<std::shared_ptr<Connection>> getIncomingConnections(CppClassView*) const;

    //void add_item(std::shared_ptr<Composition> item, const double coord_x = 0.0, const double coord_y = 0.0);
    void set_title(const QString&);
signals:
    void radioButtonChecked(BranchType);
    void modeChanged(NodeModification);
    void JSONPathRequested(const QString&);
    void serializationRequested(const QString&, Board*);
    void message(const std::string&, const std::string&);
    void error(const std::string&);
public slots:
    void onCheckRadioButtonClicked();
    void onModeClicked();
    void exportPNG();
    void requestJSONPath();
    void requestSerialization(const QString&);
    void showSerializationSuccessMessage(const QString&);

    //void on_object_clicked(Composition* clickedClass);
    void on_generate_project();

    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);

    void addLink(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void removeLink(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void removeItem(SharedNodePtr node);
    void addItem(SharedNodePtr);

    void add_item(SharedNodePtr, double, double);

    void connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command);
    void connectRemoveNodeCommand(std::shared_ptr<RemoveNodeCommand> command);
    void connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command);
    void connectRemoveBranchCommand(std::shared_ptr<RemoveBranchCommand> command);
protected:
    void onAddBranch(CppClassView*, CppClassView*, std::shared_ptr<Connection>) override;
    void onRemoveBranch(CppClassView*, CppClassView*, std::shared_ptr<Connection>) override;
    void onAddNode(CppClassView*) override;
    void onRemoveNode(CppClassView*) override;
private:

    DiagramGraph *diagram;
    CommandManager *m_command_manager;
    DialogFactory *mDialogFactory;
    ConnectionHandler *mConnectionHandler;

    QGraphicsScene* scene;

    Arrow* get_arrow(CppClassView*, BranchType);

    Ui::Board *ui;
    recoveryLog *recovery_log;

    void setup_actions();

    QString& get_file_name();

};
#endif // BOARD_H
