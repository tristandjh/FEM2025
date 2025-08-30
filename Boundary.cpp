#include "Boundary.h"

Boundary::Boundary(int id):Base(id)
{//构造函数
    m_Direction = EnumKeyword::Direction::UNKNOWN;
    m_Value = 0.0;
}
