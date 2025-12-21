#include "history.h"
#include <QLabel>
#include <QVBoxLayout>

History::History(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("History");

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    resize(400, 200);
}
