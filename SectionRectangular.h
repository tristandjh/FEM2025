#pragma once
#include "SectionBase.h"
class SectionRectangular :
    public SectionBase
{//矩形截面类
public:
    SectionRectangular(int id = 0, double width = 0, double height = 0); //构造函数

    double m_Width; //宽度
    double m_Height; //高度

};

