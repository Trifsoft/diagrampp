#include "view/method_dialog.h"
#include <QRegularExpression>

MethodDialog::MethodDialog(QWidget* parent)
    : MemberDialogBase(parent)
{
    extra_setup_ui();
    setWindowTitle("Add New Method");
}

MethodDialog::MethodDialog(const Method& existing_method, QWidget* parent)
    : MemberDialogBase(parent)
{
    extra_setup_ui();
    populate_from_method(existing_method);
    setWindowTitle("Edit Method");
}

void MethodDialog::extra_setup_ui()
{
    // Method kind section
    m_method_kind_combo = new QComboBox(this);
    m_method_kind_combo->addItem("Regular", static_cast<int>(MethodKind::Regular));
    m_method_kind_combo->addItem("Virtual", static_cast<int>(MethodKind::RegularVirtual));
    m_method_kind_combo->addItem("Pure Virtual", static_cast<int>(MethodKind::PureVirtual));
    m_form_layout->addRow("Method Kind:", m_method_kind_combo);

    // Arguments section
    m_arguments_edit = new QPlainTextEdit(this);
    m_arguments_edit->setPlaceholderText("e.g., int x, const QString& name, double value");
    m_arguments_edit->setMaximumHeight(80);
    m_form_layout->addRow("Arguments:", m_arguments_edit);

    // Definition section
    m_definition_edit = new QPlainTextEdit(this);
    m_definition_edit->setPlaceholderText("// Method body (without signature)");
    m_definition_edit->setMaximumHeight(120);
    m_form_layout->addRow("Definition:", m_definition_edit);

    connect(m_method_kind_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MethodDialog::on_method_kind_changed);

    connect(m_arguments_edit, &QPlainTextEdit::textChanged,
            this, &MethodDialog::validate_input);
}

void MethodDialog::extra_validate(QString& error) const
{
    if (!error.isEmpty()) {
        return;
    }

    // Validate arguments format
    QString args_text = m_arguments_edit->toPlainText().trimmed();
    if (!args_text.isEmpty()) {
        QList<Argument> parsed = parse_arguments(args_text);
        if (parsed.isEmpty()) {
            error = "Invalid arguments format. Use: type name, type name, ...";
        }
    }
}

void MethodDialog::on_method_kind_changed(int index)
{
    auto kind = static_cast<MethodKind>(m_method_kind_combo->itemData(index).toInt());
    bool enable_definition = (kind != MethodKind::PureVirtual);
    m_definition_edit->setEnabled(enable_definition);
    if (!enable_definition) {
        m_definition_edit->clear();
    }
}

QList<Argument> MethodDialog::parse_arguments(const QString& text) const
{
    QList<Argument> result;
    if (text.trimmed().isEmpty()) {
        return result;
    }

    QStringList entries = text.split(',', Qt::SkipEmptyParts);
    QRegularExpression nameRegex("^[a-zA-Z_][a-zA-Z0-9_]*$");

    for (const QString& raw : entries) {
        QString trimmed = raw.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }

        QStringList tokens = trimmed.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        // Exit loop if argument can't be parsed
        if (tokens.size() < 2) {
            return {};
        }

        QString arg_name = tokens.last();
        QString arg_type = trimmed.left(trimmed.size() - arg_name.size()).trimmed();

        if (!nameRegex.match(arg_name).hasMatch()) {
            return {};
        }

        result.append(Argument(arg_name, arg_type));
    }

    return result;
}

void MethodDialog::populate_from_method(const Method& method)
{
    // Set visibility
    int vis_index = m_visibility_combo->findData(static_cast<int>(method.get_visibility()));
    if (vis_index != -1) {
        m_visibility_combo->setCurrentIndex(vis_index);
    }

    // Set return type and name
    m_type_edit->setText(method.get_return_type());
    m_name_edit->setText(method.get_name());

    // Set method kind
    int kind_index = m_method_kind_combo->findData(static_cast<int>(method.get_method_kind()));
    if (kind_index != -1) {
        m_method_kind_combo->setCurrentIndex(kind_index);
    }

    // Set arguments
    QStringList args_list;
    for (const auto& arg : method.get_arguments()) {
        args_list << QString("%1 %2").arg(arg.get_type(), arg.get_name());
    }
    m_arguments_edit->setPlainText(args_list.join(", "));

    // Set definition
    m_definition_edit->setPlainText(method.get_definition_block());

    on_method_kind_changed(m_method_kind_combo->currentIndex());
    validate_input();
}

MethodKind MethodDialog::get_method_kind() const
{
    return static_cast<MethodKind>(m_method_kind_combo->currentData().toInt());
}

QList<Argument> MethodDialog::get_arguments() const
{
    return parse_arguments(m_arguments_edit->toPlainText());
}

QString MethodDialog::get_definition_block() const
{
    return m_definition_edit->toPlainText();
}

std::optional<Method> MethodDialog::create_method(QWidget* parent)
{
    MethodDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        Method method(dialog.get_name(),
                      dialog.get_type(),
                      dialog.get_visibility(),
                      dialog.get_method_kind(),
                      dialog.get_arguments());
        method.set_definition_block(dialog.get_definition_block());
        return method;
    }
    return std::nullopt;
}

std::optional<Method> MethodDialog::edit_method(const Method& method, QWidget* parent)
{
    MethodDialog dialog(method, parent);
    if (dialog.exec() == QDialog::Accepted) {
        Method updated(dialog.get_name(),
                       dialog.get_type(),
                       dialog.get_visibility(),
                       dialog.get_method_kind(),
                       dialog.get_arguments());
        updated.set_definition_block(dialog.get_definition_block());
        return updated;
    }
    return std::nullopt;
}
