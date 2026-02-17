#include <dialog_factory.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

namespace {
    QString fileExtension(ProjectFileType fileType) {
        switch(fileType) {
            case ProjectFileType::JSON : return "json";
        }
    }
    QString fileDescription(ProjectFileType fileType) {
        switch(fileType) {
            case ProjectFileType::JSON : return "JSON Files (*.json)";
        }
    }
}

DialogFactory::DialogFactory(QWidget* widget)
    : mWidget(widget) {}

void DialogFactory::handleClassClick()      { openNodeFactory(NodeType::Class); }
void DialogFactory::handleStructClick()     { openNodeFactory(NodeType::Struct); }

void DialogFactory::showError(const std::string &message)
{
    QMessageBox::critical(mWidget, "Error", QString::fromStdString(message));
}

void DialogFactory::showMessage(const std::string &title, const std::string &message)
{
    QMessageBox::information(mWidget, QString::fromStdString(title), QString::fromStdString(message));
}

void DialogFactory::selectJSON(const QString& fileName)
{
    QString path = pickPath("Export Diagram as JSON",
                            fileName,
                            ProjectFileType::JSON);
    if(!path.isEmpty()) {
        emit JSONPathSelected(path);
    }
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
