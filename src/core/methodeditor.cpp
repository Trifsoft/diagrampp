#include "methodeditor.h"
#include "ui_methodeditor.h"

MethodEditor::MethodEditor(std::weak_ptr<Method> method_weak, const QString& class_name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MethodEditor)
    , m_method_weak(method_weak)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);


    if(auto method = method_weak.lock()){
        setWindowTitle("Implementation of " + method->declaration() + " in " + class_name);
        ui->textEdit->setText(method->get_definition_block());
        connect(ui->save, &QPushButton::clicked, this, &MethodEditor::onSaveClick);
    }
}

MethodEditor::~MethodEditor()
{
    delete ui;
}

void MethodEditor::onSaveClick() {
    if(auto method = m_method_weak.lock()){
        method->set_definition_block(ui->textEdit->toPlainText());
        close();
    }
}
