#pragma once

#include <memory>
#include <map>

#include <QString>

class Part;

class PartManager
{//Part管理类，缓存已加载的Part
private:
    PartManager();

    std::map<QString, std::shared_ptr<Part>> m_Parts; //已加载的Part容器，Part名称->Part指针映射

    QString m_PartPath; //Part文件路径

public:
    //禁止拷贝和赋值，确保全局唯一性
    PartManager(const PartManager&) = delete;
    void operator=(const PartManager&) = delete;

    static PartManager& GetInstance(); //获取单例实例的静态方法

    void SetPartPath(const QString& path); //设置Part文件路径

    std::shared_ptr<Part> GetPart(const QString& name); //加载Part，如果已加载则直接返回，否则从文件中加载并缓存
};

