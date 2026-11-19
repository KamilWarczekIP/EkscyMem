#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "backend.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_wlaczone_wymuszenie_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::MainWindow *ui;
    backend backend;
};
#endif // MAINWINDOW_HPP
