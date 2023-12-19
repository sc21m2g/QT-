#ifndef COURTS_H
#define COURTS_H
#include <QSqlTableModel>
#include <QMainWindow>

namespace Ui {
class courts;
}

class courts : public QMainWindow
{
    Q_OBJECT

public:
    explicit courts(QWidget *parent = nullptr);
    ~courts();
    void closeCourts();
    void addCourts();
    void updateCourts();
    void removeCourts();
    void onBeforeUpdate(int row, QSqlRecord &record);

private:
    Ui::courts *ui;
    bool dbOpenSuccess;
    QSqlTableModel *model;
    void refreshTable();
};

#endif // COURTS_H
