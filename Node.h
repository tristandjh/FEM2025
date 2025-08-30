#pragma once
#include "Base.h"
class Node : public Base
{//节点类
public:
    Node(int id=0, double x=0, double y=0, double z=0);//构造函数
    double m_X, m_Y, m_Z;//节点坐标

};

