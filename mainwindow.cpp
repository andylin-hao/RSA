#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->crypt_btn->setEnabled(false);
    ui->encrypted_edit->setReadOnly(true);
    ui->decrypted_edit->setReadOnly(true);

    key_len = 116;
    ui->len_edit->setText("768");
    rsa = RSA();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_gen_key_btn_clicked()
{
    rsa.gen_key(key_len);
    ui->public_key_edit->setText(QString::fromStdString(rsa.public_key_n.to_string())+QString::fromStdString(rsa.public_key_e.to_string()));
    ui->private_key_edit->setText(QString::fromStdString(rsa.private_key.to_string()));
    ui->crypt_btn->setEnabled(true);
}

void MainWindow::on_crypt_btn_clicked()
{
    if (ui->crypt_btn->text() == "Encrypt") {
        string msg = ui->msg_edit->toPlainText().toStdString();
        if (msg.empty()) {
            QMessageBox::warning(this, "No Message", "No Message to Encrypt", QMessageBox::Yes);
            return;
        }

        ui->encrypted_edit->setText(QString::fromStdString(rsa.encrypt_by_public(msg)));
        ui->crypt_btn->setText("Decrypt");
    } else {
        string msg = ui->encrypted_edit->toPlainText().toStdString();
        if (msg.empty()) {
            QMessageBox::warning(this, "No Message", "No Message to Encrypt", QMessageBox::Yes);
            return;
        }

        ui->decrypted_edit->setText(QString::fromStdString(rsa.decrypt_by_private(msg)));
        ui->crypt_btn->setText("Encrypt");
    }
}

void MainWindow::on_len_edit_returnPressed()
{
    key_len = stoll(ui->len_edit->text().toStdString());
    key_len = (log10(2) * key_len) / 2;
}
