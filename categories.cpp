#include "categories.h"
#include "ui_categories.h"
#include<QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QSqlError>
#include <_mingw_mac.h>
#include<QMessageBox>
#include<QSqlRecord>



categories::categories(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::categories),dbOpenSuccess(false)
{
    ui->setupUi(this);
    connect(ui->C4,&QPushButton::clicked,this,&categories::closeCategories);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\QTwork\\ui_Lawyers\\categories.db");
    qDebug() << QSqlDatabase::drivers();

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
    return;
    }else
    {
        dbOpenSuccess=true;
        connect(ui->C1,&QPushButton::clicked,this,&categories::addCategories);
        connect(ui->C2,&QPushButton::clicked,this,&categories::removeCategories);
        connect(ui->C3,&QPushButton::clicked,this,&categories::updateCategoires);
        connect(ui->C5, &QPushButton::clicked, this, &categories::refreshTable);

    }
    model = new QSqlTableModel(this, db);// 初始化模型
    connect(model, &QSqlTableModel::beforeUpdate, this, &categories::onBeforeUpdate);
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // 设置编辑策略
    model->setTable("CaseClassification"); // 设置所使用的表名
    model->select(); // 查询表中的数据

    ui->tableView_1->setModel(model); // 将模型与视图关联
    ui->tableView_1->show(); // 显示表视图

}

void categories::closeCategories()
{
    this->close();
}

// void categories::action1()
// {
//     AddCategories *a1=new AddCategories(this);
//     a1->setAttribute(Qt::WA_DeleteOnClose);
//     a1->show();
// }

void categories::addCategories()
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.CaseClassification WHERE YourName = ?");
    QString nameValue = ui->lineEdit_name->text();
    checkQuery.addBindValue(nameValue);
    checkQuery.exec();

    if(checkQuery.next()) {
        qDebug() << "委托人已经存在，添加失败";
    }
    else{
        QSqlQuery query;
        query.prepare("INSERT INTO main.CaseClassification (Cases, YourName, Prices) VALUES (:val1, :val2, :val3)");
        QString caseValue = ui->lineEdit_case->text();
        QString nameValue = ui->lineEdit_name->text();
        int priceValue = ui->lineEdit_price->text().toDouble();

        query.bindValue(":val1",caseValue);
        query.bindValue(":val2",nameValue);
        query.bindValue(":val3",priceValue);

        if (!query.exec()) {
            qDebug() << "插入数据失败:" << query.lastError().text();
        } else {
            qDebug() << "插入数据成功";
        }
    }
    model->select();

}

void categories::updateCategoires()
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.CaseClassification WHERE YourName = ?");
    QString nameValue = ui->lineEdit_name->text();
    checkQuery.addBindValue(nameValue);
    checkQuery.exec();

    if(checkQuery.next()) {
        qDebug() << "委托人已经存在，更新失败";
    }
    else{
        QSqlQuery query;

        query.prepare("UPDATE main.CaseClassification SET Cases = :val4, YourName = :val5, Prices=:val6 WHERE rowid = :val7");
        QString caseValue = ui->lineEdit_case->text();
        QString nameValue = ui->lineEdit_name->text();
        int priceValue = ui->lineEdit_price->text().toInt();
        int lineValue=ui->lineEdit_line->text().toInt();
        query.bindValue(":val4",caseValue);
        query.bindValue(":val5",nameValue);
        query.bindValue(":val6",priceValue);
        query.bindValue(":val7",lineValue);

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

void categories::removeCategories()
{
    QSqlQuery query;
    query.prepare("DELETE FROM main.CaseClassification WHERE rowid = :val8");
    int lineValue=ui->lineEdit_line->text().toInt();
    query.bindValue(":val8",lineValue);
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

void categories::onBeforeUpdate(int row, QSqlRecord &record) {
    // 验证要更新的数据是否与其他列重复，这里我们主要关注 YourName 列
    QString newYourName = record.value("YourName").toString();
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM main.CaseClassification WHERE YourName = ?");
    checkQuery.addBindValue(newYourName);
    checkQuery.exec();

    // 如果存在重复的数据，修改 record 使其更新失败并显示警告
    if (checkQuery.next()) {
        QMessageBox::warning(this, "更新失败", "记录重复，不能更新为相同的 YourName.");
        // 修改 record 的值以取消更新操作
        QSqlRecord originalRecord = model->record(row);
        record.setGenerated("YourName", false); // 不更新 YourName 列，取消操作
    }
}
void categories::refreshTable() {
    model->select();
}


categories::~categories()
{
    qDebug()<<"已退出事务模块";
    delete model; // 清理模型资源
    delete ui;
}

