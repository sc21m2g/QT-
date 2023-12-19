#include "customers.h"
#include "ui_customers.h"
#include<QDebug>
#include<QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QSqlError>
#include <_mingw_mac.h>
#include<QSqlRecord>
#include<QMessageBox>

customers::customers(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::customers),dbOpenSuccess(false)
{
    ui->setupUi(this);
    connect(ui->U4,&QPushButton::clicked,this,&customers::closeCustomers);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\QTwork\\ui_Lawyers\\customers.db");
    qDebug() << QSqlDatabase::drivers();

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
         return;
    }else
    {
        dbOpenSuccess=true;
        connect(ui->U1,&QPushButton::clicked,this,&customers::addCustomers);
        connect(ui->U2,&QPushButton::clicked,this,&customers::updateCustomers);
        connect(ui->U3,&QPushButton::clicked,this,&customers::findCustomers);
        connect(ui->U5,&QPushButton::clicked,this,&customers::removeCustomers);
        connect(ui->U6,&QPushButton::clicked,this,&customers::refreshTable);

    }
    model = new QSqlTableModel(this, db); // 初始化模型
    connect(model,&QSqlTableModel::beforeUpdate,this,&customers::onBeforeUpdate);
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // 设置编辑策略
    model->setTable("Customers"); // 设置所使用的表名
    model->select(); // 查询表中的数据

    ui->tableView_2->setModel(model); // 将模型与视图关联
    ui->tableView_2->show(); // 显示表视图

}

void customers::closeCustomers()
{
    this->close();
}

void customers::addCustomers()
{
    QSqlQuery checkCustomer;
    checkCustomer.prepare("SELECT * FROM main.Customers WHERE ID = ?");
    QString idValue= ui->lineEdit_id->text();
    checkCustomer.addBindValue(idValue);
    checkCustomer.exec();
    if(checkCustomer.next())
    {
        qDebug()<<"证件号重复输入，添加失败";
    }

    else{
        QSqlQuery query;
        query.prepare("INSERT INTO main.Customers (ID, Name, PhoneNumber,City) VALUES (:val1, :val2, :val3,:val4)");
        int idValue = ui->lineEdit_id->text().toInt();
        QString nameValue=ui->lineEdit_name->text();
        int telValue = ui->lineEdit_tel->text().toInt();
        QString cityValue=ui->lineEdit_address->text();
        query.bindValue(":val1",idValue);
        query.bindValue(":val2",nameValue);
        query.bindValue(":val3",telValue);
        query.bindValue(":val4",cityValue);

        if (!query.exec()) {
            qDebug() << "插入数据失败:" << query.lastError().text();
        } else {
            qDebug() << "插入数据成功";
        }
    }

    model->select();
}
void customers::updateCustomers()
{
    QSqlQuery checkCustomer;
    checkCustomer.prepare("SELECT * FROM main.Customers WHERE ID = ?");
    QString idValue= ui->lineEdit_id->text();
    checkCustomer.addBindValue(idValue);
    checkCustomer.exec();
    if(checkCustomer.next())
    {
        qDebug()<<"证件号重复输入，添加失败";
    }
    else{
        QSqlQuery query;
        query.prepare("UPDATE main.Customers SET ID = :val5, Name = :val6, PhoneNumber=:val7, City=:val8 WHERE rowid = :val");
        int idValue = ui->lineEdit_id->text().toInt();
        QString nameValue=ui->lineEdit_name->text();
        int telValue = ui->lineEdit_tel->text().toInt();
        int lineValue =ui->lineEdit_line->text().toInt();
        QString cityValue=ui->lineEdit_address->text();
        query.bindValue(":val5",idValue);
        query.bindValue(":val6",nameValue);
        query.bindValue(":val7",telValue);
        query.bindValue(":val8",cityValue);
        query.bindValue(":val",lineValue);

        if (!query.exec())
        {
            qDebug() << "更新数据失败:" << query.lastError();
        }
        else
        {
            if (query.numRowsAffected() > 0)
            {
                qDebug() << "更新数据成功。";
            }
            else
            {
                qDebug() << "没有找到对应的记录进行更新。";
            }
        }
    }

    model->select();
}

void customers::findCustomers()
{
    QSqlQuery query;
    query.prepare("SELECT ID,Name,PhoneNumber,City FROM main.Customers WHERE rowid = :val");
    int lineValue=ui->lineEdit_line->text().toInt();
    query.bindValue(":val",lineValue);
    if (!query.exec()) {
        qDebug() << "查询数据失败:" << query.lastError().text();
        return;
    }

    while(query.next())
    {
        int id=query.value(0).toInt();
        QString name = query.value(1).toString();
        long int telnum=query.value(2).toInt();
        QString address=query.value(3).toString();

        qDebug()<<id<<""<<name<<""<<telnum<<""<<address;

    }
}

void customers::removeCustomers()
{
    QSqlQuery query;
    query.prepare("DELETE FROM main.Customers WHERE rowid = :val");
    int lineValue =ui->lineEdit_line->text().toInt();
    query.bindValue(":val",lineValue);
    if (!query.exec()) {
        qDebug() << "删除数据失败:" << query.lastError().text();
    } else {
        if (query.numRowsAffected() > 0) {
            qDebug() << "删除数据成功。";
        } else {
            qDebug() << "没有找到对应记录进行删除。";
        }
    }
    model->select();
}
void customers::onBeforeUpdate(int row,QSqlRecord &record)
{
    QString newID= record.value("ID").toString();
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.Customers WHERE ID = ?");
    checkQuery.addBindValue(newID);
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "更新失败", "记录重复，不能更新为相同的 ID.");
        // 修改 record 的值以取消更新操作
        QSqlRecord originalRecord = model->record(row);
        record.setGenerated("ID", false); // 不更新 ID列，取消操作
    }
}

void customers::refreshTable()
{
    model->select();
}

customers::~customers()
{
    qDebug()<<"已退出用户模块";
    delete model;
    delete ui;
}
