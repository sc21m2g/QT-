#ifndef ADDCATEGORIES_H
#define ADDCATEGORIES_H

#include <QMainWindow>

namespace Ui {
class AddCategories;
}

class AddCategories : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddCategories(QWidget *parent = nullptr);
    ~AddCategories();
    bool isDbOpen() const { return dbOpenSuccess;}

private:
    Ui::AddCategories *ui;
    bool dbOpenSuccess;
};

#endif // ADDCATEGORIES_H
