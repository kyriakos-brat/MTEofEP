#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "dialog_clroom.h"
#include "dialog_equipment.h"
#include "dialog_department.h"
#include "dialog_mrp.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    isConnect = false;

   ui->btnMrp->setEnabled(false);
   ui->btnDept->setEnabled(false);
   ui->btnEquip->setEnabled(false);
   ui->btnClroom->setEnabled(false);
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}


void MainWindow::on_btnConnect_clicked()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("MTEofEP");
    db.setUserName(ui->lineEdit_uname->text());
    db.setPassword(ui->lineEdit_password->text());
    bool ok = db.open();
    if (!ok){
        QMessageBox::critical(this, "CONNECTION ERROR", "ERROR: " + db.lastError().text());
        exit(-1);
    }
    else{
        ui->label_connection_info->setText("Connection established!");
        ui->btnMrp->setEnabled(true);
        ui->btnDept->setEnabled(true);
        ui->btnEquip->setEnabled(true);
        ui->btnClroom->setEnabled(true);
        isConnect = true;
    }
    ui->lineEdit_password->clear();
}

void MainWindow::on_btnClroom_clicked()
{
    if (isConnect){
        Dialog_clroom dlg_clroom;
        dlg_clroom.setModal(true);
        dlg_clroom.exec();
    }
}

void MainWindow::on_btnEquip_clicked()
{
    if (isConnect){
        Dialog_equipment dlg_equip;
        dlg_equip.setModal(true);
        dlg_equip.exec();
    }
}

void MainWindow::on_btnDept_clicked()
{
    if (isConnect){
        Dialog_department dlg_dept;
        dlg_dept.setModal(true);
        dlg_dept.exec();
    }
}

void MainWindow::on_btnMrp_clicked()
{
    if (isConnect){
        Dialog_mrp dlg_mrp;
        dlg_mrp.setModal(true);
        dlg_mrp.exec();
    }
}
