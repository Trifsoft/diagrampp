#include "importproject.h"
#include <QLabel>
#include <QVBoxLayout>

ImportProject::ImportProject(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Import Project");

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    resize(400, 200);
}
