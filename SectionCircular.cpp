#include "SectionCircular.h"

SectionCircular::SectionCircular(int id, double r):SectionBase(id)
{//构造函数
    m_R = r;
    m_A = std::numbers::pi * m_R * m_R;
}
