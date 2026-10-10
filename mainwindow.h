#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>

#include<QtSql>
#include<QtDebug>
#include<QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

//added
public:
    QSqlDatabase mydb; //adding database
    QString currentUser;
    QString otherUser;

    void connClose()//closes connection
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen() // open connection
    {
        mydb = QSqlDatabase::addDatabase("QSQLITE");//adding database from sqlite



        /////////////////////////////////////////////////////////////////////////////////////
        // CHANGE THIS PATH TO PATH OF YOUR DATABASE FROM SQLITE USING DB BROWSER FOR SQLITE/

        mydb.setDatabaseName("B:/sqlitestuff/users1.db");

        /////////////////////////////////////////////////////////////////////////////////////




        if(!mydb.open()) //check if connected to database
        {
            qDebug() << ("NOT CONNECTED");
            return false;
        }
        else{
           qDebug() << ("CONNECTED");
            return true;
        }
    }

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Login_clicked();

    void on_go_schedule_clicked();

    void on_go_comp_clicked();

    void on_go_setEvent_clicked();

    void on_go_memo_clicked();

    void on_go_editProfile_clicked();

    void on_back1_clicked();

    void on_back2_clicked();

    void on_back3_clicked();

    void on_back4_clicked();

    void on_back5_clicked();

    void on_pushButton_Register_clicked();

    void on_calendarWidget_selectionChanged();

    void on_addNew_button_clicked();

    void populateList(QDate date); //made to auto populate planner list

    void populateList2(QDate date); //made to auto populate 2nd planner list

    void populateList3(QDate date); //made to auto populate 3nd planner lists

    void populateList4(QDate date); //made to auto populate 4nd planner lists of other user

    void on_saveChanges_button_clicked();

    //void updateCheck(QDate date);

    void on_pushButton_alladd_clicked();

    void on_calendarWidget_all_selectionChanged();

    void on_pushButton_saveall_clicked();

    void on_enter_button_clicked();

    void on_comparecal_selectionChanged();

    void on_SAVE_CHANGES_clicked();

    void on_SAVE_CHANGES_2_clicked();

    void on_SAVE_CHANGES_3_clicked();

    void on_SAVE_CHANGES_4_clicked();

    void on_SAVE_CHANGES_5_clicked();

    void on_delete_persona_clicked();

    void on_pushButton_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::MainWindow *ui; 
};
#endif // MAINWINDOW_H
