#include "PartLoader.h"

#include <string_view>
#include <charconv>
#include <vector>
#include <cstring>

#include <QFile>
#include <QFileInfo>


#include "Node.h"
#include "Material.h"
#include "SectionCircular.h"
#include "SectionL.h"
#include "SectionRectangular.h"
#include "ElementTruss.h"

namespace
{//匿名命名空间，与Static类似
    std::string_view GetNextLine(const char*& pos_current, const char* pos_end)
    {//从内存缓存区获取下一行
        if (pos_current >= pos_end)
        {//如果已经读到或超过文件末尾，返回一个空的string_view
            return {};
        }
        const char* pos_line_start = pos_current;
        const char* pos_line_end = static_cast<const char*>(memchr(pos_line_start, '\n', pos_end - pos_line_start));
        if (pos_line_end != nullptr)
        {//如果找到了换行符，就让pos_current指向下一行的开始
            pos_current = pos_line_end + 1;
            if (pos_line_end > pos_line_start && *(pos_line_end - 1) == '\r')
            {//处理Windows风格的换行符，如果'\n'前一个是'\r'，那么行尾应该在'\r'处
                --pos_line_end;
            }
            //返回一个string_view，指向当前行的内存起始地址和长度
            return { pos_line_start, static_cast<size_t>(pos_line_end - pos_line_start) };
        }
        else
        {//如果没有找到换行符，就让pos_current指向文件末尾
            pos_current = pos_end;
            //返回从pos_line_start到文件末尾的string_view
            return { pos_line_start, static_cast<size_t>(pos_end - pos_line_start) };
        }
    }

    std::string_view TrimSv(std::string_view sv)
    {//修剪string_view前后的空白字符和注释
        const auto pos_comment = sv.find("**");//查找"**"注释的位置
        if (pos_comment != std::string_view::npos)
        {//如果找到注释，则截取到注释之前的内容
            sv = sv.substr(0, pos_comment);
        }

        const auto pos_start = sv.find_first_not_of(" \t\r\n");//查找第一个非空字符的位置
        if (pos_start == std::string_view::npos)
        {//如果sv全是空白字符，返回一个空的string_view
            return {};
        }
        const auto pos_end = sv.find_last_not_of(" \t\r\n");//查找最后一个非空字符的位置
        return sv.substr(pos_start, pos_end - pos_start + 1);//返回从第一个空白字符到最后一个空白字符的子视图
    }

    std::vector<std::string_view> SplitSv(std::string_view sv)
    {//高性能的字符串分割函数，用于替换QT的正则表达式分割函数
        std::vector<std::string_view> result;//用于存放分割结果的容器
        static constexpr std::string_view delimiters = " \t,;，；、";//用于分割字符串的分隔符
        size_t pos_start = 0;//初始化搜索的起始位置
        while (pos_start < sv.size())
        {
            const size_t token_start = sv.find_first_not_of(delimiters, pos_start);//查找第一个非分隔符的位置
            if (token_start == std::string_view::npos)
            {//如果没有找到非分隔符的字符，则退出循环
                break;
            }
            const size_t token_end = sv.find_first_of(delimiters, token_start);//查找第一个分隔符的位置
            if (token_end == std::string_view::npos)
            {//提取从token_start到文件末尾的子视图
                result.emplace_back(sv.substr(token_start));
                break;
            }
            else
            {
                result.emplace_back(sv.substr(token_start, token_end - token_start));//提取从token_start到token_end的子视图
                pos_start = token_end;//更新搜索的起始位置
            }
        }
        return result;
    }

    bool SvToInt(std::string_view sv, int& value)
    {//将string_view转换为整数，失败返回false
        auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        return result.ec == std::errc();
    }

    bool SvToDouble(std::string_view sv, double& value)
    {//将string_view转换为浮点数，失败返回false
        auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        return result.ec == std::errc();
    }
}//匿名命名空间结束

std::shared_ptr<Part> PartLoader::LoadPart(const QString& filename, QString& errorstring)
{//主加载函数，用于从文件中读取数据并创建Part对象
    auto part = std::make_shared<Part>();
    part->m_Name = QFileInfo(filename).baseName(); //获取文件名作为Part的名称

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {//打开文件失败，返回空指针
        errorstring = "打开文件失败: " + file.errorString();
        return nullptr;
    }

    if (file.size() == 0)
    {//文件为空，但也属于有效文件，返回一个空的part
        file.close();
        return part;
    }
    //==========开始解析==========

    //1.将文件映射到内存
    uchar* data = file.map(0, file.size());
    if (data == nullptr)
    {//文件映射失败，返回空指针
        errorstring = "文件映射失败！";
        file.close();
        return nullptr;
    }

    //2.设置解析指针
    const char* pos_current = reinterpret_cast<const char*>(data);//当前指针
    const char* pos_end = pos_current + file.size();//结束指针

    bool success = true;//跟踪整个解析过程是否成功
    while (pos_current < pos_end)
    {
        std::string_view line_sv = GetNextLine(pos_current, pos_end);//获取下一行
        line_sv = TrimSv(line_sv);//去除行首和行尾空白
        if (line_sv.empty() || line_sv.starts_with("**"))
        {//跳过空行和以**开头的注释行
            continue;
        }
        if (line_sv.starts_with("*"))
        {//检查是否为关键字行
            line_sv.remove_prefix(1);//去除行首的*
            auto keyword_parts = SplitSv(line_sv);//分割关键字
            if (keyword_parts.empty()) { continue; }
            QString keyword_str = QString::fromLatin1(keyword_parts[0].data(), keyword_parts[0].size()).toUpper();
            EnumKeyword::KeyData keydata = EnumKeyword::KeyData::UNKNOWN;
            auto keyword = EnumKeyword::MapKeyData.find(keyword_str);//查找关键字对应的枚举值
            if (keyword != EnumKeyword::MapKeyData.end())
            {//如果找到了关键字对应的枚举值，则更新keydata
                keydata = keyword->second;
            }
            bool result = true;
            switch (keydata)
            {
            case EnumKeyword::KeyData::UNKNOWN:
                errorstring = QString("未知关键字: %1").arg(keyword_str);
                result = false;
                break;
            case EnumKeyword::KeyData::MATERIAL:
                result = ParseMaterial(*part, pos_current, pos_end, errorstring);
                break;
            case EnumKeyword::KeyData::SECTION:
                result = ParseSection(*part, pos_current, pos_end, errorstring);
                break;
            case EnumKeyword::KeyData::NODE:
                result = ParseNode(*part, pos_current, pos_end, errorstring);
                break;
            case EnumKeyword::KeyData::ELEMENT:
                result = ParseElement(*part, pos_current, pos_end, errorstring);
                break;
            }
            if (result == false)
            {//如果任何子解析函数失败，立即停止整个解析过程
                success = false;
                break;
            }
        }
    }
    //3.解除文件映射
    file.unmap(data);
    //4.关闭文件
    file.close();
    //5.返回解析结果
    if (success)
    {
        return part;
    }
    else
    {
        return nullptr;
    }
}

bool PartLoader::ParseNode(Part& part, const char*& currentpos, const char* endpos, QString& errorstring)
{//解析节点数据
    while (currentpos < endpos)
    {
        const char* savepos = currentpos;
        std::string_view line_sv = GetNextLine(currentpos, endpos);//获取下一行
        line_sv = TrimSv(line_sv);//去除行首和行尾空白以及注释
        if (line_sv.empty())
        {//跳过修剪后的空行
            continue;
        }
        if (line_sv.starts_with("*"))
        {
            currentpos = savepos;
            return true;
        }
        auto data = SplitSv(line_sv);//分割节点数据行
        if (data.size() < 4)
        {//数据行项目不足
            errorstring = "节点数据行项目不足！（至少需要 ID,X,Y,Z）" + QString::fromStdString(std::string(line_sv));
            return false;
        }
        int id;
        double x, y, z;
        if (!SvToInt(data[0], id) ||
            !SvToDouble(data[1], x) ||
            !SvToDouble(data[2], y) ||
            !SvToDouble(data[3], z))
        {//数据类型转换失败
            errorstring = "节点数据类型转换失败！（ID,X,Y,Z）" + QString::fromStdString(std::string(line_sv));
            return false;
        }
        auto pentity = std::make_shared<Node>(id, x, y, z);
        part.m_Node[id] = pentity;
    }
    return true;
}

bool PartLoader::ParseElement(Part& part, const char*& currentpos, const char* endpos, QString& errorstring)
{//解析单元数据
    while (currentpos < endpos)
    {
        const char* savepos = currentpos;
        std::string_view line_sv = GetNextLine(currentpos, endpos);//获取下一行
        line_sv = TrimSv(line_sv);//去除行首和行尾空白以及注释
        if (line_sv.empty())
        {//跳过修剪后的空行
            continue;
        }
        if (line_sv.starts_with("*"))
        {
            currentpos = savepos;
            return true;
        }
        auto data = SplitSv(line_sv);//分割节点数据行
        if (data.empty())
        {//跳过分割后的空行
            continue;
        }
        auto typestr = QString::fromLatin1(data[0].data(), data[0].size()).toUpper();
        auto type = EnumKeyword::ElementType::UNKNOWN;
        auto it = EnumKeyword::MapElementType.find(typestr);//查找关键字对应的枚举值
        if (it != EnumKeyword::MapElementType.end())
        {//如果找到了关键字对应的枚举值，则更新type
            type = it->second;
        }
        switch (type)
        {
            case EnumKeyword::ElementType::UNKNOWN:
                errorstring = "未知单元类型：" + typestr;
                return false;
            case EnumKeyword::ElementType::T3D2:
            {//解析桁架单元数据
                if (data.size() < 5)
                {//数据行项目不足
                    errorstring = "T3D2单元数据行项目不足！（至少需要 Type,ID,SectionID,NodeID1,NodeID2）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                int id, sectionid, nodeid1, nodeid2;
                if (!SvToInt(data[1], id) ||
                    !SvToInt(data[2], sectionid) ||
                    !SvToInt(data[3], nodeid1) ||
                    !SvToInt(data[4], nodeid2))
                {//数据类型转换失败
                    errorstring = "T3D2单元数据类型转换失败！（Type,ID,SectionID,NodeID1,NodeID2）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                auto pentity = std::make_shared<ElementTruss>(id);
                auto psection = part.m_Section.find(sectionid);
                if (psection == part.m_Section.end())
                {//找不到对应的材料
                    errorstring = "找不到截面ID：" + QString::number(sectionid);
                    return false;
                }
                pentity->m_pSection = psection->second;
                part.m_Element[id] = pentity;
                break;
            }
            case EnumKeyword::ElementType::B31:
            {//解析B31单元数据
                errorstring = "B31单元暂不支持解析！";
                break;
            }
        }
    }
    return true;
}

bool PartLoader::ParseMaterial(Part& part, const char*& currentpos, const char* endpos, QString& errorstring)
{//解析材料数据
    while (currentpos < endpos)
    {
        const char* savepos = currentpos;
        std::string_view line_sv = GetNextLine(currentpos, endpos);//获取下一行
        line_sv = TrimSv(line_sv);//去除行首和行尾空白以及注释
        if (line_sv.empty())
        {//跳过修剪后的空行
            continue;
        }
        if (line_sv.starts_with("*"))
        {
            currentpos = savepos;
            return true;
        }
        auto data = SplitSv(line_sv);//分割节点数据行
        if (data.size() < 5)
        {//数据行项目不足
            errorstring = "节点数据行项目不足！（至少需要Name,ID,E,V,Density）" + QString::fromStdString(std::string(line_sv));
            return false;
        }
        int id;
        QString name;
        double e, v, density;
        if (!SvToInt(data[1], id) ||
            !SvToDouble(data[2], e) ||
            !SvToDouble(data[3], v) ||
            !SvToDouble(data[4], density))
        {//数据类型转换失败
            errorstring = "材料数据类型转换失败！（ID,E,V,Density）" + QString::fromStdString(std::string(line_sv));
            return false;
        }
        name = QString::fromStdString(std::string(data[0]));
        auto pentity = std::make_shared<Material>(id, name, e, v, density);
        part.m_Material[id] = pentity;
    }
    return true;
}

bool PartLoader::ParseSection(Part& part, const char*& currentpos, const char* endpos, QString& errorstring)
{//解析截面数据
    while (currentpos < endpos)
    {
        const char* savepos = currentpos;
        std::string_view line_sv = GetNextLine(currentpos, endpos);//获取下一行
        line_sv = TrimSv(line_sv);//去除行首和行尾空白以及注释
        if (line_sv.empty())
        {//跳过修剪后的空行
            continue;
        }
        if (line_sv.starts_with("*"))
        {
            currentpos = savepos;
            return true;
        }
        auto data = SplitSv(line_sv);//分割节点数据行
        if (data.empty())
        {//跳过分割后的空行
            continue;
        }
        auto typestr = QString::fromLatin1(data[0].data(), data[0].size()).toUpper();
        auto type = EnumKeyword::SectionType::UNKNOWN;
        auto it = EnumKeyword::MapSectionType.find(typestr);//查找关键字对应的枚举值
        if (it != EnumKeyword::MapSectionType.end())
        {//如果找到了关键字对应的枚举值，则更新type
            type = it->second;
        }
        switch (type)
        {
            case EnumKeyword::SectionType::UNKNOWN:
                errorstring = "未知截面类型：" + typestr;
                return false;
            case EnumKeyword::SectionType::CIRCULAR:
            {//解析圆形截面数据
                if (data.size() < 4)
                {//数据行项目不足
                    errorstring = "圆形截面数据行项目不足！（至少需要 Type,ID,MaterialID,Radius）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                int id, materialid;
                double radius;
                if (!SvToInt(data[1], id) ||
                    !SvToInt(data[2], materialid) ||
                    !SvToDouble(data[3], radius))
                {//数据类型转换失败
                    errorstring = "圆形截面数据类型转换失败！（ID,MaterialID,Radius）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                auto pentity = std::make_shared<SectionCircular>(id, radius);
                auto pmaterial = part.m_Material.find(materialid);
                if (pmaterial == part.m_Material.end())
                {//找不到对应的材料
                    errorstring = "找不到材料ID：" + QString::number(materialid);
                    return false;
                }
                pentity->m_pMaterial = pmaterial->second;
                part.m_Section[id] = pentity;
                break;
            }
            case EnumKeyword::SectionType::L:
            {//解析L形截面数据
                if (data.size() < 7)
                {//数据行项目不足
                    errorstring = "L形截面数据行项目不足！（至少需要 Type,ID,MaterialID,Width,Height,Thickness1,Thickness2）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                int id, materialid;
                double width, height, thickness1, thickness2;
                if (!SvToInt(data[1], id) ||
                    !SvToInt(data[2], materialid) ||
                    !SvToDouble(data[3], width) ||
                    !SvToDouble(data[4], height) ||
                    !SvToDouble(data[5], thickness1) ||
                    !SvToDouble(data[6], thickness2))
                {//数据类型转换失败
                    errorstring = "L形截面数据类型转换失败！（ID,MaterialID,Width,Height,Thickness1,Thickness2）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                auto pentity = std::make_shared<SectionL>(id, width, height, thickness1, thickness2);
                auto pmaterial = part.m_Material.find(materialid);
                if (pmaterial == part.m_Material.end())
                {//找不到对应的材料
                    errorstring = "找不到材料ID：" + QString::number(materialid);
                    return false;
                }
                pentity->m_pMaterial = pmaterial->second;
                part.m_Section[id] = pentity;
                break;
            }
            case EnumKeyword::SectionType::RECTANGULAR:
            {//解析矩形截面数据
                if (data.size() < 5)
                {//数据行项目不足
                    errorstring = "矩形截面数据行项目不足！（至少需要 Type,ID,MaterialID,Width,Height）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                int id, materialid;
                double width, height;
                if (!SvToInt(data[1], id) ||
                    !SvToInt(data[2], materialid) ||
                    !SvToDouble(data[3], width) ||
                    !SvToDouble(data[4], height))
                {//数据类型转换失败
                    errorstring = "L形截面数据类型转换失败！（ID,MaterialID,Width,Height）" + QString::fromStdString(std::string(line_sv));
                    return false;
                }
                auto pentity = std::make_shared<SectionL>(id, width, height);
                auto pmaterial = part.m_Material.find(materialid);
                if (pmaterial == part.m_Material.end())
                {//找不到对应的材料
                    errorstring = "找不到材料ID：" + QString::number(materialid);
                    return false;
                }
                pentity->m_pMaterial = pmaterial->second;
                part.m_Section[id] = pentity;
                break;
            }
        }
    }
    return true;
}
