#include "methodeditor.h"
#include "ui_methodeditor.h"

MethodEditor::MethodEditor(Method* method, const QString& class_name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MethodEditor)
    , m_method(method)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("Implementation of " + method->get_declaration() + " in " + class_name);
    ui->textEdit->setText(method->get_definition_block());
    connect(ui->save, &QPushButton::clicked, this, &MethodEditor::onSaveClick);
}

MethodEditor::~MethodEditor()
{
    delete ui;
}

void MethodEditor::onSaveClick() {
    m_method->set_definition_block(ui->textEdit->toPlainText());
    close();
}
