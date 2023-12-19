#ifndef LAWYERS_H
#define LAWYERS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Lawyers; }
QT_END_NAMESPACE

class Lawyers : public QMainWindow
{
    Q_OBJECT

public:
    Lawyers(QWidget *parent = nullptr);
    ~Lawyers();
    void openNewWindow();
    void openNewWindow_2();
    void openNewWindow_3();
    void closeWindow();
private:
    Ui::Lawyers *ui;
};
#endif // LAWYERS_H
