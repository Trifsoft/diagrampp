#include "view/field_dialog.h"
#include "model/elements/type/regular_type.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

FieldDialog::FieldDialog(QWidget* parent)
    : QDialog(parent)
    , m_is_valid(false)
{
    setup_ui();
    setWindowTitle("Add New Field");
}

FieldDialog::FieldDialog(const Field& existing_field, QWidget* parent)
    : QDialog(parent)
    , m_is_valid(true)
{
    setup_ui();
    populate_from_field(existing_field);
    setWindowTitle("Edit Field");
}

void FieldDialog::setup_ui()
{
    // Main layout
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Form layout for invididual fields
    QFormLayout* form_layout = new QFormLayout();

    // Visibility, add icons potentially
    m_visibility_combo = new QComboBox(this);
    m_visibility_combo->addItem("Private", static_cast<int>(Visibility::Private));
    m_visibility_combo->addItem("Protected", static_cast<int>(Visibility::Protected));
    m_visibility_combo->addItem("Public", static_cast<int>(Visibility::Public));
    form_layout->addRow("Visibility:", m_visibility_combo);

    // Type
    m_type_edit = new QLineEdit(this);
    m_type_edit->setPlaceholderText("e.g., int, QString, std::vector<double>");
    QRegularExpression typeRegex("[a-zA-Z_][a-zA-Z0-9_:<>]*");
    m_type_edit->setValidator(new QRegularExpressionValidator(typeRegex, this));
    form_layout->addRow("Type:", m_type_edit);

    // Name
    m_name_edit = new QLineEdit(this);
    m_name_edit->setPlaceholderText("e.g., m_value, name_, items");
    QRegularExpression nameRegex("[a-zA-Z_][a-zA-Z0-9_]*");
    m_name_edit->setValidator(new QRegularExpressionValidator(nameRegex, this));
    form_layout->addRow("Name:", m_name_edit);


    // Destruction section
    m_destruction_checkbox = new QCheckBox("Has special destruction?", this);
    form_layout->addRow(m_destruction_checkbox);

    m_destruction_edit = new QLineEdit(this);
    m_destruction_edit->setPlaceholderText("e.g., delete[] data, close()");
    m_destruction_edit->setEnabled(false);
    form_layout->addRow("Destruction Code:", m_destruction_edit);

    main_layout->addLayout(form_layout);

    // Error label
    m_error_label = new QLabel(this);
    m_error_label->setStyleSheet("color: red;");
    m_error_label->setVisible(false);
    main_layout->addWidget(m_error_label);

    // Buttons
    QHBoxLayout* button_layout = new QHBoxLayout();
    m_ok_button = new QPushButton("OK", this);
    m_cancel_button = new QPushButton("Cancel", this);

    m_ok_button->setEnabled(false);

    button_layout->addStretch();
    button_layout->addWidget(m_ok_button);
    button_layout->addWidget(m_cancel_button);

    main_layout->addLayout(button_layout);

    // Connecting signals and slots
    connect(m_destruction_checkbox, &QCheckBox::checkStateChanged,
            this, &FieldDialog::on_destruction_checkbox_changed);

    connect(m_type_edit, &QLineEdit::textChanged, this, &FieldDialog::validate_input);
    connect(m_name_edit, &QLineEdit::textChanged, this, &FieldDialog::validate_input);

    connect(m_ok_button, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancel_button, &QPushButton::clicked, this, &QDialog::reject);

    setMinimumWidth(400);
}

void FieldDialog::on_destruction_checkbox_changed(int state)
{
    m_destruction_edit->setEnabled(state == Qt::Checked);
}

void FieldDialog::validate_input()
{
    QString error;

    if (m_type_edit->text().trimmed().isEmpty()) {
        error = "Type is required";
    } else if (m_name_edit->text().trimmed().isEmpty()) {
        error = "Name is required";
    } else if (!m_type_edit->hasAcceptableInput()) {
        error = "Invalid type format";
    } else if (!m_name_edit->hasAcceptableInput()) {
        error = "Invalid name format";
    }

    m_is_valid = error.isEmpty();
    m_ok_button->setEnabled(m_is_valid);
    m_error_label->setText(error);
    m_error_label->setVisible(!error.isEmpty());
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

QString FieldDialog::get_field_name() const
{
    return m_name_edit->text().trimmed();
}

QString FieldDialog::get_field_type() const
{
    return m_type_edit->text().trimmed();
}

Visibility FieldDialog::get_visibility() const
{
    return static_cast<Visibility>(m_visibility_combo->currentData().toInt());
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
        Field field(dialog.get_field_name(), dialog.get_field_type(), dialog.get_visibility(), dialog.get_destruction());

        return field;
    }
    return std::nullopt;
}

std::optional<Field> FieldDialog::edit_field(const Field& field, QWidget* parent)
{
    FieldDialog dialog(field, parent);
    if (dialog.exec() == QDialog::Accepted) {

        // Similar to create_field, to be tested!
        Field field(dialog.get_field_name(), dialog.get_field_type(), dialog.get_visibility(), dialog.get_destruction());

        return field;
    }
    return std::nullopt;
}
