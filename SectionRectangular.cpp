#include "SectionRectangular.h"

SectionRectangular::SectionRectangular(int id, double width, double height):SectionBase(id)
{//构造函数
    m_Width = width;
    m_Height = height;
    m_A = m_Width * m_Height;
}
