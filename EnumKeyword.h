#pragma once
#include <map>
#include <QString>
class EnumKeyword
{//枚举类，用于存放枚举
public:
    enum class KeyData
    {//数据文件的顶级关键字
        UNKNOWN, MATERIAL, SECTION, NODE, ELEMENT, BOUNDARY, LOAD
    };
    static const std::map<QString, KeyData> MapKeyData; //文件字符串->枚举值映射表

    enum class Direction
    {//三维坐标系的方向
        UNKNOWN, X, Y, Z, RX, RY, RZ
    };
    static const std::map<QString, Direction> MapDirection; //方向字符串->枚举值映射表

    enum class ElementType
    {//单元类型
        UNKNOWN, T3D2, B31
    };
    static const std::map<QString, ElementType> MapElementType; //单元类型字符串->枚举值映射表

    enum class LoadType
    {//载荷类型
        UNKNOWN, CLOAD, DLOAD
    };
    static const std::map<QString, LoadType> MapLoadType; //载荷类型字符串->枚举值映射表

    enum class SectionType
    {//截面类型
        UNKNOWN, CIRCULAR, L, RECTANGULAR
    };
    static const std::map<QString, SectionType> MapSectionType; //截面类型字符串->枚举值映射表
};

