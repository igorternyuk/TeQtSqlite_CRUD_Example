#pragma once

#include <QSqlQueryModel>
#include <memory>

template <class T, class Tid>
class InterfaceDAO
{
public:
    virtual ~InterfaceDAO() {}
    virtual bool insert(const T& obj) = 0;
    virtual bool update(const T& obj) = 0;
    virtual bool remove(Tid) = 0;
    virtual bool selectByID(Tid id) = 0;
    virtual void selectAll() = 0;
};
