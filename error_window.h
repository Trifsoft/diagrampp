#ifndef ERROR_WINDOW_H
#define ERROR_WINDOW_H

#include <QDialog>

namespace Ui {
class error_window;
}

class error_window : public QDialog
{
    Q_OBJECT

public:
    explicit error_window(QWidget *parent = nullptr);
    ~error_window();

private:
    Ui::error_window *ui;
};

#endif // ERROR_WINDOW_H
