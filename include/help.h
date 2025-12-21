#ifndef HELP_H
#define HELP_H

#include <QWidget>

class Help: public QWidget
{
    Q_OBJECT
public:
    explicit Help(QWidget *parent = nullptr);
    ~Help() = default;
};

#endif // HELP_H
