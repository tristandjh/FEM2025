#pragma once
#include "SectionBase.h"
class SectionCircular :
    public SectionBase
{//圆形截面类
public:
    SectionCircular(int id = 0, double r = 0); //构造函数
    double m_R; //半径
};

