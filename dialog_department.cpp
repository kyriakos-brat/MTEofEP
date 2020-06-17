#include "dialog_department.h"
#include "ui_dialog_department.h"
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlTableModel>

Dialog_department::Dialog_department(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_department)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("db");
    department = new QSqlTableModel(this, db);
    department->setTable("department");
    department->select();
    department->setHeaderData(0, Qt::Horizontal, "ID");
    department->setHeaderData(1, Qt::Horizontal, "Title");
    department->setHeaderData(2, Qt::Horizontal, "Amount of MRPs");
    ui->tableView_department->setModel(department);
    id_curr = -1;
    ui->lineEdit_deptTitle->setPlaceholderText("Enter department title here");
}

Dialog_department::~Dialog_department()
{
    delete ui;
}

void Dialog_department::on_btnAddDept_clicked()
{
    QSqlQuery query_add_dept = QSqlQuery(db);
    ui->label_status->clear();
    query_add_dept.prepare("INSERT INTO department(title) VALUES (:title)");
    query_add_dept.bindValue(":title", ui->lineEdit_deptTitle->text());
    ok = query_add_dept.exec();
    if (!ok)
        QMessageBox::critical(this, "Adding error!", "ERROR: " + query_add_dept.lastError().text());
    else{
        ui->lineEdit_deptTitle->clear();
        ui->label_status->setText("Added succefully!");
        department->select();
    }
}

void Dialog_department::on_tableView_department_clicked(const QModelIndex &index)
{
    QModelIndex set_id = index.siblingAtColumn(0);
    QModelIndex set_title = index.siblingAtColumn(1);

    QString dept_title = ui->tableView_department->model()->data(set_title).toString();
    ui->lineEdit_deptTitle->setText(dept_title);
    id_curr = ui->tableView_department->model()->data(set_id).toInt();

}

void Dialog_department::on_btnDelDept_clicked()
{
    QSqlQuery query_del_dept = QSqlQuery(db);
    query_del_dept.prepare("DELETE FROM department WHERE id_dept = :sel_id");
    query_del_dept.bindValue(":sel_id", id_curr);
    qDebug() << query_del_dept.boundValue(":sel_id").toInt();
    ok = query_del_dept.exec();
    if (!ok)
        QMessageBox::critical(this, "Deleting error!", "ERROR: " + query_del_dept.lastError().text());
    else{
        ui->lineEdit_deptTitle->clear();
        ui->label_status->setText("Deleted succefully!");
        department->select();
    }
}

void Dialog_department::on_btnChgDept_clicked()
{
    QSqlQuery query_chg_dept = QSqlQuery(db);
    query_chg_dept.prepare("UPDATE department SET title = :new_title WHERE id_dept = :id");
    query_chg_dept.bindValue(":new_title", ui->lineEdit_deptTitle->text());
    query_chg_dept.bindValue(":id", id_curr);
    ok = query_chg_dept.exec();
    if (!ok)
        QMessageBox::critical(this, "Changing error!", "ERROR: " + query_chg_dept.lastError().text());
    else{
        ui->lineEdit_deptTitle->clear();
        ui->label_status->setText("Changed succefully!");
        department->select();
    }
}
