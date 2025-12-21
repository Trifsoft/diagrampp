#include "new_project.h"
#include <QLabel>
#include <QVBoxLayout>

NewProject::NewProject(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("New Project");

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    resize(400, 200);
}
