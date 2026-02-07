#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QWidget>

class NewProject : public QWidget
{
    Q_OBJECT
public:
    explicit NewProject(QWidget *parent = nullptr);
    ~NewProject() = default;
};

#endif // NEWPROJECT_H
