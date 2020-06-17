#include "dialog_clroom.h"
#include "ui_dialog_clroom.h"
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlTableModel>

Dialog_clroom::Dialog_clroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_clroom)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("db");
    QSqlQuery enum_query = QSqlQuery(db);
    ok = enum_query.exec("SELECT enumlabel FROM pg_enum WHERE enumtypid = 24612");
    if (!ok)
        QMessageBox::critical(this, "ERROR", "ERROR: " + enum_query.lastError().text());
    else{
        while (enum_query.next())
            ui->comboBox_enum->addItem(enum_query.value(0).toString());
    }

    classroom = new QSqlTableModel(this, db);
    classroom->setTable("classroom");
    classroom->select();
    classroom->setHeaderData(0, Qt::Horizontal,"ID");
    classroom->setHeaderData(1, Qt::Horizontal,"Room type");
    classroom->setHeaderData(2, Qt::Horizontal,"Area");
    classroom->setHeaderData(3, Qt::Horizontal,"Date of last maintenance");
    classroom->setHeaderData(4, Qt::Horizontal,"Class log");
    classroom->setHeaderData(5, Qt::Horizontal,"Title");
    ui->tableView_classroom->setModel(classroom);
    ui->lineEdit_title->setPlaceholderText("Enter classroom title here");
}

Dialog_clroom::~Dialog_clroom()
{
    delete ui;
}


void Dialog_clroom::on_btnAdd_clicked()
{
    QSqlQuery query_add_clroom = QSqlQuery(db);
    QString room_title(ui->lineEdit_title->text());
    int room_area = ui->spinBox_area->value();
    QString room_type(ui->comboBox_enum->currentText());

    query_add_clroom.prepare("INSERT INTO classroom (type_of_room, area, title) VALUES (:type, :area, :title)");
    query_add_clroom.bindValue(":type", room_type);
    query_add_clroom.bindValue(":area", room_area);
    query_add_clroom.bindValue(":title", room_title);
    ok = query_add_clroom.exec();
    if (!ok)
        QMessageBox::critical(this, "ERROR", "Adding error: " + query_add_clroom.lastError().text());
    else{
        ui->label_status->setText("Added successfully!");
        ui->lineEdit_title->clear();
        ui->comboBox_enum->setCurrentIndex(0);
        ui->spinBox_area->clear();
        classroom->select();
    }



}

void Dialog_clroom::on_tableView_classroom_clicked(const QModelIndex &index)
{
    QModelIndex idx_id = index.siblingAtColumn(0);
    QModelIndex idx_type = index.siblingAtColumn(1);
    QModelIndex idx_area = index.siblingAtColumn(2);
    QModelIndex idx_date = index.siblingAtColumn(3);
    QModelIndex idx_title = index.siblingAtColumn(5);

    id_curr = ui->tableView_classroom->model()->data(idx_id).toInt();
    QString set_type(ui->tableView_classroom->model()->data(idx_type).toString());
    int set_area = ui->tableView_classroom->model()->data(idx_area).toInt();
    QDate set_date = ui->tableView_classroom->model()->data(idx_date).toDate();
    QString set_title(ui->tableView_classroom->model()->data(idx_title).toString());

    ui->lineEdit_title->setText(set_title);
    ui->spinBox_area->setValue(set_area);
    ui->comboBox_enum->setCurrentIndex(ui->comboBox_enum->findText(set_type));
    ui->dateEdit_maintenance->setDate(set_date);


}

void Dialog_clroom::on_btnDel_clicked()
{
    QSqlQuery query_clroom_del = QSqlQuery(db);
    query_clroom_del.prepare("DELETE FROM classroom WHERE id_clroom = :id");
    query_clroom_del.bindValue(":id", id_curr);
    ok = query_clroom_del.exec();
    if (!ok)
        QMessageBox::critical(this, "ERROR", "Deleting error: " + query_clroom_del.lastError().text());
    else{
        ui->label_status->setText("Deleted successfully!");
        ui->lineEdit_title->clear();
        ui->comboBox_enum->setCurrentIndex(0);
        ui->spinBox_area->clear();
        classroom->select();
    }

}

void Dialog_clroom::on_btnChg_clicked()
{
    QString title(ui->lineEdit_title->text());
    QString type(ui->comboBox_enum->currentText());
    int area = ui->spinBox_area->value();
    QString date(ui->dateEdit_maintenance->date().toString("yyyy.MM.dd"));

    QSqlQuery query_clroom_chg = QSqlQuery(db);
    query_clroom_chg.prepare("SELECT * FROM clroom_update(:id, :title, :type, :area, :date)");
    query_clroom_chg.bindValue(":id", id_curr);
    if (!ui->checkBox_title->isChecked())
        query_clroom_chg.bindValue(":title", QVariant(QVariant::String));
    else
        query_clroom_chg.bindValue(":title", title);

    if (!ui->checkBox_type->isChecked())
        query_clroom_chg.bindValue(":type", QVariant(QVariant::String));
    else
        query_clroom_chg.bindValue(":type", type);

    if (!ui->checkBox_area->isChecked())
        query_clroom_chg.bindValue(":area", QVariant(QVariant::String));
    else
        query_clroom_chg.bindValue(":area", area);

    if (!ui->checkBox_date->isChecked())
        query_clroom_chg.bindValue(":date", QVariant(QVariant::String));
    else
        query_clroom_chg.bindValue(":date", date);
    ok = query_clroom_chg.exec();
    if (!ok)
        QMessageBox::critical(this, "ERROR", "Deleting error: " + query_clroom_chg.lastError().text());
    else{
        ui->label_status->setText("Changed successfully!");
        ui->lineEdit_title->clear();
        ui->comboBox_enum->setCurrentIndex(0);
        ui->spinBox_area->clear();
        classroom->select();
    }

}
