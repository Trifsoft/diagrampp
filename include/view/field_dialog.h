#ifndef FIELD_DIALOG_H
#define FIELD_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <model/elements/field.h>
#include <model/elements/method.h>
#include <optional>

class FieldDialog : public QDialog
{
    Q_OBJECT

public:
    FieldDialog(QWidget* parent = nullptr);
    FieldDialog(const Field& existing_field, QWidget* parent = nullptr);

    QString get_field_name() const;
    QString get_field_type() const;
    Visibility get_visibility() const;
    bool has_destruction() const;
    std::optional<QString> get_destruction() const;
    QString get_description() const;

    static std::optional<Field> create_field(QWidget* parent = nullptr);
    static std::optional<Field> edit_field(const Field& field, QWidget* parent = nullptr);

private slots:
    void on_destruction_checkbox_changed(int state);
    void validate_input();

private:
    void setup_ui();
    void populate_from_field(const Field& field);

    // UI components
    QComboBox* m_visibility_combo;
    QLineEdit* m_type_edit;
    QLineEdit* m_name_edit;
    QCheckBox* m_destruction_checkbox;
    QLineEdit* m_destruction_edit;
    QLabel* m_error_label;
    QPushButton* m_ok_button;
    QPushButton* m_cancel_button;

    // validate input
    bool m_is_valid;
};

#endif // FIELD_DIALOG_H
