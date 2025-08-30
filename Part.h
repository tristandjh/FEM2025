#pragma once
#include "Base.h"

class Node;
class ElementBase;
class Material;
class SectionBase;

class Part : public Base
{//部件类
public:
    Part(int id = 0, QString name = ""); //构造函数

    std::map<int, std::shared_ptr<Node>> m_Node;           //节点容器
    std::map<int, std::shared_ptr<ElementBase>> m_Element; //单元容器
    std::map<int, std::shared_ptr<Material>> m_Material;   //材料容器
    std::map<int, std::shared_ptr<SectionBase>> m_Section; //截面容器
};

