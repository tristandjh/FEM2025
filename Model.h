#pragma once
#include "Base.h"

class Instance;

class Model : public Base
{//模型类，用于组装实例
public:
    Model(int id = 0, QString name = ""); //构造函数

    std::vector<std::shared_ptr<Instance>> m_Instances; //组装实例
};

