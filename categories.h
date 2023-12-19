#ifndef CATEGORIES_H
#define CATEGORIES_H
#include <QSqlTableModel>
#include <QMainWindow>

namespace Ui {
class categories;
}

class categories : public QMainWindow
{
    Q_OBJECT

public:
    explicit categories(QWidget *parent = nullptr);
    ~categories();
    void closeCategories();
    void action1();
    void addCategories();
    void updateCategoires();
    void removeCategories();
    void onBeforeUpdate(int row, QSqlRecord &record);

private:
    Ui::categories *ui;
    bool dbOpenSuccess;
    QSqlTableModel *model;
    void refreshTable();

};


#endif // CATEGORIES_H
