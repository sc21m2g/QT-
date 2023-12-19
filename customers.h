#ifndef CUSTOMERS_H
#define CUSTOMERS_H
#include<QSqlTableModel>
#include <QMainWindow>

namespace Ui {
class customers;
}

class customers : public QMainWindow
{
    Q_OBJECT

public:
    explicit customers(QWidget *parent = nullptr);
    ~customers();
    void closeCustomers();
    void addCustomers();
    void updateCustomers();
    void findCustomers();
    void removeCustomers();
    void onBeforeUpdate(int row, QSqlRecord &record);

private:
    Ui::customers *ui;
    bool dbOpenSuccess;
    QSqlTableModel *model;
    void refreshTable();
};

#endif // CUSTOMERS_H
