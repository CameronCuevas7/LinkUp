#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <cstdlib>
#include <QtCore>
#include <QListWidgetItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!connOpen()) //check if connected to database
    {
        ui->label->setText("NOT CONNECTED"); //for label at the top to debug
    }
    else
        ui->label->setText("CONNECTED");
}

MainWindow::~MainWindow()
{
    delete ui;
}


//LOGIN BUTTON///////
void MainWindow::on_pushButton_Login_clicked()
{
    QString username, password;
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;
    qry.prepare("select * from information where username='"+username+"' and password='"+password+"'");//getting info from username and pass

    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if(count == 1){
            ui->label_2->setText("Username and Password is correct"); //debug purposes
            connClose(); //close connection after login complete
            ui->stackedWidget->setCurrentIndex(1); //go to next page (aka the home page)
            currentUser = username;            
            qDebug() << username;

            QDate dateSelected = ui->calendarWidget->selectedDate(); //gets date selected and makes it a string
            QDate dateSelected2 = ui->calendarWidget_all->selectedDate();
            QDate dateSelected3 = ui->comparecal->selectedDate();
            //MIGHT CHANGE
            populateList(dateSelected);
            populateList2(dateSelected2);
            populateList3(dateSelected3);
        }
        if(count > 1)
            ui->label_2->setText("Duplicate Username and Password");
        if(count < 1)
            ui->label_2->setText("Username and Password NOT correct");
    }
}

//////////////REGISTER BUTTON CLICK////////
void MainWindow::on_pushButton_Register_clicked()
{
    QString firstname, lastname, username, password,sharecode;

    firstname = ui->lineEdit_FN->text();
    lastname = ui->lineEdit_LN->text();
    username = ui->lineEdit_usernameR->text();
    password = ui->lineEdit_passwordR->text();
    sharecode = ui->sharecode_LE->text();

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    if(firstname == ""|| lastname == ""|| username == ""|| password == ""|| sharecode == "")
    {
         QMessageBox::critical(this,tr("NOT SAVE"),tr("Can not leave any fields empty")); //writes a message
         return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("insert into information (firstname,lastname,username,password,sharecode) values ('"+firstname+"','"+lastname+"','"+username+"','"+password+"','"+sharecode+"')");
    if(qry.exec())
    {
        QMessageBox::critical(this,tr("SAVE"),tr("Saved")); //writes a message
        connClose(); //close connection after register complete
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry.lastError().text()); //shows the error
    }
}

////////////////////////////////////////////////////////////////
//schedule button clicked
void MainWindow::on_go_schedule_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//compare button clicked
void MainWindow::on_go_comp_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//set event button clicked                      //buttons to navigate
void MainWindow::on_go_setEvent_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

//memo button clicked
void MainWindow::on_go_memo_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

//edit profile button clicked
void MainWindow::on_go_editProfile_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

///////////////////////////////////////////////////////////////
void MainWindow::on_back1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back2_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back3_clicked()                      //back buttons
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back4_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back5_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

//////////////////////////////////////////////////////
///CALANDER
void MainWindow::on_calendarWidget_selectionChanged()
{
    QDate dateSelected = ui->calendarWidget->selectedDate(); //gets date selected and makes it a string
    qDebug() << dateSelected;
    ui->listWidget->clear(); // clears on changing date
    populateList(dateSelected);
}

//////////////////////////////////////////////////////
///ADD NEW TASK
void MainWindow::on_addNew_button_clicked()
{
    QDate dateSelected = ui->calendarWidget->selectedDate();//grabs selected date
    QString date = dateSelected.toString(); //date but as a string
    QString newTask = ui->newtaskLine->text();//task that is in the line edit
    QString completeStatus = "NO"; //assumes added task is not complete

    if(newTask == "")
    {
        return;
    }

    QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
    item->setText(newTask); //same as newTask but for listbox item

    item->setCheckState(Qt::Unchecked); // gives checkbox (MIGHT CHANGE)  ///////

    ui->listWidget->addItem(item); //adds to listbox

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("insert into userPlannerData (user, task, completed, date) values ('"+currentUser+"','"+newTask+"','"+completeStatus+"','"+date+"')");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
        ui->newtaskLine->clear();
        connClose(); //close connection after register complete
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry.lastError().text()); //shows the error
    }
}

//////////////////////////////////////////////
//MADE TO AUTO POPULATE A LIST ITEM
void MainWindow::populateList(QDate date)
{
    QString stringDate = date.toString();
    QString task, completed;

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;

    //qry.prepare("select * from userPlannerDate");
    //qry.prepare("SELECT task, completed FROM plannerData1 WHERE date = ?");
    qry.exec("SELECT task, completed FROM userPlannerData WHERE user = '"+currentUser+"' and date ='"+stringDate+"'");
    while(qry.next())
    {
        task = qry.value (0).toString();
        completed = qry.value (1).toString();

        QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
        item->setText(task); //same as newTask but for listbox item

        if(completed == "NO"){
            item->setCheckState(Qt::Unchecked); // gives checkbox (MIGHT CHANGE)  ///////
        }
        else{
            item->setCheckState(Qt::Checked);
        }
        ui->listWidget->addItem(item); //adds to listbox

        qDebug() << task << completed << stringDate;
    }
    connClose();
}



///PUSHES NEW CHECKS TO DATABASE for personal data
void MainWindow::on_saveChanges_button_clicked()
{
    QDate date = ui->calendarWidget->selectedDate();
    QString stringDate = date.toString();

    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(item->checkState() == Qt::CheckState(Qt::Checked))
        {
            connOpen(); //open connection
            QString completed = "YES";
            QString task = ui->listWidget->item(i)->text();
            QSqlQuery qry;
            qry.prepare("update userPlannerData set completed ='"+completed+"' where task ='"+task+"'");

            qry.exec();
            connClose(); //close connection after register complete
        }
        else if(item->checkState() == Qt::CheckState(Qt::Unchecked))
        {
            connOpen(); //open connection
            QString completed = "NO";
            QString task = ui->listWidget->item(i)->text();
            QSqlQuery qry2;
            qry2.prepare("update userPlannerData set completed ='"+completed+"' where task ='"+task+"'");

            qry2.exec();
            connClose(); //close connection after register complete
        }
    }

    QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
}


/////////////////////////////////////////////
/// MainWindow::on_delete_persona_clicked
///DELETES EVENTS
void MainWindow::on_delete_persona_clicked()
{

    QDate date = ui->calendarWidget->selectedDate();
    QString stringDate = date.toString();


    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(item->checkState() == Qt::CheckState(Qt::Checked))
        {
            connOpen(); //open connection
            QString completed = "YES";
            QString task = ui->listWidget->item(i)->text();

            QSqlQuery qry2;
            qry2.prepare("update userPlannerData set completed ='"+completed+"' where task ='"+task+"'");
            qry2.exec();

            QSqlQuery qry;
            qry.prepare("delete from userPlannerData where completed = '"+completed+"' and task = '"+task+"'");
            qry.exec();

            connClose(); //close connection after register complete
        }
        else if(item->checkState() == Qt::CheckState(Qt::Unchecked))
        {
            connOpen(); //open connection
            QString completed = "NO";
            QString task = ui->listWidget->item(i)->text();
            QSqlQuery qry2;
            qry2.prepare("update userPlannerData set completed ='"+completed+"' where task ='"+task+"'");

            qry2.exec();
            connClose(); //close connection after register complete
        }
    }
    QMessageBox::information(this,tr("DELETED"),tr("Event was Deleted")); //writes a message
    ui->listWidget->clear();
    populateList(date);

}


///////////////////////////////////////////////////////////////////////////////FOR UNIVERSAL CALANDER
///MADE TO AUTO POPULATE A LIST ITEM on 2nd calander
void MainWindow::populateList2(QDate date)
{

    QString stringDate = date.toString();
    QString task, completed;

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;

    //qry.prepare("select * from userPlannerDate");
    //qry.prepare("SELECT task, completed FROM plannerData1 WHERE date = ?");
    qry.exec("SELECT task, completed FROM plannerData1 WHERE date ='"+stringDate+"'");
    while(qry.next())
    {
        task = qry.value (0).toString();
        completed = qry.value (1).toString();

        QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
        item->setText(task); //same as newTask but for listbox item

        if(completed == "NO"){
            item->setCheckState(Qt::Unchecked); // gives checkbox (MIGHT CHANGE)  ///////
        }
        else{
            item->setCheckState(Qt::Checked);
        }
        ui->listWidget_all->addItem(item); //adds to listbox

        qDebug() << task << completed << stringDate;
    }
    connClose();
}

//////////////////////////////////////////////
/// for 2nd calander
void MainWindow::on_pushButton_alladd_clicked()
{
    QDate dateSelected = ui->calendarWidget_all->selectedDate();//grabs selected date
    QString date = dateSelected.toString(); //date but as a string
    QString newTask = ui->lineEdit_all->text();//task that is in the line edit
    QString completeStatus = "NO"; //assumes added task is not complete

    if(newTask == "")
    {
        return;
    }

    QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
    item->setText(newTask); //same as newTask but for listbox item

    item->setCheckState(Qt::Unchecked); // gives checkbox (MIGHT CHANGE)  ///////

    ui->listWidget_all->addItem(item); //adds to listbox

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry2;
    qry2.prepare("insert into plannerData1 (task, completed, date) values ('"+newTask+"','"+completeStatus+"','"+date+"')");

    if(qry2.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
        ui->lineEdit_all->clear();
        connClose(); //close connection after register complete
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry2.lastError().text()); //shows the error
    }
    connClose();
}

///////////////////////////////////////////////////
/// for second calander
///
void MainWindow::on_calendarWidget_all_selectionChanged()
{

        QDate dateSelected = ui->calendarWidget_all->selectedDate(); //gets date selected and makes it a string
        qDebug() << dateSelected;
        ui->listWidget_all->clear(); // clears on changing date
        populateList2(dateSelected);
}

/////////////////////////////////////////////////////

//void updateCheck(QDate date)
//{
//}

/////////////////////////////////////////////////////
///PUSHES NEW CHECKS TO DATABASE for universal tasks
void MainWindow::on_pushButton_saveall_clicked()
{
    QDate date = ui->calendarWidget_all->selectedDate();
    QString stringDate = date.toString();

    for(int i = 0; i < ui->listWidget_all->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget_all->item(i);
        if(item->checkState() == Qt::CheckState(Qt::Checked))
        {
            connOpen(); //open connection
            QString completed = "YES";
            QString task = ui->listWidget_all->item(i)->text();
            QSqlQuery qry;
            qry.prepare("update plannerData1 set completed ='"+completed+"' where task ='"+task+"'");

            qry.exec();
            connClose(); //close connection after register complete
        }
        else if(item->checkState() == Qt::CheckState(Qt::Unchecked))
        {
            connOpen(); //open connection
            QString completed = "NO";
            QString task = ui->listWidget_all->item(i)->text();
            QSqlQuery qry2;
            qry2.prepare("update plannerData1 set completed ='"+completed+"' where task ='"+task+"'");

            qry2.exec();
            connClose(); //close connection after register complete
        }
    }

    QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
}







//////////////////////////////////////////////////////
/// COMPARE
/// listWidget_you, listWidget_other
/// comparecal, comp_user, comp_code, enter_button
///

void MainWindow::on_enter_button_clicked()
{

    QString username, code;
    username = ui->comp_user->text();
    code = ui->comp_code->text();

    if(username == currentUser)
    {
        QMessageBox::critical(this,tr("error::"),"CAN NOT COMPARE WITH YOURSELF"); //shows the error
        return;
    }

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;
    qry.prepare("select * from information where username = '"+username+"' and sharecode ='"+code+"'");//getting info from username and pass

    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if(count == 1){
            otherUser = username;
            connClose(); //close connection after login complete

            QMessageBox::information(this,tr("error::"),"THANK YOU, YOU CAN NOW COMPARE"); //shows the error
            ui->status->setText("COMPARE"); //debug purposes
            ui->other_label->setText(otherUser); //debug purposes

            QDate dateSelected4 = ui->comparecal->selectedDate();
            populateList4(dateSelected4);
        }
        if(count < 1)
            ui->status->setText("Username/Code is incorrect");
    }
    ui->listWidget_other->clear();
}
///////////////////////////////////////////////
//// current users CALANDER FOR COMPARE
///
void MainWindow::populateList3(QDate date)
{
    QString stringDate = date.toString();
    QString task, completed;

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;

    //qry.prepare("select * from userPlannerDate");
    //qry.prepare("SELECT task, completed FROM plannerData1 WHERE date = ?");
    qry.exec("SELECT task FROM userPlannerData WHERE user = '"+currentUser+"' and date ='"+stringDate+"'");
    while(qry.next())
    {
        task = qry.value (0).toString();
        QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
        item->setText(task); //same as newTask but for listbox item
        ui->listWidget_you->addItem(item); //adds to listbox
    }
    connClose();
}

///////////////////////////////////////////////
/// OTHER USERS CALANDER FOR COMPARE
///

void MainWindow::populateList4(QDate date)
{

    if(otherUser == "")
    {
        ui->status->setText("PLEASE ENTER ANOTHER USER"); //debug purposes
        return;
    }

    QString stringDate = date.toString();
    QString task, completed;

    if(!connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    connOpen(); //open connection
    QSqlQuery qry;

    //qry.prepare("select * from userPlannerDate");
    //qry.prepare("SELECT task, completed FROM plannerData1 WHERE date = ?");
    qry.exec("SELECT task FROM userPlannerData WHERE user = '"+otherUser+"' and date ='"+stringDate+"'");
    while(qry.next())
    {
        task = qry.value (0).toString();
        QListWidgetItem *item = new QListWidgetItem; //makes an item to appear in listbox
        item->setText(task); //same as newTask but for listbox item
        ui->listWidget_other->addItem(item); //adds to listbox
    }
    connClose();
}

/////////////////////////////////////

void MainWindow::on_comparecal_selectionChanged()
{
    QDate dateSelected = ui->comparecal->selectedDate(); //gets date selected and makes it a string
    qDebug() << dateSelected;

    ui->listWidget_you->clear(); // clears on changing date
    ui->listWidget_other->clear(); // clears on changing date

    populateList3(dateSelected);
    populateList4(dateSelected);
}



///////////////////////////////////////////////////////////////////////
/// CHANGE INFO
///

void MainWindow::on_SAVE_CHANGES_clicked()
{
    QString newUsername = ui->change_UN->text();

    if(newUsername == "")
    {
        QMessageBox::information(this,tr("NOT SAVED"),tr("CAN NOT LEAVE EMPTY")); //writes a message
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("update information set username ='"+newUsername+"' where username ='"+currentUser+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),"USERNAME not Unique"); //shows the error
        return;
    }

    QSqlQuery qry2;
    qry2.prepare("update userPlannerData set user ='"+newUsername+"' where user ='"+currentUser+"'");
    currentUser = newUsername;
    qry2.exec();
    connClose(); //close connection after register complete
}


void MainWindow::on_SAVE_CHANGES_2_clicked()
{
    QString newLN = ui->change_LN->text();

    if(newLN == "")
    {
        QMessageBox::information(this,tr("NOT SAVED"),tr("CAN NOT LEAVE EMPTY")); //writes a message
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("update information set lastname ='"+newLN+"' where username ='"+currentUser+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
    }
    else
    {
        return;
    }
    connClose(); //close connection after register complete
}


void MainWindow::on_SAVE_CHANGES_3_clicked()
{
    QString newFN = ui->change_FN->text();

    if(newFN == "")
    {
        QMessageBox::information(this,tr("NOT SAVED"),tr("CAN NOT LEAVE EMPTY")); //writes a message
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("update information set firstname ='"+newFN+"' where username ='"+currentUser+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
    }
    else
    {
        return;
    }
    connClose(); //close connection after register complete
}


void MainWindow::on_SAVE_CHANGES_4_clicked()
{
    QString newPass = ui->change_pass->text();

    if(newPass == "")
    {
        QMessageBox::information(this,tr("NOT SAVED"),tr("CAN NOT LEAVE EMPTY")); //writes a message
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("update information set password ='"+newPass+"' where username ='"+currentUser+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
    }
    else
    {
        return;
    }
    connClose(); //close connection after register complete
}


void MainWindow::on_SAVE_CHANGES_5_clicked()
{
    QString newCode = ui->change_SC->text();

    if(newCode == "")
    {
        QMessageBox::information(this,tr("NOT SAVED"),tr("CAN NOT LEAVE EMPTY")); //writes a message
        return;
    }

    connOpen(); //open connection

    QSqlQuery qry;
    qry.prepare("update information set sharecode ='"+newCode+"' where username ='"+currentUser+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("SAVE"),tr("Saved")); //writes a message
    }
    else
    {
        return;
    }
    connClose(); //close connection after register complete
}


//////////////////////////////////////////////////////////////////
///
///ignore, bug does not allow me to delete safely
void MainWindow::on_pushButton_clicked()
{

    QDate date = ui->calendarWidget_all->selectedDate();
    QString stringDate = date.toString();


    for(int i = 0; i < ui->listWidget_all->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget_all->item(i);
        if(item->checkState() == Qt::CheckState(Qt::Checked))
        {
            connOpen(); //open connection
            QString completed = "YES";
            QString task = ui->listWidget_all->item(i)->text();

            QSqlQuery qry2;
            qry2.prepare("update plannerData1 set completed ='"+completed+"' where task ='"+task+"'");
            qry2.exec();

            QSqlQuery qry;
            qry.prepare("delete from plannerData1 where completed = '"+completed+"' and task = '"+task+"'");
            qry.exec();

            connClose(); //close connection after register complete
        }
        else if(item->checkState() == Qt::CheckState(Qt::Unchecked))
        {
            connOpen(); //open connection
            QString completed = "NO";
            QString task = ui->listWidget_all->item(i)->text();
            QSqlQuery qry2;
            qry2.prepare("update userPlannerData set completed ='"+completed+"' where task ='"+task+"'");

            qry2.exec();
            connClose(); //close connection after register complete
        }
    }
    QMessageBox::information(this,tr("DELETED"),tr("Event(s) was Deleted")); //writes a message
    ui->listWidget_all->clear();
    populateList2(date);

}

