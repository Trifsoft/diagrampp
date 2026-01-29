#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "validator.h"
#include <QWidget>
#include <QGraphicsScene>
#include <model/elements/field.h>
#include <model/elements/method.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class Validator;
class DiagramGraph;
class IClassElement;

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();
    Validator* validator;

    void ValidateAndLink(SharedNodePtr activator);
    const DiagramGraph* get_diagram() const;

    bool inheritance;
    bool association;
    bool navigation;
    bool linkageMode;

private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();
    void onCheckRadioButtonToggled();
    void onLinkageToggled();

    void on_add_field_requested(Composition* node, const Field& field);
    void on_add_method_requested(Composition* node, const Method& method);
    void on_edit_field_requested(Composition* node, int field_id, const Field& new_field);
    void on_edit_method_requested(Composition* node, int method_id, const Method& new_method);
private:
    DiagramGraph* diagram;
    Ui::Board *ui;

    void add_item(std::shared_ptr<Composition> item);

};
#endif // BOARD_H
