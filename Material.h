#pragma once
#include "Base.h"
class Material :
    public Base
{//材料类
public:
    Material(int id = 0, QString name = "", double e = 0, double v = 0, double density = 0); //构造函数
    double m_E;       //弹性模量
    double m_V;       //泊松比
    double m_Density; //密度
};

