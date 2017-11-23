#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dao.h"
#include <QMessageBox>
#include <QInputDialog>
#ifdef DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(mWindowTitle);
    ui->cmbSort->addItems(mComboItems);
    ui->cmbSort->setCurrentIndex(0);
    try
    {
        mPersonDAO = new DAO(mPathToDBFile, this);
        ui->tableView->setModel(mPersonDAO->model());
    }
    catch(std::exception &ex)
    {
        QMessageBox::critical(this, "Database error", ex.what());
    }

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addAction(ui->action_edit_register);
    ui->tableView->addAction(ui->action_remove_register);
    ui->tableView->addAction(ui->action_load_full_list);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    auto signal = static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(ui->cmbSort, signal, [this](int index){
        mPersonDAO->sortBy(static_cast<DAO::Column>(index),
                           ui->checkBoxDecOrder->isChecked());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnInsert_clicked()
{
    Person p;
    p.setName(ui->txtName->text());
    p.setAge(ui->spinBoxAge->value());
    p.setSalary(ui->spinBoxSalary->value());
    if(mPersonDAO->insert(p))
    {
        QMessageBox::information(this, "Success", "New person was successfully"
                                                  " inserted to the database");
    }
    else
    {
        QMessageBox::information(this, "Failure", "Some error ocuurs while"
                                                  " inserting to the database");
    }
    mPersonDAO->selectAll();
}

void MainWindow::on_btnUpdate_clicked()
{
    auto getInt = [&](const QString &title, const QString &label,
                      int val, QWidget *parent = nullptr){
        bool ok;
        int res = QInputDialog::getInt(parent, title, label, val,
                                       -2147483647, 2147483647, 1, &ok);
        return ok ? res : val;
    };

    auto getDouble = [&](const QString &title, const QString &label,
                      double val, QWidget *parent = nullptr){
        bool ok;
        double res = QInputDialog::getDouble(parent, title, label, val,
                                       -2147483647, 2147483647, 3, &ok);
        return ok ? res : val;
    };

    auto getText = [&](const QString &title, const QString &label,
                       const QString &val, QWidget *parent = nullptr){
        bool ok;
        QString res = QInputDialog::getText(parent, title, label,
                                            QLineEdit::Normal, val, &ok);
        return ok ? res : val;
    };

    const int row = ui->tableView->currentIndex().row();

    Person p;
    p.setId(ui->tableView->model()->
            index(row, static_cast<int>(DAO::Column::ID)).data().toInt());
    p.setName(ui->tableView->model()->
              index(row, static_cast<int>(DAO::Column::NAME)).data().toString());
    p.setAge(ui->tableView->model()->
             index(row, static_cast<int>(DAO::Column::AGE)).data().toInt());
    p.setSalary(ui->tableView->model()->
                index(row, static_cast<int>(DAO::Column::SALARY)).data().toDouble());

    const int col = ui->tableView->currentIndex().column();
    bool isItNecessaryToUpdate { true };
    switch(col)
    {
    case static_cast<int>(DAO::Column::NAME):
        p.setName(getText("Enter new name", "Name:", p.name(), this));

        break;
    case static_cast<int>(DAO::Column::AGE):
        p.setAge(getInt("Enter new age", "Age", p.age(), this));
        break;
    case static_cast<int>(DAO::Column::SALARY):
        p.setSalary(getDouble("Enter new salary", "Salary", p.salary(), this));
        break;
    default:
        isItNecessaryToUpdate = false;
        break;
    }

    if(isItNecessaryToUpdate)
    {
        if(mPersonDAO->update(p))
        {
            mPersonDAO->execLastQuery();
            QMessageBox::information(this, "Success", "Choosen register was"
                                                      " successfully updated");
        }
        else
        {
            QMessageBox::critical(this, "Failure", "Could not update choosen"
                                                " register");
        }
    }
}


void MainWindow::on_btnDelete_clicked()
{
    auto row = ui->tableView->currentIndex().row();
    bool ok {false};
    auto id = mPersonDAO->model()->index(row, 0).data().toInt(&ok);
    if(ok)
    {
        auto answer = QMessageBox::question(this, "Confirm deleting",
                                            "Do you really want to delete"
                                            " selected register?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            auto res = mPersonDAO->remove(id);
            if(res)
            {
                QMessageBox::information(this, "Success", "Choosen register was"
                                                          " successfully deleted"
                                                          " from th database");
                mPersonDAO->execLastQuery();
            }
            else
            {
                QMessageBox::critical(this, "Failure", "Could not delete choosen"
                                                    " register");
            }
        }
    }
}

void MainWindow::on_btnClearForm_clicked()
{
    ui->txtName->setText("");
    ui->spinBoxAge->setValue(0);
    ui->spinBoxSalary->setValue(0);
}


void MainWindow::on_btnSearch_clicked()
{
    auto numMatches = mPersonDAO->search(ui->txtSearch->text(),
                                         ui->checkBoxAge->isChecked(),
                                         ui->spinBoxMinAge->value(),
                                         ui->spinBoxMaxAge->value(),
                                         ui->checkBoxSalary->isChecked(),
                                         ui->spinBoxMinSalary->value(),
                                         ui->spinBoxMaxSalary->value());
    if(numMatches > 0)
    {
        QMessageBox::information(this, "Search results",
                                 QString("%1 matches found").arg(numMatches));
    }
    else
    {
        QMessageBox::information(this, "Search results",
                                 QString("No matches found"));
    }
}

void MainWindow::on_btnLoadCompleteList_clicked()
{
    mPersonDAO->selectAll();
}



void MainWindow::on_action_edit_register_triggered()
{

}

void MainWindow::on_action_remove_register_triggered()
{
    on_btnDelete_clicked();
}

void MainWindow::on_action_load_full_list_triggered()
{
    mPersonDAO->selectAll();
}

void MainWindow::on_btnSort_clicked()
{
    mPersonDAO->sortBy(static_cast<DAO::Column>(ui->cmbSort->currentIndex()),
                       ui->checkBoxDecOrder->isChecked());
}

void MainWindow::on_checkBoxDecOrder_clicked()
{
    on_btnSort_clicked();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &)
{
    on_btnUpdate_clicked();
}
