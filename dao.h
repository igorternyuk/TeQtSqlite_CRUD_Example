#pragma once

#include <QObject>
#include "person.h"
#include "interfacedao.h"
#include <QSqlDatabase>
#include <QMap>

class DAO: public QObject, public InterfaceDAO<Person, int>
{
    Q_OBJECT
public:
    enum class Column { ID, NAME, AGE, SALARY };
    explicit DAO(const QString &pathToDBFile, QObject *parent = nullptr);
    ~DAO();
    virtual bool insert(const Person &obj) override;
    virtual bool update(const Person &obj) override;
    virtual bool remove(int id) override;
    virtual bool selectByID(int id) override;
    virtual void selectAll() override;
    size_t search(const QString &regEx, bool considerAge = false, int ageMin = 0,
                int ageMax = 0, bool considerSalary = false,
                double salaryMin = 0.f, double salaryMax = 0.f);
    void execLastQuery();
    void sortBy(Column col, bool isDescendingOrder = false);
    QSqlQueryModel *model() const;

private:
    enum class SQL_CMD
    {
        CREATE_TABLE,
        INSERT,
        UPDATE,
        REMOVE,
        READ_BY_ID,
        READ_ALL
    };
    const QString mNameDB { "persons.db" };
    QMap<SQL_CMD, QString> mQueries
    {
      { SQL_CMD::CREATE_TABLE, "CREATE TABLE IF NOT EXISTS person (id INTEGER"
                               " PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT"
                               " NOT NULL, age INTEGER NOT NULL,"
                               " salary DOUBLE NOT NULL);"},
      { SQL_CMD::INSERT, "INSERT INTO person (name,age,salary) VALUES"
                         "('%1', %2, %3);"},
      { SQL_CMD::UPDATE, "UPDATE person SET name = '%1', age = %2, salary = %3 "
                         "WHERE id = %4;"},
      { SQL_CMD::REMOVE, "DELETE FROM person WHERE id = %1;"},
      { SQL_CMD::READ_BY_ID, "SELECT * FROM person WHERE id = %1;"},
      { SQL_CMD::READ_ALL, "SELECT * FROM person"}
    };
    QSqlDatabase mDatabase;
    QSqlQueryModel *mModel;
    QString mLastQuery { mQueries[SQL_CMD::READ_ALL] };
};
