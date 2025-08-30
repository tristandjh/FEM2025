#include "SectionL.h"

SectionL::SectionL(int id, double width, double height, double thickness1, double thickness2):SectionBase(id)
{//构造函数
    m_Width = width;
    m_Height = height;
    m_Thickness1 = thickness1;
    m_Thickness2 = thickness2;
    m_A = m_Height * m_Thickness1 + (m_Width - m_Thickness1) * m_Thickness2;
}
