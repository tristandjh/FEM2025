#pragma once
#include "ElementBase.h"
class ElementBeam :
    public ElementBase
{
public:
    ElementBeam(int id = 0);//构造函数
    Eigen::Vector3d m_Direction; //梁截面方向向量
};

