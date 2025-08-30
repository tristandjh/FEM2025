#include "Instance.h"

Instance::Instance(int id, QString name, std::shared_ptr<const Part> part):Base(id,name)
{//构造函数
    m_PPart = part;
    m_Transform.setIdentity();//初始化变换矩阵为单位矩阵
}
