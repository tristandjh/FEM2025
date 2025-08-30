#include "ElementBase.h"

ElementBase::ElementBase(int id):Base(id)
{//构造函数
    m_pNode.reserve(2);//预分配空间
}
