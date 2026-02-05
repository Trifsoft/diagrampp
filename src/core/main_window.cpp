#include "main_window.h"
#include "ui_main_window.h"
#include "new_project.h"
#include "Board.h"
#include <QInputDialog>

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

    connect(ui->pbHistory, &QPushButton::clicked,
            this, &MainWindow::onHistoryClicked);

    connect(ui->pbHelp, &QPushButton::clicked,
            this, &MainWindow::onHelpClicked);

    connect(ui->pbExample, &QPushButton::clicked,
            this, &MainWindow::onExampleClicked);
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
    );

    if (dialog.exec() == QDialog::Accepted && !dialog.textValue().isEmpty()) {
        auto project = new Board(dialog.textValue());
        project->show();
    }
}

void MainWindow::onImportProjectClicked()
{
    QWidget *w = new QWidget();
    w->setWindowTitle("Import Project");
    w->resize(400, 200);
    w->show();
}

void MainWindow::onHistoryClicked()
{
    QWidget *w = new QWidget();
    w->setWindowTitle("History");
    w->resize(400, 200);
    w->show();
}

void MainWindow::onHelpClicked()
{
    QWidget *w = new QWidget();
    w->setWindowTitle("Help");
    w->resize(400, 200);
    w->show();
}

void MainWindow::onExampleClicked()
{
    auto project = new Board();
    project->show();
}
