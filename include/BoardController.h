#ifndef BOARD_CONTROLLER_H
#define BOARD_CONTROLLER_H

#include <QObject>
#include <QMenuBar>
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

private slots:
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

    void select(NodePtr);
    void deselect(NodePtr);

private:
    DiagramGraph *diagram;
    CommandManager *m_command_manager;
    DialogFactory *mDialogFactory;
    ConnectionHandler *mConnectionHandler;
    Board *window;
    recoveryLog *recovery_log;

    QMenuBar *menuBar;
    void addMenuList(const QString &menuName, const QList<QList<QAction *> > &actions);
    void addMenu(const QString&, const QList<QAction*>&);


    void setupActions();
    QString getFileName();

    void setSelect(NodePtr, bool);


    template <typename Func>
    QAction* addAction(const QString& label, const QKeySequence& keySequence, const typename QtPrivate::ContextTypeForFunctor<Func>::ContextType *context, Func &&slot, const QIcon& icon = QIcon()) {
        auto newAction = new QAction(icon, label, this);
        newAction->setShortcut(keySequence);
        connect(newAction, &QAction::triggered, context, slot);
        window->addAction(newAction);
        return newAction;
    }

    template <typename Func>
    QAction* addCtrlAltAction(const QString& label, Qt::Key key, const typename QtPrivate::ContextTypeForFunctor<Func>::ContextType *context, Func &&slot, const QIcon& icon = QIcon()) {
        return addAction(label, QKeySequence(Qt::CTRL | Qt::ALT | key), context, slot, icon);
    }

    template <typename Func>
    QAction* addAction(const QString& label, Qt::Key key, const typename QtPrivate::ContextTypeForFunctor<Func>::ContextType *context, Func &&slot, const QIcon& icon = QIcon()) {
        return addAction(label, QKeySequence(key), context, slot, icon);
    }
};

#endif // BOARD_CONTROLLER_H
