#ifndef DIALOG_FACTORY_H
#define DIALOG_FACTORY_H

#include <QObject>
#include <QWidget>

#include <node_type.h>

enum class ProjectFileType {
    JSON,
    PNG
};

class DialogFactory: public QObject {
    Q_OBJECT
public:
    DialogFactory(QWidget* widget);
signals:
    void createNodeRequest(const QString&, NodeType);
    void JSONPathSelected(const QString&);
    void PNGPathSelected(const QString&);
public slots:
    void handleClassClick();
    void handleStructClick();
    void showError(const QString&);
    void showMessage(const QString&, const QString&);
    void selectJSON(const QString&);
    void selectPNG(const QString&);
private:
    QWidget* mWidget;

    void openNodeFactory(NodeType);
    QString pickPath(const QString&, const QString&, ProjectFileType);
};

#endif
