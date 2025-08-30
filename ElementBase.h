#pragma once
#include "Base.h"

class Node;
class SectionBase;

class ElementBase :
    public Base
{//单元基类
public:
    ElementBase(int id);                        //构造函数
    std::vector<std::weak_ptr<Node>> m_pNode;   //节点
    std::weak_ptr<SectionBase> m_pSection;      //所属截面
};

