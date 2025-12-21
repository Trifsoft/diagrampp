#include "help.h"
#include <QLabel>
#include <QVBoxLayout>

Help::Help(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Help");

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    resize(400, 200);
}
