#ifndef IMPORTPROJECT_H
#define IMPORTPROJECT_H

#include <QWidget>

class ImportProject: public QWidget
{
    Q_OBJECT
public:
    explicit ImportProject(QWidget *parent = nullptr);
    ~ImportProject() = default;
};

#endif // IMPORTPROJECT_H

