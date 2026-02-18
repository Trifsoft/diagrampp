#ifndef BOARD_CONTROLLER_H
#define BOARD_CONTROLLER_H

#include <QObject>
#include "graph/diagram_graph.h"
#include "recoveyLog.h"
#include "commandManager/command_manager.h"
#include "connection_handler.h"
#include "dialog_factory.h"
#include <Board.h>
#include <view/cpp_class_view.h>

class BoardController : public QObject
{
    Q_OBJECT

public:
    BoardController(Board* board, DiagramGraph* graph);
    BoardController(const QString&);
    ~BoardController();

signals:
    void message(const QString&, const QString&);
    void error(const QString&);
    void warning(const QString&);

public slots:
    void exportPNG(const QString&);
    void showSerializationSuccessMessage(const QString&);

    void generateProject(const QString&,
                         ProjectGenerator::FileNameNotation,
                         ProjectGenerator::ReplaceToggle,
                         QString&);

    void requestJSONPath();
    void requestPNGPath();

    void connectCreateNodeCommand(std::shared_ptr<AddNodeCommand> command);
    void connectRemoveNodeCommand(std::shared_ptr<RemoveNodeCommand> command);
    void connectCreateBranchCommand(std::shared_ptr<AddBranchCommand> command);
    void connectRemoveBranchCommand(std::shared_ptr<RemoveBranchCommand> command);
    void connectNewNodeView(CppClassView*);

private:
    DiagramGraph *diagram;
    CommandManager *m_command_manager;
    DialogFactory *mDialogFactory;
    ConnectionHandler *mConnectionHandler;
    Board *window;
    recoveryLog *recovery_log;

    void setupActions();
    QString getFileName();
};

#endif // BOARD_CONTROLLER_H
