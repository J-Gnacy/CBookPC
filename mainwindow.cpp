#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recipe.h"
#include "ingredient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ingredientUnitComboBox = qobject_cast<QComboBox*>(ui->ingredientUnitComboBox);
    PrepareComboBox(ingredientUnitComboBox);
    ingredientAmountSpinBox=qobject_cast<QSpinBox*>(ui->ingredientAmountSpinBox);
    ingredientNameText=qobject_cast<QTextEdit*>(ui->ingredientNameText);

    recipeAmountLabel=qobject_cast<QLabel*>(ui->recipeAmountLabel);
    recipeNameLabel=qobject_cast<QLabel*>(ui->recipeNameLabel);

    Recipe defaultRecipe(1000, "Przykładowy przepis", Unit::g);
    recipeNameLabel->setText(defaultRecipe.GetName());


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addIngredient_clicked()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->recipeArea->layout());

    QHBoxLayout* ingredientLayout=new QHBoxLayout();

    QTextEdit* ingredientName=new QTextEdit(ingredientNameText->toPlainText());

    QSpinBox* ingredientAmount=new QSpinBox();
    ingredientAmount->setValue(ingredientAmountSpinBox->value());

    QComboBox* ingredientUnit=new QComboBox();
    PrepareComboBox(ingredientUnit);
    ingredientUnit->setCurrentIndex(ingredientUnitComboBox->currentIndex());

    ingredientName->setFixedSize(100,30);
    ingredientLayout->addWidget(ingredientName);
    ingredientLayout->addWidget(ingredientAmount);
    ingredientLayout->addWidget(ingredientUnit);

    layout->addLayout(ingredientLayout);

}

void MainWindow::PrepareComboBox(QComboBox* comboBox)
{
    comboBox->addItem("g");
    comboBox->addItem("kg");
    comboBox->addItem("l");
    comboBox->addItem("ml");
}

