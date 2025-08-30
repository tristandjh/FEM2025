#include "FEM2025.h"
#include <QtWidgets/QApplication>

#include "Part.h"
#include "PartManager.h"
#include <QDebug>

void testPartManager()
{
    qDebug() << "--- Testing PartManager ---";

    // 第一次请求 TowerA，应该会从文件加载
    qDebug() << "Requesting TowerA for the first time...";
    auto towerA_1 = PartManager::GetInstance().GetPart("TowerA");
    if (towerA_1) {
        qDebug() << "Got TowerA, name:" << towerA_1->m_Name;
    }
    else {
        qDebug() << "Failed to get TowerA.";
    }

    qDebug() << "\nRequesting TowerA for the second time...";
    // 第二次请求 TowerA，应该会从缓存中获取
    auto towerA_2 = PartManager::GetInstance().GetPart("TowerA");
    if (towerA_2) {
        qDebug() << "Got TowerA again, name:" << towerA_2->m_Name;
    }
    qDebug() << "\nRequesting a non-existent part...";
    // 请求一个不存在的 Part，应该会加载失败
    auto non_existent = PartManager::GetInstance().GetPart("NonExistentPart");
    if (!non_existent) {
        qDebug() << "Correctly failed to get non-existent part.";
    }

    qDebug() << "--- Test Finished ---";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    testPartManager();
    FEM2025 window;
    window.show();
    return app.exec();
}
