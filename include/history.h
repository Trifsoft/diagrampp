#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>

class History: public QWidget
{
    Q_OBJECT
public:
    explicit History(QWidget *parent = nullptr);
    ~History() = default;
};

#endif // HISTORY_H
