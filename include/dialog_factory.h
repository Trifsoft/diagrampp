#ifndef DIALOG_FACTORY_H
#define DIALOG_FACTORY_H

#include <QObject>
#include <QWidget>

#include <node_type.h>
#include <generate_project_dialog.h>

enum class ProjectFileType {
    JSON,
    PNG
};

class DialogFactory: public QObject {
    Q_OBJECT
public:
    DialogFactory(QWidget* widget);
    ~DialogFactory();

    generateProjectDialog* generateDialog;
    QString selectJSON(const QString&);
    QString selectPNG(const QString&);
signals:
    void createNodeRequest(const QString&, NodeType);
    void projectGenerationRequested(const QString&,
                                  ProjectGenerator::FileNameNotation,
                                  ProjectGenerator::ReplaceToggle,
                                  QString&);
public slots:
    void showError(const QString&);
    void showWarning(const QString&);
    void showMessage(const QString&, const QString&);
    void openNodeFactory(NodeType);
private slots:
    void requestProjectGeneration();
private:
    QWidget* mWidget;

    QString pickPath(const QString&, const QString&, ProjectFileType);
};

#endif
