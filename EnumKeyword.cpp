#include "EnumKeyword.h"

const std::map<QString, EnumKeyword::KeyData> EnumKeyword::MapKeyData =
{//将数据文件中的关键字字符串映射到对应的枚举值
    {"MATERIAL", EnumKeyword::KeyData::MATERIAL},
    {"SECTION",  EnumKeyword::KeyData::SECTION},
    {"NODE",     EnumKeyword::KeyData::NODE},
    {"ELEMENT",  EnumKeyword::KeyData::ELEMENT},
    {"BOUNDARY", EnumKeyword::KeyData::BOUNDARY},
    {"LOAD",     EnumKeyword::KeyData::LOAD},
};

const std::map<QString, EnumKeyword::Direction> EnumKeyword::MapDirection =
{//将数据文件中的方向字符串映射到对应的枚举值
    {"X",  EnumKeyword::Direction::X},
    {"Y",  EnumKeyword::Direction::Y},
    {"Z",  EnumKeyword::Direction::Z},
    {"RX", EnumKeyword::Direction::RX},
    {"RY", EnumKeyword::Direction::RY},
    {"RZ", EnumKeyword::Direction::RZ},
};

const std::map<QString, EnumKeyword::ElementType> EnumKeyword::MapElementType =
{//将数据文件中的单元类型字符串映射到对应的枚举值
    {"T3D2", EnumKeyword::ElementType::T3D2},
    {"B31",  EnumKeyword::ElementType::B31},
};

const std::map<QString, EnumKeyword::LoadType> EnumKeyword::MapLoadType =
{//将数据文件中的载荷类型字符串映射到对应的枚举值
    {"CLOAD", EnumKeyword::LoadType::CLOAD},
    {"DLOAD", EnumKeyword::LoadType::DLOAD},
};

const std::map<QString, EnumKeyword::SectionType> EnumKeyword::MapSectionType =
{//将数据文件中的截面类型字符串映射到对应的枚举值
    {"CIRCULAR",    EnumKeyword::SectionType::CIRCULAR},
    {"RECTANGULAR", EnumKeyword::SectionType::RECTANGULAR},
    {"L",           EnumKeyword::SectionType::L},
};
