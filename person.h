#pragma once
#include <QString>

class Person
{
public:
    //Constructors
    explicit Person();
    explicit Person(int id, const QString &name, int age, double salary);
   /*Person(const Person& other) = delete;
    Person& operator=(const Person& other) = delete;
    Person(Person&& other) = delete;
    Person& operator=(Person&& other) = delete;*/
    //Getters and setters (accesadores y mudadores)
    int id() const;
    void setId(int id);
    QString name() const;
    void setName(const QString &name);
    int age() const;
    void setAge(int age);
    double salary() const;
    void setSalary(double salary);

private:
    int mId;
    QString mName;
    int mAge;
    double mSalary;
};
