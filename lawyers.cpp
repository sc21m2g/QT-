#include "lawyers.h"
#include "ui_lawyers.h"
#include"categories.h"
#include"customers.h"
#include"courts.h"
#include<QDebug>

Lawyers::Lawyers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lawyers)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&Lawyers::openNewWindow);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&Lawyers::openNewWindow_2);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&Lawyers::openNewWindow_3);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&Lawyers::closeWindow);
}

void Lawyers::openNewWindow()
{
    categories *window=new categories(this);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void Lawyers::openNewWindow_2()
{
    customers *window_2=new customers(this);
    window_2->setAttribute(Qt::WA_DeleteOnClose);
    window_2->show();
}

void Lawyers::openNewWindow_3()
{
    courts *window_3=new courts(this);
    window_3->setAttribute(Qt::WA_DeleteOnClose);
    window_3->show();
}

void Lawyers::closeWindow()
{
    this->close();
}
Lawyers::~Lawyers()
{
    qDebug()<<"已退出系统";
    delete ui;
}

