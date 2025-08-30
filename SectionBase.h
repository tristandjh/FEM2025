#pragma once
#include "Base.h"

class Material;

class SectionBase :
    public Base
{//截面基类
public:
    SectionBase(int id = 0); //构造函数
    double m_A;                              //面积
    std::weak_ptr<Material> m_pMaterial;     //材料
};

