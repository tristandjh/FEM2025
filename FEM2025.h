#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FEM2025.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FEM2025Class; };
QT_END_NAMESPACE

class FEM2025 : public QMainWindow
{
    Q_OBJECT

public:
    FEM2025(QWidget *parent = nullptr);
    ~FEM2025();

private:
    Ui::FEM2025Class *ui;
};

