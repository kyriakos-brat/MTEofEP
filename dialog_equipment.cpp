#include "dialog_equipment.h"
#include "ui_dialog_equipment.h"
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QScrollBar>

Dialog_equipment::Dialog_equipment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_equipment)
{
    ui->setupUi(this);
    id_curr = -1;
    db = QSqlDatabase::database("db");
    equipment = new QSqlTableModel(this, db);
    QSqlQuery query_clroom_title = QSqlQuery(db);
    ok = query_clroom_title.exec("SELECT title FROM classroom ORDER BY id_clroom");
    if (!ok)
        QMessageBox::critical(this, "ERROR", "ERROR: " + query_clroom_title.lastError().text());
    else{
        while (query_clroom_title.next())
            ui->comboBox_clroom->addItem(query_clroom_title.value(0).toString());
    }

    QSqlQuery query_mrp_fname = QSqlQuery(db);
    ok = query_mrp_fname.exec("SELECT fullname FROM mrp ORDER BY fullname");
    if (!ok)
        QMessageBox::critical(this, "ERROR", "ERROR: " + query_mrp_fname.lastError().text());
    else{
        while (query_mrp_fname.next())
            ui->comboBox_mrp->addItem(query_mrp_fname.value(0).toString());
    }
    query_equip = QSqlQuery(db);
    ok = query_equip.exec("SELECT equipment.id_equip, equipment.title, "
                          "classroom.id_clroom, mrp.fullname, equipment.start_cost, equipment.actual_cost, "
                          "equipment.date_of_last_cost "
                          "FROM equipment J"
                          "OIN classroom ON equipment.id_clroom = classroom.id_clroom "
                          "JOIN mrp ON mrp.id_mrp = equipment.id_mrp");
    if (!ok)
        qDebug() << query_equip.lastError().text();
    equipment->setQuery(query_equip);
    equipment->setHeaderData(0, Qt::Horizontal, "ID");
    equipment->setHeaderData(1, Qt::Horizontal, "Title");
    equipment->setHeaderData(2, Qt::Horizontal, "Classroom");
    equipment->setHeaderData(3, Qt::Horizontal, "MRP");
    equipment->setHeaderData(4, Qt::Horizontal, "Start cost");
    equipment->setHeaderData(5, Qt::Horizontal, "Actual cost");
    equipment->setHeaderData(6, Qt::Horizontal, "Date of last cost");
    ui->tableView_equipment->setModel(equipment);
    ui->lineEdit_title->setPlaceholderText("Enter equipment title here");

}

Dialog_equipment::~Dialog_equipment()
{
    delete ui;
}

void Dialog_equipment::on_btnAddEquip_clicked()
{
    QSqlQuery query_add_equip = QSqlQuery(db);
    query_add_equip.prepare("SELECT * FROM equip_add(:clroom_title, :mrp_fname, :cost, :equip_title)");
    query_add_equip.bindValue(":clroom_title", ui->comboBox_clroom->currentText());
    query_add_equip.bindValue(":mrp_fname", ui->comboBox_mrp->currentText());
    query_add_equip.bindValue(":cost", ui->spinBox_cost->value());
    query_add_equip.bindValue(":equip_title", ui->lineEdit_title->text());
    ok = query_add_equip.exec();
    if (!ok)
        QMessageBox::critical(this, "Adding error!", "ERROR: " + query_add_equip.lastError().text());
    else{
       ui->label_status->setText("Added successfully!");
       QSqlQuery query_refresh = QSqlQuery(db);
       ok = query_refresh.exec("SELECT equipment.id_equip, equipment.title, "
                             "classroom.id_clroom, mrp.fullname, equipment.start_cost, equipment.actual_cost, "
                             "equipment.date_of_last_cost "
                             "FROM equipment J"
                             "OIN classroom ON equipment.id_clroom = classroom.id_clroom "
                             "JOIN mrp ON mrp.id_mrp = equipment.id_mrp");
       equipment->setQuery(query_refresh);
       ui->comboBox_clroom->setCurrentIndex(0);
       ui->comboBox_mrp->setCurrentIndex(0);
       ui->lineEdit_title->clear();
       ui->spinBox_cost->setValue(0);

    }
}

void Dialog_equipment::on_tableView_equipment_clicked(const QModelIndex &index)
{
    QModelIndex idx_clroom = index.siblingAtColumn(2);
    QModelIndex idx_mrp = index.siblingAtColumn(3);
    QModelIndex idx_actual_cost = index.siblingAtColumn(5);
    QModelIndex idx_title = index.siblingAtColumn(1);
    QModelIndex idx_id_to_change = index.siblingAtColumn(0);

    QString set_clroom = ui->tableView_equipment->model()->data(idx_clroom).toString();
    QString set_mrp = ui->tableView_equipment->model()->data(idx_mrp).toString();
    int set_actual_cost = ui->tableView_equipment->model()->data(idx_actual_cost).toInt();
    QString set_title = ui->tableView_equipment->model()->data(idx_title).toString();
    id_curr = ui->tableView_equipment->model()->data(idx_id_to_change).toInt();

    int cmb_clroom_pos = ui->comboBox_clroom->findText(set_clroom);
    int cmb_mrp_pos = ui->comboBox_mrp->findText(set_mrp);

    ui->comboBox_clroom->setCurrentIndex(cmb_clroom_pos);
    ui->comboBox_mrp->setCurrentIndex(cmb_mrp_pos);
    ui->spinBox_cost->setValue(set_actual_cost);
    ui->lineEdit_title->setText(set_title);
}

void Dialog_equipment::on_btnChgEquip_clicked()
{
    QString set_clroom = ui->comboBox_clroom->currentText();
    QString set_mrp = ui->comboBox_mrp->currentText();
    QString set_actual_cost = QString::number(ui->spinBox_cost->value());
    QString set_title = ui->lineEdit_title->text();

    QSqlQuery query_equip_change = QSqlQuery(db);
    query_equip_change.prepare("SELECT * FROM equip_update(:id, :clroom, :mrp_fname, :actual_cost, :title)");

    query_equip_change.bindValue(":id", id_curr);
    if (!ui->checkBox_clroom->isChecked())
        query_equip_change.bindValue(":clroom", QVariant(QVariant::String));
    else
        query_equip_change.bindValue(":clroom", set_clroom);

    if (!ui->checkBox_mrp->isChecked())
        query_equip_change.bindValue(":mrp_fname", QVariant(QVariant::String));
    else
        query_equip_change.bindValue(":mrp_fname", set_mrp);

    if (!ui->checkBox_cost->isChecked())
        query_equip_change.bindValue(":actual_cost", QVariant(QVariant::String));
    else
        query_equip_change.bindValue(":actual_cost", set_actual_cost);

    if (!ui->checkBox_title->isChecked())
        query_equip_change.bindValue(":title", QVariant(QVariant::String));
    else
        query_equip_change.bindValue(":title", set_title);

    qDebug() << query_equip_change.boundValue(":id")
                << query_equip_change.boundValue(":clroom")
                   << query_equip_change.boundValue(":mrp_fname")
                      << query_equip_change.boundValue(":actual_cost")
                         << query_equip_change.boundValue(":title");
    ok = query_equip_change.exec();
    if (!ok)
        QMessageBox::critical(this, "Changing error!", "ERROR: " + query_equip_change.lastError().text());
    else{
       ui->label_status->setText("Changed successfully!");
       QSqlQuery query_refresh = QSqlQuery(db);
       ok = query_refresh.exec("SELECT equipment.id_equip, equipment.title, "
                             "classroom.title, mrp.fullname, equipment.start_cost, equipment.actual_cost, "
                             "equipment.date_of_last_cost "
                             "FROM equipment "
                             "JOIN classroom ON equipment.id_clroom = classroom.id_clroom "
                             "JOIN mrp ON mrp.id_mrp = equipment.id_mrp");
       equipment->setQuery(query_refresh);
       ui->comboBox_clroom->setCurrentIndex(0);
       ui->comboBox_mrp->setCurrentIndex(0);
       ui->lineEdit_title->clear();
       ui->spinBox_cost->setValue(0);

    }
}

void Dialog_equipment::on_btnDelEquip_clicked()
{
    QSqlQuery query_del_equip = QSqlQuery(db);
    query_del_equip.prepare("DELETE FROM equipment WHERE id_equip = :id_to_del");
    query_del_equip.bindValue(":id_to_del", id_curr);
    ok = query_del_equip.exec();
    if (!ok)
        QMessageBox::critical(this, "Deleting error!", "ERROR: " + query_del_equip.lastError().text());
    else{
        ui->label_status->setText("Deleted successfully!");
        QSqlQuery query_refresh = QSqlQuery(db);
        ok = query_refresh.exec("SELECT equipment.id_equip, equipment.title, "
                                "classroom.id_clroom, mrp.fullname, equipment.start_cost, equipment.actual_cost, "
                                "equipment.date_of_last_cost "
                                "FROM equipment J"
                                "OIN classroom ON equipment.id_clroom = classroom.id_clroom "
                                "JOIN mrp ON mrp.id_mrp = equipment.id_mrp");
        equipment->setQuery(query_refresh);
        ui->comboBox_clroom->setCurrentIndex(0);
        ui->comboBox_mrp->setCurrentIndex(0);
        ui->lineEdit_title->clear();
        ui->spinBox_cost->setValue(0);
    }
}
