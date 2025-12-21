#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QGraphicsScene>
#include "CppClassItem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Project;
}
QT_END_NAMESPACE

class Project : public QWidget
{
    QGraphicsScene* scene;

    Q_OBJECT

public:
    Project(QWidget *parent = nullptr);
    ~Project();

private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();

private:
    Ui::Project *ui;
};
#endif // PROJECT_H
