#ifndef METHOD_DIALOG_H
#define METHOD_DIALOG_H

#include <view/member_dialog_base.h>
#include <model/elements/method.h>
#include <QComboBox>
#include <QPlainTextEdit>

class MethodDialog final: public MemberDialogBase
{
    Q_OBJECT

public:
    explicit MethodDialog(QWidget* parent = nullptr);
    MethodDialog(const Method& existing_method, QWidget* parent = nullptr);

    MethodKind get_method_kind() const;
    QList<Argument*> get_arguments() const;
    QString get_definition_block() const;

    static std::shared_ptr<Method> create_method(QWidget* parent = nullptr);
    static std::shared_ptr<Method> edit_method(const Method& method, QWidget* parent = nullptr);

private slots:
    void on_method_kind_changed(int index);

protected:
    void extra_setup_ui() override;
    void extra_validate(QString& error) const override;

private:
    void populate_from_method(const Method& method);
    QList<Argument *> parse_arguments(const QString& text) const;

    QComboBox*      m_method_kind_combo;
    QPlainTextEdit* m_arguments_edit;
    QPlainTextEdit* m_definition_edit;
};

#endif // METHOD_DIALOG_H
