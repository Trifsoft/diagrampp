#include <dialog_factory.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

namespace {
    QString fileExtension(ProjectFileType fileType) {
        switch(fileType) {
            case ProjectFileType::JSON : return "json";
            case ProjectFileType::PNG : return "png";
        }
    }
    QString fileDescription(ProjectFileType fileType) {
        QString desc;
        switch(fileType) {
            case ProjectFileType::JSON : {
                desc = "JSON Files";
                break;
            }
            case ProjectFileType::PNG : {
                desc = "PNG Images";
                break;
            }
        }
        desc += " (*.";
        desc += fileExtension(fileType);
        desc += ")";
        return desc;
    }
}

DialogFactory::DialogFactory(QWidget* widget)
    : QObject(widget), mWidget(widget), generateDialog(new generateProjectDialog(widget))
{
    connect(generateDialog, &QDialog::accepted, this, &DialogFactory::requestProjectGeneration);
}

DialogFactory::~DialogFactory() {
    delete generateDialog;

    qDebug() << "Deleted dialog factory";
}

void DialogFactory::showError(const QString &message)
{
    QMessageBox::critical(mWidget, "Error", message);
}

void DialogFactory::showWarning(const QString &message)
{
    QMessageBox::warning(mWidget, "Warning", message);
}

void DialogFactory::showMessage(const QString &title, const QString &message)
{
    QMessageBox::information(mWidget, title, message);
}

QString DialogFactory::selectJSON(const QString& fileName)
{
    return pickPath("Export Diagram as JSON",
                    fileName,
                    ProjectFileType::JSON);
}

QString DialogFactory::selectPNG(const QString& fileName)
{
    return pickPath("Export Diagram as PNG",
                    fileName,
                    ProjectFileType::PNG);
}

void DialogFactory::requestProjectGeneration()
{
    QString path = generateDialog->get_selected_path();
    ProjectGenerator::FileNameNotation notation = generateDialog->get_selected_notation();
    ProjectGenerator::ReplaceToggle toggle = generateDialog->get_selected_toggle();
    QString project_dir_name = generateDialog->get_project_dir_name();
    emit projectGenerationRequested(path, notation, toggle, project_dir_name);
}

void DialogFactory::openNodeFactory(NodeType nodeType) {
    QString label;
    switch (nodeType) {
        case NodeType::Class:  label = "class"; break;
        case NodeType::Struct: label = "struct"; break;
    }

    QInputDialog dialog(mWidget);
    dialog.setWindowTitle("Create new " + label);
    dialog.setLabelText("Enter " + label + " name:");
    dialog.setTextValue("");
    dialog.setStyleSheet(
        "QInputDialog QPushButton { background-color: #2c3e50; color: white; border-radius: 4px; padding: 6px 12px; }"
        "QInputDialog QPushButton:hover { background-color: #34495e; }"
    );

    if (dialog.exec() == QDialog::Accepted && !dialog.textValue().isEmpty()) {
        emit createNodeRequest(dialog.textValue(), nodeType); //onGenerateClicked(dialog.textValue(), node_type);
    }
}

QString DialogFactory::pickPath(const QString& caption,
                                const QString& fileName,
                                ProjectFileType fileType)
{
    auto extension = "." + fileExtension(fileType);
    QString filePath = QFileDialog::getSaveFileName(
        mWidget,
        caption,
        QDir::homePath() + "/" + fileName + extension,
        fileDescription(fileType)
    );
    if (filePath.isEmpty()) {
        return "";
    }
    if (!filePath.endsWith(extension, Qt::CaseInsensitive)) {
        filePath += extension;
    }
    return filePath;
}
