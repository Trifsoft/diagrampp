#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QGraphicsScene>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class Project;
}
QT_END_NAMESPACE

class CommandManager;
class DiagramGraph;

class Project : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Project(QWidget *parent = nullptr);
    ~Project();

private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();
    void onUndo();
    void onRedo();
private:

    Ui::Project *ui;
    std::unique_ptr<CommandManager> m_command_manager;
    std::unique_ptr<DiagramGraph> m_graph;

    void setup_actions();
    QAction* m_undo_action;
    QAction* m_redo_action;
};
#endif // PROJECT_H
