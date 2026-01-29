#ifndef METHODEDITOR_H
#define METHODEDITOR_H

#include <QWidget>
#include <model/elements/method.h>

namespace Ui {
class MethodEditor;
}

class MethodEditor : public QWidget
{
    Q_OBJECT

public:
    MethodEditor(std::weak_ptr<Method> method_weak, const QString& class_name, QWidget *parent = nullptr);
    ~MethodEditor();
private slots:
    void onSaveClick();

private:
    Ui::MethodEditor *ui;
    std::weak_ptr<Method> m_method_weak;
};

#endif // METHODEDITOR_H
