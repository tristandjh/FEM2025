#include "Material.h"

Material::Material(int id, QString name, double e, double v, double density) :Base(id, name)
{//构造函数
    m_E = e;
    m_V = v;
    m_Density = density;
}