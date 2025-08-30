#pragma once
#include "SectionBase.h"
class SectionL :
    public SectionBase
{//L型角钢截面类
public:
    SectionL(int id = 0,double width = 0, double height = 0, double thickness1 = 0, double thickness2 = 0); //构造函数

    double m_Width;     //整个截面宽度
    double m_Height;    //整个截面高度
    double m_Thickness1;//角钢厚度1
    double m_Thickness2;//角钢厚度2
};

