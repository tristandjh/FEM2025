#pragma once
#include "Base.h"
class LoadBase :
    public Base
{//荷载基类
public:
    LoadBase(int id = 0);
    EnumKeyword::Direction m_Direction; //方向
    double m_Value;                     //大小
};

