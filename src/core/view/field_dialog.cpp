#include "view/field_dialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

FieldDialog::FieldDialog(QWidget* parent)
    : MemberDialogBase(parent)
{
    extra_setup_ui();
    setWindowTitle("Add New Field");
}

FieldDialog::FieldDialog(const Field& existing_field, QWidget* parent)
    : MemberDialogBase(parent)
{
    extra_setup_ui();
    populate_from_field(existing_field);
    setWindowTitle("Edit Field");
}


void FieldDialog::extra_setup_ui()
{
    // Destruction section
    m_destruction_checkbox = new QCheckBox("Has special destruction?", this);
    m_form_layout->addRow(m_destruction_checkbox);

    m_destruction_edit = new QLineEdit(this);
    m_destruction_edit->setPlaceholderText("e.g., delete[] data, close()");
    m_destruction_edit->setEnabled(false);
    m_form_layout->addRow("Destruction Code:", m_destruction_edit);

    connect(m_destruction_checkbox, &QCheckBox::checkStateChanged,
            this, &FieldDialog::on_destruction_checkbox_changed);
}

void FieldDialog::on_destruction_checkbox_changed(int state)
{
    m_destruction_edit->setEnabled(state == Qt::Checked);
}

void FieldDialog::populate_from_field(const Field& field)
{
    // Set visibility
    int index = m_visibility_combo->findData(static_cast<int>(field.get_visibility() ));
    if (index != -1) {
        m_visibility_combo->setCurrentIndex(index);
    }

    // Set name and type
    m_type_edit->setText(field.get_type());
    m_name_edit->setText(field.get_name());

    // Sset destruction
    auto destruction = field.get_destruction();
    if (destruction.has_value()) {
        m_destruction_checkbox->setChecked(true);
        m_destruction_edit->setText(destruction.value());
        m_destruction_edit->setEnabled(true);
    }

    validate_input();
}

bool FieldDialog::has_destruction() const
{
    return m_destruction_checkbox->isChecked();
}

std::optional<QString> FieldDialog::get_destruction() const
{
    if (has_destruction()) {
        return m_destruction_edit->text();
    }else{
        return std::nullopt;
    }
}

std::optional<Field> FieldDialog::create_field(QWidget* parent)
{
    FieldDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {

        // Create a field
        Field field(dialog.get_name(),
                    dialog.get_type(),
                    dialog.get_visibility(),
                    dialog.get_destruction());

        return field;
    }
    return std::nullopt;
}

std::optional<Field> FieldDialog::edit_field(const Field& field, QWidget* parent)
{
    FieldDialog dialog(field, parent);
    if (dialog.exec() == QDialog::Accepted) {

        Field updated(dialog.get_name(),
                      dialog.get_type(),
                      dialog.get_visibility(),
                      dialog.get_destruction());

        return updated;
    }
    return std::nullopt;
}
