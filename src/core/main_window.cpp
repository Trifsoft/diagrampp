#include "main_window.h"
#include "ui_main_window.h"
#include "new_project.h"
#include "Board.h"
#include "serializers/diagram_json_serializer.h"
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileInfo>
#include <QInputDialog>

namespace {
    QString title_from_file(const QFile& file) {
        QFileInfo fileInfo(file);
        QString baseName = fileInfo.completeBaseName();

        QStringList words = baseName.split('_', Qt::SkipEmptyParts);
        for (QString& word : words) {
            if (!word.isEmpty()) {
                word[0] = word[0].toUpper();
            }
        }

        return words.join(' ');
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet(
        "QWidget { background-color: #E0FFFF; }"
        "QLabel { color: #008B8B; }"
        "QPushButton { background-color: #444; color: white; border-radius: 8px; }"
        "QPushButton:hover { background-color: #666; }"
        );
    connect(ui->pbNew, &QPushButton::clicked,
            this, &MainWindow::onNewProjectClicked);

    connect(ui->pbImport, &QPushButton::clicked,
            this, &MainWindow::onImportProjectClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewProjectClicked()
{
    QInputDialog dialog(this);
    dialog.setWindowTitle("Project Name");
    dialog.setLabelText("Enter project name:");
    dialog.setTextValue("");
    dialog.setStyleSheet(
        "QInputDialog QPushButton { background-color: #2c3e50; color: white; border-radius: 4px; padding: 6px 12px; }"
        "QInputDialog QPushButton:hover { background-color: #34495e; }"
        "QInputDialog QLabel { "
        "  color: #2b2b2b; "
        "} "
        "QInputDialog QLineEdit { "
        "  background-color: #2b2b2b; "
        "  color: #ffffff; "
        "  border: 1px solid #3a3a3a; "
        "  padding: 4px; "
        "} "
    );

    if (dialog.exec() == QDialog::Accepted && !dialog.textValue().isEmpty()) {
        auto project = new Board(dialog.textValue());
        project->show();
    }
}

void MainWindow::onImportProjectClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Import Project",
        QString(),
        "JSON Files (*.json)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file: " + filePath);
        return;
    }

    QByteArray jsonData = file.readAll();
    QString title = title_from_file(file);
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, "Error", "Failed to parse JSON: " + parseError.errorString());
        return;
    }

    if (!jsonDoc.isArray()) {
        QMessageBox::warning(this, "Error", "Invalid JSON format: expected array");
        return;
    }

    Board* board = new Board();

    DiagramJsonSerializer serializer(board);
    serializer.deserialize(jsonDoc.array());
    board->set_title(title);

    board->show();
}
