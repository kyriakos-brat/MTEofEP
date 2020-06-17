#ifndef DIALOG_CLROOM_H
#define DIALOG_CLROOM_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class Dialog_clroom;
}

class Dialog_clroom : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_clroom(QWidget *parent = nullptr);
    ~Dialog_clroom();

private slots:

    void on_btnAdd_clicked();

    void on_tableView_classroom_clicked(const QModelIndex &index);

    void on_btnDel_clicked();

    void on_btnChg_clicked();

private:
    Ui::Dialog_clroom *ui;
    QSqlDatabase db;
    QSqlTableModel *classroom;
    int id_curr;
    bool ok;
};

#endif // DIALOG_CLROOM_H
