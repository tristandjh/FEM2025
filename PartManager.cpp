#include "PartManager.h"

#include <QDir>
#include <QDebug>

#include "PartLoader.h"

PartManager::PartManager()
{//构造函数
    m_PartPath = QDir::currentPath() + "/Parts/";
    qDebug() << "Current Part Path:" << m_PartPath;
}

PartManager& PartManager::GetInstance()
{
    static PartManager instance;
    return instance;
}

void PartManager::SetPartPath(const QString& path)
{//设置Part路径
    m_PartPath = path;
    if (!m_PartPath.endsWith('/') && !m_PartPath.endsWith('\\'))
    {//确保路径以斜杠结尾
        m_PartPath += '/';
    }
    qDebug() << "Current Part Path:" << m_PartPath;
}

std::shared_ptr<Part> PartManager::GetPart(const QString& name)
{//加载Part
    auto check = m_Parts.find(name);
    if (check != m_Parts.end())
    {//如果已经加载过，直接返回
        qDebug() << "Part: " << name << " has been loaded.";
        return check->second;
    }
    qDebug() << "Loading Part: " << name;
    auto filepath = m_PartPath + name + ".txt"; //文件路径，目前假定文件.txt格式
    QString errormsg;
    auto newpart = PartLoader::LoadPart(filepath, errormsg);

    if (newpart)
    {//如果成功加载，加入到缓存
        qDebug() << "Part: " << name << " 成功加载，加入缓存。";
        m_Parts[name] = newpart;
    }
    else
    {//如果失败，输出错误信息
        qDebug() << "Part: " << name << " 加载失败，错误信息：" << errormsg;
    }

    return newpart;//返回新加载的Part，如果失败，返回空指针
}
