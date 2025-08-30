#pragma once
#include "Base.h"

class Part;

class Instance : public Base
{//实例类，是对Part类的一次“摆放”
public:
    Instance(int id = 0, QString name = "", std::shared_ptr<const Part> part = nullptr); //构造函数

    std::shared_ptr<const Part> m_PPart; //指向所属的Part
    Eigen::Affine3d m_Transform; //变换矩阵
};

