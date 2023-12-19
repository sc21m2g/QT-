#include "addcategories.h"
#include "ui_addcategories.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include<QSqlError>
#include <_mingw_mac.h>


AddCategories::AddCategories(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddCategories),dbOpenSuccess(false)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\QTwork\\ui_Lawyers\\categories.db");
    qDebug() << QSqlDatabase::drivers();

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return;
    }else
    {
        dbOpenSuccess=true;
        QSqlQuery query;
        // query.prepare("INSERT INTO main.CaseClassification (Criminal, Civil, Economical) VALUES (:val1, :val2, :val3)");
        // query.bindValue(":val1",1);
        // query.bindValue(":val2",2);
        // query.bindValue(":val3",3);

        // if (!query.exec()) {
        //     qDebug() << "插入数据失败:" << query.lastError().text();
        // } else {
        //     qDebug() << "插入数据成功";
        // }

        query.prepare("UPDATE main.CaseClassification SET Criminal = :val4, Civil = :val5, Economical=:val6 WHERE rowid = 1");
        query.bindValue(":val4",1);
        query.bindValue(":val5",2);
        query.bindValue(":val6",3);

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



}

AddCategories::~AddCategories()
{
    delete ui;
}
