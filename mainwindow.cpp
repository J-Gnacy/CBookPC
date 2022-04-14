#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addIngredient_clicked()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->recipeArea->layout());

    QHBoxLayout* ingredientLayout=new QHBoxLayout();

    QTextEdit* ingredientName=new QTextEdit("Nazwa składnika");
    QSpinBox* ingredientAmount=new QSpinBox();
    QComboBox* ingredientUnit=new QComboBox();

    ingredientUnit->addItem("kek");

    ingredientName->setFixedSize(100,50);
    ingredientLayout->addWidget(ingredientName);
    ingredientLayout->addWidget(ingredientAmount);
    ingredientLayout->addWidget(ingredientUnit);

    layout->addLayout(ingredientLayout);

}

