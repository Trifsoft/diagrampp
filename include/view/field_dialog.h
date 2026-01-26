#ifndef FIELD_DIALOG_H
#define FIELD_DIALOG_H

#include <view/member_dialog_base.h>
#include <model/elements/field.h>

class FieldDialog final: public MemberDialogBase
{
    Q_OBJECT

public:
    explicit FieldDialog(QWidget* parent = nullptr);
    FieldDialog(const Field& existing_field, QWidget* parent = nullptr);

    bool has_destruction() const;
    std::optional<QString> get_destruction() const;
    QString get_description() const;

    static std::optional<Field> create_field(QWidget* parent = nullptr);
    static std::optional<Field> edit_field(const Field& field, QWidget* parent = nullptr);

private slots:
    void on_destruction_checkbox_changed(int state);

protected:
    void extra_setup_ui() override;
    void extra_validate(QString& error) const override {};

private:
    void populate_from_field(const Field& field);

    QCheckBox* m_destruction_checkbox;
    QLineEdit* m_destruction_edit;

};

#endif // FIELD_DIALOG_H
