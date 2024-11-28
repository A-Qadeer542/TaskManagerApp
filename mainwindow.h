#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAddTask();
    void handleEditTask();
    void handleDeleteTask();
    void handleSaveTasks();
    void handleLoadTasks();
    void handleClearAllTasks();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
