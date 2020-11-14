#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include "RSA.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gen_key_btn_clicked();
    void on_crypt_btn_clicked();

    void on_len_edit_returnPressed();

private:
    Ui::MainWindow *ui;
    long long key_len;
    RSA rsa;
};
#endif // MAINWINDOW_H
