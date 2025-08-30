#include "FEM2025.h"

FEM2025::FEM2025(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FEM2025Class())
{
    ui->setupUi(this);
}

FEM2025::~FEM2025()
{
    delete ui;
}

