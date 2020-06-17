#ifndef DIALOG_MRP_H
#define DIALOG_MRP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

namespace Ui {
class Dialog_mrp;
}

class Dialog_mrp : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_mrp(QWidget *parent = nullptr);
    ~Dialog_mrp();

private slots:
    void on_btnAddMrp_clicked();

    void on_tableView_mrp_clicked(const QModelIndex &index);

    void on_btnDelMrp_clicked();

    void on_btnChgMrp_clicked();

private:
    Ui::Dialog_mrp *ui;
    QSqlDatabase db;
    QSqlRelationalTableModel *mrp;
    int id_curr;
    bool ok;
};

#endif // DIALOG_MRP_H
