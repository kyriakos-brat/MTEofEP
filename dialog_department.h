#ifndef DIALOG_DEPARTMENT_H
#define DIALOG_DEPARTMENT_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class Dialog_department;
}

class Dialog_department : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_department(QWidget *parent = nullptr);
    ~Dialog_department();

private slots:
    void on_btnAddDept_clicked();

    void on_tableView_department_clicked(const QModelIndex &index);

    void on_btnDelDept_clicked();

    void on_btnChgDept_clicked();

private:
    Ui::Dialog_department *ui;
    QSqlDatabase db;
    QSqlTableModel *department;
    int id_curr;
    bool ok;
};

#endif // DIALOG_DEPARTMENT_H
