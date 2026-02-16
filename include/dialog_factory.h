#ifndef DIALOG_FACTORY_H
#define DIALOG_FACTORY_H

#include <QObject>
#include <QWidget>

#include <node_type.h>

class DialogFactory: public QObject {
    Q_OBJECT
public:
    DialogFactory(QWidget* widget);
signals:
    void createNodeRequest(const QString&, NodeType);
public slots:
    void handleClassClick();
    void handleStructClick();
    void showError(const std::string&);
private:
    QWidget* mWidget;

    void openNodeFactory(NodeType);
};

#endif
