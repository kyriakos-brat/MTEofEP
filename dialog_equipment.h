#ifndef DIALOG_EQUIPMENT_H
#define DIALOG_EQUIPMENT_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>

namespace Ui {
class Dialog_equipment;
}

class Dialog_equipment : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_equipment(QWidget *parent = nullptr);
    ~Dialog_equipment();

private slots:
    void on_btnAddEquip_clicked();

    void on_tableView_equipment_clicked(const QModelIndex &index);

    void on_btnChgEquip_clicked();

    void on_btnDelEquip_clicked();

private:
    Ui::Dialog_equipment *ui;
    QSqlDatabase db;
    QSqlQueryModel *equipment;
    QSqlQuery query_equip;
    int id_curr;
    bool ok;
};

#endif // DIALOG_EQUIPMENT_H
