#include "courts.h"
#include "ui_courts.h"
#include<QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QSqlError>
#include <_mingw_mac.h>
#include<QMessageBox>
#include<QSqlRecord>

courts::courts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::courts),dbOpenSuccess(false)
{
    ui->setupUi(this);
    connect(ui->T4,&QPushButton::clicked,this,&courts::closeCourts);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\QTwork\\ui_Lawyers\\courts.db");
    qDebug() << QSqlDatabase::drivers();
    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
    return;
    }else
    {
        dbOpenSuccess=true;
        connect(ui->T1,&QPushButton::clicked,this,&courts::addCourts);
        connect(ui->T2,&QPushButton::clicked,this,&courts::updateCourts);
        connect(ui->T3,&QPushButton::clicked,this,&courts::removeCourts);
        connect(ui->T5,&QPushButton::clicked,this,&courts::refreshTable);
    }
    model = new QSqlTableModel(this, db); // 初始化模型
    connect(model,&QSqlTableModel::beforeUpdate,this,&courts::onBeforeUpdate);
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // 设置编辑策略
    model->setTable("Courts"); // 设置所使用的表名
    model->select(); // 查询表中的数据

    ui->tableView_3->setModel(model); // 将模型与视图关联
    ui->tableView_3->show(); // 显示表视图
}

void courts::closeCourts()
{
    this->close();

}

void courts::addCourts()
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.Courts WHERE Name = ?");
    QString nameValue = ui->lineEdit_name->text();
    checkQuery.addBindValue(nameValue);
    checkQuery.exec();

    if(checkQuery.next()) {
        qDebug() << "委托人已经存在，添加失败";
    }
    else{
        QSqlQuery query;
        query.prepare("INSERT INTO main.Courts (Name, LawyerName, Cases, Time) VALUES (:val1, :val2, :val3,:val4)");
        QString nameValue=ui->lineEdit_name->text();
        QString lawyerValue=ui->lineEdit_lawyer->text();
        QString caseValue=ui->lineEdit_case->text();
        QString timeValue = ui->lineEdit_time->text();
        query.bindValue(":val1",nameValue);
        query.bindValue(":val2",lawyerValue);
        query.bindValue(":val3",caseValue);
        query.bindValue(":val4",timeValue);

        if (!query.exec()) {
            qDebug() << "插入数据失败:" << query.lastError().text();
        } else {
            qDebug() << "插入数据成功";
        }
    }
    model->select();

}

void courts::updateCourts()
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.Courts WHERE Name = ?");
    QString nameValue = ui->lineEdit_name->text();
    checkQuery.addBindValue(nameValue);
    checkQuery.exec();

    if(checkQuery.next()) {
        qDebug() << "委托人已经存在，更新失败";
    }
    else{
        QSqlQuery query;
        query.prepare("UPDATE main.Courts SET Name = :val5, LawyerName = :val6, Cases=:val7, Time=:val8 WHERE rowid = :val");
        QString nameValue=ui->lineEdit_name->text();
        QString lawyerValue=ui->lineEdit_lawyer->text();
        QString caseValue=ui->lineEdit_case->text();
        QString timeValue = ui->lineEdit_time->text();
        int lineValue=ui->lineEdit_line->text().toInt();
        query.bindValue(":val5",nameValue);
        query.bindValue(":val6",lawyerValue);
        query.bindValue(":val7",caseValue);
        query.bindValue(":val8",timeValue);
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

void courts::removeCourts()
{
    QSqlQuery query;
    query.prepare("DELETE FROM main.Courts WHERE rowid = :val");
    int lineValue=ui->lineEdit_line->text().toInt();
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

void courts::onBeforeUpdate(int row,QSqlRecord &record)
{
    QString newName=record.value("Name").toString();
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.Courts WHERE Name=?");
    checkQuery.addBindValue(newName);
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "更新失败", "记录重复，不能更新为相同的 Name.");
        // 修改 record 的值以取消更新操作
        QSqlRecord originalRecord = model->record(row);
        record.setGenerated("Name", false); // 不更新 Name 列，取消操作
    }
}
void courts::refreshTable() {
    model->select();
}

courts::~courts()
{
    qDebug()<<"已退出庭审模块";
    delete model;
    delete ui;
}
