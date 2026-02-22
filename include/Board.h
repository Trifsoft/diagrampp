#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include <QWidget>
#include <QGraphicsScene>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include "view/cpp_class_view.h"
#include <graph/graph.h>

#include <view/connection.h>
#include <node_modification.h>

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

    QGraphicsScene* scene;
    Ui::Board *ui;

    CppClassView* get_view_from_node(SharedNodePtr) const;
    std::shared_ptr<Connection> get_connection_from_nodes(CppClassView*,
                                                          CppClassView*,
                                                          BranchType) const;
    QList<std::shared_ptr<Connection>> getIncomingConnections(CppClassView*) const;

    void set_title(const QString&);

    void setSelected(SharedNodePtr, bool);
signals:
    void radioButtonChecked(BranchType);
    void modeChanged(NodeModification);
    void nodeCreationRequest(NodeType);

    void itemCreated(CppClassView*);

    void error(const QString&);
public slots:
    void on_add_field_requested(Composition* node, std::shared_ptr<Field> field);
    void on_add_method_requested(Composition* node, std::shared_ptr<Method>  method);
    void on_edit_field_requested(Composition* node, std::weak_ptr<Field> old_field_weak, std::shared_ptr<Field>  new_field);
    void on_edit_method_requested(Composition* node, std::weak_ptr<Method> old_method_weak, std::shared_ptr<Method>  new_method);

    void addLink(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void removeLink(SharedNodePtr from, SharedNodePtr to, BranchType branch);
    void removeItem(SharedNodePtr node);
    void addItem(SharedNodePtr);

    void add_item(SharedNodePtr, double, double);

    void onClassClicked();
    void onStructClicked();

    void selectNextBranchType();
    void selectPreviousBranchType();
private slots:
    void onCheckRadioButtonClicked();
    void onModeClicked();
protected:
    void onAddBranch(CppClassView*, CppClassView*, std::shared_ptr<Connection>) override;
    void onRemoveBranch(CppClassView*, CppClassView*, std::shared_ptr<Connection>) override;
    void onAddNode(CppClassView*) override;
    void onRemoveNode(CppClassView*) override;
private:

    Arrow* get_arrow(CppClassView*, BranchType);

    NodeModification oldMode = NodeModification::None;
};
#endif // BOARD_H
