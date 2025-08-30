#pragma once

#include <memory>
#include <numbers>

#include <Eigen/Dense>
#include <QString>

#include "EnumKeyword.h"

class Base
{//基类，用于存放id和name
public:
    Base(int id=0, QString name=""); //构造函数
    int m_Id; //编号
    QString m_Name; //名称
    virtual ~Base() = default; //虚析构函数
};

