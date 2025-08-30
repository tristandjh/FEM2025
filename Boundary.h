#pragma once
#include "Base.h"

class Node;

class Boundary :
    public Base
{//边界条件类
public:
    Boundary(int id = 0);
    std::vector<std::weak_ptr<Node>> m_pNode;   //作用点
    EnumKeyword::Direction m_Direction;         //方向
    double m_Value;                             //大小
};

