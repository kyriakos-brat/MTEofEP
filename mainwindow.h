#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();

    void on_btnClroom_clicked();

    void on_btnEquip_clicked();

    void on_btnDept_clicked();

    void on_btnMrp_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    bool isConnect;
};
#endif // MAINWINDOW_H
