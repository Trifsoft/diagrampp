#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QGraphicsScene>

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
    void generateProjectFS(std::string& file_path, std::string& project_name);
    void generateProjectFiles();


private slots:
    void onAddClassClicked();
    void onAddInterfaceClicked();
    void onAddEnumClicked();
    void onGenerateProjectClicked();

private:
    std::string dirPaths[3];
    std::string projectPath;
    Ui::Project *ui;
};
#endif // PROJECT_H
