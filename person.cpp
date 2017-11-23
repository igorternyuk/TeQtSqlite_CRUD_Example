#include "person.h"

Person::Person()
{

}

Person::Person(int id, const QString &name, int age, double salary):
    mId { id }, mName { name }, mAge { age }, mSalary { salary }
{

}

int Person::id() const
{
    return mId;
}

void Person::setId(int id)
{
    mId = id;
}

QString Person::name() const
{
    return mName;
}

void Person::setName(const QString &name)
{
    mName = name;
}

int Person::age() const
{
    return mAge;
}

void Person::setAge(int age)
{
    mAge = age;
}

double Person::salary() const
{
    return mSalary;
}

void Person::setSalary(double salary)
{
    mSalary = salary;
}
