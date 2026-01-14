#ifndef BOARD_H
#define BOARD_H

#include "graph/diagram_graph.h"

#include <QWidget>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class Board;
}
QT_END_NAMESPACE

class Board : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();

private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();

private:
    DiagramGraph* diagram;
    Ui::Board *ui;
};
#endif // BOARD_H
