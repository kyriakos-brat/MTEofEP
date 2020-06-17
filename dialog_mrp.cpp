#include "dialog_mrp.h"
#include "ui_dialog_mrp.h"
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>

Dialog_mrp::Dialog_mrp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_mrp)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("db");
    QSqlQuery query_title_dept = QSqlQuery(db);
    ok = query_title_dept.exec("SELECT title FROM department ORDER BY title");
    if (!ok)
        QMessageBox::critical(this, "ERROR", "ERROR: " + query_title_dept.lastError().text());
    else{
        while (query_title_dept.next())
            ui->comboBox_dept->addItem(query_title_dept.value(0).toString());
    }

    mrp = new QSqlRelationalTableModel(this, db);
    mrp->setTable("mrp");
    mrp->setRelation(3,QSqlRelation("department", "id_dept", "title") );
    mrp->select();
    mrp->setHeaderData(0, Qt::Horizontal, "ID");
    mrp->setHeaderData(1, Qt::Horizontal, "Fullname");
    mrp->setHeaderData(2, Qt::Horizontal, "Post");
    mrp->setHeaderData(3, Qt::Horizontal, "Department");
    mrp->setHeaderData(4, Qt::Horizontal, "Reprimands");
    mrp->setHeaderData(5, Qt::Horizontal, "Date of birth");
    ui->tableView_mrp->setModel(mrp);
    ui->lineEdit_fullname->setPlaceholderText("Enter MRP's fullname here");
    ui->lineEdit_post->setPlaceholderText("Enter MRP's post here");
    ui->lineEdit_sub_fullname->setPlaceholderText("For delete only");
    ui->lineEdit_newFname->setPlaceholderText("New fullname (change only)");
}

Dialog_mrp::~Dialog_mrp()
{
    delete ui;
}

void Dialog_mrp::on_btnAddMrp_clicked()
{
    QSqlQuery query_add_mrp = QSqlQuery(db);
    QString fname(ui->lineEdit_fullname->text());
    QString dob(ui->dateEdit_dob->date().toString("yyyy.MM.dd"));
    QString post(ui->lineEdit_post->text());
    QString dept(ui->comboBox_dept->currentText());
    ok = query_add_mrp.exec("CALL mrp_add('" + fname + "','"
                            + dob + "','" + post + "','" + dept + "')");
    if (!ok)
        QMessageBox::critical(this, "Adding error", "ERROR: " + query_add_mrp.lastError().text());
    else{
        ui->label_status->setText("Added successfully!");
        ui->lineEdit_fullname->clear();
        ui->lineEdit_post->clear();
        ui->dateEdit_dob->setDate(QDate::fromString("01.01.2000", "dd.MM.yyyy"));
        ui->comboBox_dept->setCurrentIndex(0);
        mrp->select();
    }
}

void Dialog_mrp::on_tableView_mrp_clicked(const QModelIndex &index)
{
    QModelIndex idx_id = index.siblingAtColumn(0);
    QModelIndex idx_fname = index.siblingAtColumn(1);
    QModelIndex idx_dob = index.siblingAtColumn(5);
    QModelIndex idx_post = index.siblingAtColumn(2);
    QModelIndex idx_dept = index.siblingAtColumn(3);

    id_curr = ui->tableView_mrp->model()->data(idx_id).toInt();
    QString set_fname = ui->tableView_mrp->model()->data(idx_fname).toString();
    QDate set_dob = ui->tableView_mrp->model()->data(idx_dob).toDate();
    QString set_post = ui->tableView_mrp->model()->data(idx_post).toString();
    QString set_dept = ui->tableView_mrp->model()->data(idx_dept).toString();

    int cmb_dept_pos = ui->comboBox_dept->findText(set_dept);

    ui->lineEdit_fullname->setText(set_fname);
    ui->dateEdit_dob->setDate(set_dob);
    ui->comboBox_dept->setCurrentIndex(cmb_dept_pos);
    ui->lineEdit_post->setText(set_post);
}

void Dialog_mrp::on_btnDelMrp_clicked()
{
    QSqlQuery query_del_mrp = QSqlQuery(db);

    QString fname(ui->lineEdit_fullname->text());
    QString dob(ui->dateEdit_dob->date().toString("yyyy.MM.dd"));

    QString sub_fname(ui->lineEdit_sub_fullname->text());
    QString sub_dob(ui->dateEdit_sub_dob->date().toString("yyyy.MM.dd"));
    ok = query_del_mrp.exec("CALL mrp_delete('" + fname +  "','" + dob + "','"
                            + sub_fname + "','" + sub_dob  + "')");
    if (!ok)
        QMessageBox::critical(this, "Deleting error", "ERROR: " + query_del_mrp.lastError().text());
    else{
        ui->label_status->setText("Deleted successfully!");
        ui->lineEdit_fullname->clear();
        ui->lineEdit_post->clear();
        ui->dateEdit_dob->setDate(QDate::fromString("01.01.2000", "dd.MM.yyyy"));
        ui->comboBox_dept->setCurrentIndex(0);
        mrp->select();
    }
}

void Dialog_mrp::on_btnChgMrp_clicked()
{
    QSqlQuery query_upd_mrp = QSqlQuery(db);
    QString fname(ui->lineEdit_fullname->text());
    QString dob(ui->dateEdit_dob->date().toString("yyyy.MM.dd"));
    QString post(ui->lineEdit_post->text());
    QString dept(ui->comboBox_dept->currentText());
    QString query("CALL mrp_update('" + fname + "','" + dob + "',");


    if (ui->chkPost->isChecked())
        query.append("'" + ui->lineEdit_post->text() + "',");
    else
        query.append("NULL,");

    if (ui->chkDept->isChecked())
        query.append("'" + ui->comboBox_dept->currentText() + "',");
    else
        query.append("NULL,");

    if (ui->chkFname->isChecked())
        query.append("'" + ui->lineEdit_newFname->text() + "'");
    else
        query.append("NULL");

    query.append(")");

    qDebug() << query;
    if (!(ui->chkDept->isChecked() || ui->chkPost->isChecked() || ui->chkFname->isChecked())){
        QMessageBox::information(this, "No info to change", "Info for change wasn't selected");
        return;
    }

    ok = query_upd_mrp.exec(query);
    if (!ok)
        QMessageBox::critical(this, "Changing error", "ERROR: " + query_upd_mrp.lastError().text());
    else{
        ui->label_status->setText("Changed successfully!");
        ui->lineEdit_fullname->clear();
        ui->lineEdit_post->clear();
        ui->dateEdit_dob->setDate(QDate::fromString("01.01.2000", "dd.MM.yyyy"));
        ui->comboBox_dept->setCurrentIndex(0);
        mrp->select();
    }

}
