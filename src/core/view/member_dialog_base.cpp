#include "view/member_dialog_base.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

MemberDialogBase::MemberDialogBase(QWidget* parent)
    : QDialog(parent)
    , m_is_valid(false)
{
    setup_base_ui();
}

void MemberDialogBase::setup_base_ui()
{
    setStyleSheet("background-color: black");
    // Main layout
    m_main_layout = new QVBoxLayout(this);

    // Form layout for invididual fields
    m_form_layout = new QFormLayout();

    // Visibility, add icons potentially
    m_visibility_combo = new QComboBox(this);
    m_visibility_combo->addItem("Private", static_cast<int>(Visibility::Private));
    m_visibility_combo->addItem("Protected", static_cast<int>(Visibility::Protected));
    m_visibility_combo->addItem("Public", static_cast<int>(Visibility::Public));
    m_form_layout->addRow("Visibility:", m_visibility_combo);

    // Type
    m_type_edit = new QLineEdit(this);
    m_type_edit->setPlaceholderText("e.g., int, QString, std::vector<double>");
    QRegularExpression type_regex(R"([a-zA-Z_][a-zA-Z0-9_\s:&*<>:,]*)");
    m_type_edit->setValidator(new QRegularExpressionValidator(type_regex, this));
    m_form_layout->addRow("Type:", m_type_edit);

    // Name
    m_name_edit = new QLineEdit(this);
    m_name_edit->setPlaceholderText("e.g., m_value, name_, items");
    QRegularExpression name_regex("[a-zA-Z_][a-zA-Z0-9_]*");
    m_name_edit->setValidator(new QRegularExpressionValidator(name_regex, this));
    m_form_layout->addRow("Name:", m_name_edit);

    m_main_layout->addLayout(m_form_layout);

    // Error label
    m_error_label = new QLabel(this);
    m_error_label->setStyleSheet("color: red;");
    m_error_label->setVisible(false);
    m_main_layout->addWidget(m_error_label);

    // Buttons
    QHBoxLayout* button_layout = new QHBoxLayout();
    m_ok_button = new QPushButton("OK", this);
    m_cancel_button = new QPushButton("Cancel", this);

    m_ok_button->setEnabled(false);

    button_layout->addStretch();
    button_layout->addWidget(m_ok_button);
    button_layout->addWidget(m_cancel_button);

    m_main_layout->addLayout(button_layout);

    // Connecting signals and slot
    connect(m_type_edit, &QLineEdit::textChanged, this, &MemberDialogBase::validate_input);
    connect(m_name_edit, &QLineEdit::textChanged, this, &MemberDialogBase::validate_input);

    connect(m_ok_button, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancel_button, &QPushButton::clicked, this, &QDialog::reject);

    // extra_setup_ui();

    setMinimumWidth(400);
}

void MemberDialogBase::validate_input()
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

    extra_validate(error);

    m_is_valid = error.isEmpty();
    m_ok_button->setEnabled(m_is_valid);
    m_error_label->setText(error);
    m_error_label->setVisible(!error.isEmpty());
}

QString MemberDialogBase::get_name() const
{
    return m_name_edit->text().trimmed();
}

QString MemberDialogBase::get_type() const
{
    return m_type_edit->text().trimmed();
}

Visibility MemberDialogBase::get_visibility() const
{
    return static_cast<Visibility>(m_visibility_combo->currentData().toInt());
}
