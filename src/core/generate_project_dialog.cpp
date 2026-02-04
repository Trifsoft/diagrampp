#include "generate_project_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <filesystem>
#include <QFileDialog>
#include <QDialogButtonBox>

generateProjectDialog::generateProjectDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Generate project");
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *pathLabel = new QLabel("Select Directory:");

    QHBoxLayout *pathLayout = new QHBoxLayout();
    path_edit = new QLineEdit();
    path_edit->setReadOnly(true);
    browse_button = new QPushButton("Select Path");
    browse_button->setFixedWidth(80);

    pathLayout->addWidget(path_edit);
    pathLayout->addWidget(browse_button);

    QLabel *notationLabel = new QLabel("Notation:");

    camel_case_radio = new QRadioButton("camelCase");
    snake_case_radio = new QRadioButton("snake_case");

    notation_group = new QButtonGroup(this);
    notation_group->addButton(camel_case_radio);
    notation_group->addButton(snake_case_radio);

    camel_case_radio->setChecked(true);

    QVBoxLayout *notationLayout = new QVBoxLayout();
    notationLayout->addWidget(camel_case_radio);
    notationLayout->addWidget(snake_case_radio);

    QLabel *toggleLabel = new QLabel("Toggle:");

    on_toggle = new QRadioButton("on");
    off_toggle = new QRadioButton("off");

    toggle_group = new QButtonGroup(this);
    toggle_group->addButton(on_toggle);
    toggle_group->addButton(off_toggle);

    on_toggle->setChecked(true);

    QVBoxLayout *toggleLayout = new QVBoxLayout();
    toggleLayout->addWidget(on_toggle);
    toggleLayout->addWidget(off_toggle);


    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    mainLayout->addWidget(pathLabel);
    mainLayout->addLayout(pathLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(notationLabel);
    mainLayout->addLayout(notationLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(toggleLabel);
    mainLayout->addLayout(toggleLayout);
    mainLayout->addWidget(buttonBox);

    connect(browse_button, &QPushButton::clicked, this, &generateProjectDialog::browse_directory);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void generateProjectDialog::browse_directory(){
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "Select folder",
                                                    path_edit->text().isEmpty() ? QDir::homePath() : path_edit->text());

    if (!dir.isEmpty() && std::filesystem::is_directory(dir.toStdString())){
        // we don't check any dir permission here (projectgenerator will)
        path_edit->setText(dir);
    }

}

QString generateProjectDialog::get_selected_path() const {
    return path_edit->text();
}

ProjectGenerator::FileNameNotation generateProjectDialog::get_selected_notation() const {
    return snake_case_radio->isChecked() ? ProjectGenerator::FileNameNotation::SNAKE_NOTATION
                                       : ProjectGenerator::FileNameNotation::CAMEL_NOTATION;
}

ProjectGenerator::ReplaceToggle generateProjectDialog::get_selected_toggle() const{
    return on_toggle->isChecked() ? ProjectGenerator::ReplaceToggle::ON
                                  : ProjectGenerator::ReplaceToggle::OFF;
}
