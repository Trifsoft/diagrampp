#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"
#include "validator.h"
#include <QWidget>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class Validator;
class DiagramGraph;

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();
    Validator* validator;

    void ValidateAndLink(std::shared_ptr<IUMLClassDiagramNode> activator);
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

private:
    DiagramGraph* diagram;
    Ui::Board *ui;

};
#endif // BOARD_H
