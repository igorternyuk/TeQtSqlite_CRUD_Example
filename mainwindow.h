#pragma once

#include <QMainWindow>
#include <QStringList>

//Редактирование всей записи по двойному щелчку по таблице заполняются поля ввода
//А через правую кнопку редактируются отдельные колонки через ипут диалог
//Можно использовать действия с картинками

namespace Ui
{
    class MainWindow;
}

class DAO;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInsert_clicked();
    void on_btnSearch_clicked();
    void on_btnLoadCompleteList_clicked();
    void on_btnUpdate_clicked();
    void on_action_edit_register_triggered();
    void on_action_remove_register_triggered();
    void on_action_load_full_list_triggered();
    void on_btnSort_clicked();
    void on_btnClearForm_clicked();
    void on_btnDelete_clicked();
    void on_checkBoxDecOrder_clicked();
    void on_tableView_doubleClicked(const QModelIndex &);

private:
    Ui::MainWindow *ui;
    DAO *mPersonDAO;
    const QString mWindowTitle { "TeQtGRUDExample" };
    const QString mPathToDBFile { "personas.db" };
    const QStringList mComboItems { "ID", "Name", "Age", "Salary" };
};

