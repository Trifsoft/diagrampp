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
    MethodEditor(Method* method, const QString& class_name, QWidget *parent = nullptr);
    ~MethodEditor();
private slots:
    void onSaveClick();

private:
    Ui::MethodEditor *ui;
    Method* m_method;
};

#endif // METHODEDITOR_H
