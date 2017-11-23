#include "dao.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QMessageBox>
#include <stdexcept>
#ifdef DEBUG
#include <QDebug>
#endif

DAO::DAO(const QString &pathToDBFile, QObject *parent):
    QObject(parent)
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(pathToDBFile);

    if(!mDatabase.open())
    {
        throw std::runtime_error(mDatabase.lastError().text().toStdString());

        //QMessageBox::critical(this, QString("Database error"), mDatabase.lastError().text());
        return;
    }
    QSqlQuery cmd;
    if(!cmd.exec(mQueries[SQL_CMD::CREATE_TABLE]))
    {
        throw std::runtime_error(cmd.lastError().text().toStdString());
        //QMessageBox::critical(this, QString("Database error"), cmd.lastError().text());
        return;
    }
    mModel = new QSqlQueryModel(this);
    mModel->setQuery(mQueries[SQL_CMD::READ_ALL]);
}

DAO::~DAO()
{

}

bool DAO::insert(const Person &obj)
{
    QString cmd { mQueries[SQL_CMD::INSERT].arg(obj.name()).arg(obj.age()).arg(obj.salary()) };
    QSqlQuery query;
    return query.exec(cmd);
}

bool DAO::update(const Person &obj)
{
    QString cmd { mQueries[SQL_CMD::UPDATE].arg(obj.name()).arg(obj.age())
                  .arg(obj.salary()).arg(obj.id()) };
#ifdef DEBUG
    qDebug() << "SQL = " << cmd;
#endif
    QSqlQuery query;
    return query.exec(cmd);
}

bool DAO::remove(int id)
{
    QString cmd { mQueries[SQL_CMD::REMOVE].arg(id) };
    QSqlQuery query;
    return query.exec(cmd);
}

bool DAO::selectByID(int id)
{
    QString cmd { mQueries[SQL_CMD::READ_BY_ID].arg(id) };
    mModel->setQuery(cmd);
    return mModel->rowCount() > 0;
}

void DAO::selectAll()
{
    mModel->setQuery(mQueries[SQL_CMD::READ_ALL]);
    mLastQuery = mQueries[SQL_CMD::READ_ALL];
}

size_t DAO::search(const QString &regEx, bool considerAge, int ageMin,
                   int ageMax, bool considerSalary, double salaryMin,
                   double salaryMax)
{
    QString cmd = QString("SELECT * FROM person WHERE name LIKE '%"+ regEx + "%' ");

    if(considerAge)
    {
        cmd += QString("AND (age >= %1 AND age <= %2) ").arg(ageMin).arg(ageMax);
    }

    if(considerSalary)
    {
        cmd += QString("AND (salary >= %1 AND salary <= %2) ").arg(salaryMin).arg(salaryMax);
    }
    mLastQuery = cmd;
    mModel->setQuery(cmd);
    return mModel->rowCount();
}

void DAO::execLastQuery()
{
    mModel->setQuery(mLastQuery);
}

void DAO::sortBy(DAO::Column col, bool isDescendingOrder)
{
    QString cmd = mLastQuery;
    switch (col)
    {
    case Column::ID:
        cmd += " ORDER BY id ";
        break;
    case Column::NAME:
        cmd += " ORDER BY name ";
        break;
    case Column::AGE:
        cmd += " ORDER BY age ";
        break;
    case Column::SALARY:
        cmd += " ORDER BY salary ";
        break;
    default:
        break;
    }
    cmd += isDescendingOrder ? " DESC;" : ";";
#ifdef DEBUG
    qDebug() << "SQL = " << cmd;
#endif
    mModel->setQuery(cmd);
}

QSqlQueryModel *DAO::model() const
{
    return mModel;
}
