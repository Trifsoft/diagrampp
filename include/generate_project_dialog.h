#ifndef GENERATE_PROJECT_DIALOG_H
#define GENERATE_PROJECT_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include "project_generator.h"

class generateProjectDialog : public QDialog {
    Q_OBJECT
public:

    generateProjectDialog(QWidget *parent = nullptr);

    QString get_selected_path() const;
    ProjectGenerator::FileNameNotation get_selected_notation() const;
    ProjectGenerator::ReplaceToggle get_selected_toggle() const;
    QString get_project_dir_name() const;

private slots:
    void browse_directory();

private:
    QLineEdit *project_name_edit = nullptr;
    QLineEdit *path_edit;
    QPushButton *browse_button;
    QRadioButton *camel_case_radio;
    QRadioButton *snake_case_radio;
    QRadioButton *on_toggle;
    QRadioButton *off_toggle;
    QButtonGroup *notation_group;
    QButtonGroup *toggle_group;
    ProjectGenerator::FileNameNotation notation;
};

#endif // GENERATE_PROJECT_DIALOG_H
