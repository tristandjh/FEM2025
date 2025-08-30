#pragma once
#include "Part.h"

class PartLoader
{//Part加载器，用于从文件中解析数据并创建Part对象
public:
    static std::shared_ptr<Part> LoadPart(const QString& filename, QString& errorstring); //主加载函数

private:
    static bool ParseNode(Part& part, const char*& currentpos, const char* endpos, QString& errorstring);     //解析节点数据
    static bool ParseElement(Part& part, const char*& currentpos, const char* endpos, QString& errorstring);  //解析单元数据
    static bool ParseMaterial(Part& part, const char*& currentpos, const char* endpos, QString& errorstring); //解析材料数据
    static bool ParseSection(Part& part, const char*& currentpos, const char* endpos, QString& errorstring);  //解析截面数据
};

