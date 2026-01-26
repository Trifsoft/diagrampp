#ifndef MEMBER_DIALOG_BASE_H
#define MEMBER_DIALOG_BASE_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <model/base/visibility.h>

class IClassElement;

class MemberDialogBase : public QDialog {
    Q_OBJECT

public:
    explicit MemberDialogBase(QWidget* parent = nullptr);
    QString get_name() const;
    QString get_type() const;
    Visibility get_visibility() const;

protected slots:
    void validate_input();

protected:
    QVBoxLayout*   m_main_layout;
    QFormLayout*   m_form_layout;
    QComboBox*     m_visibility_combo;
    QLineEdit*     m_type_edit;
    QLineEdit*     m_name_edit;
    QLabel*        m_error_label;
    QPushButton*   m_ok_button;
    QPushButton*   m_cancel_button;
    bool           m_is_valid;

    void setup_base_ui();
    virtual void extra_setup_ui() {}
    virtual void extra_validate(QString& error) const {}

};

#endif //MEMBER_DIALOG_BASE_H
