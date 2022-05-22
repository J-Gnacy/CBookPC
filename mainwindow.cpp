#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iterator>
#include <vector>
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include "recipefilemanager.h"

 Recipe currentRecipe(100, "Przykładowy przepis", Unit::kg);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recipeLayout = qobject_cast<QVBoxLayout*>(ui->recipeArea->layout());
    recipeUnitCBox = qobject_cast<QComboBox*>(ui->recipeUnitCBox);
    PrepareComboBox(recipeUnitCBox);
    recipeAmountSpinBox=qobject_cast<QSpinBox*>(ui->recipeAmountSpinBox);
    recipeNameText=qobject_cast<QTextEdit*>(ui->recipeNameText);

    SetRecipeWidgets();

    ingredientAmountSpinBox=qobject_cast<QSpinBox*>(ui->ingredientAmountSpinBox);
    ingredientNameText=qobject_cast<QTextEdit*>(ui->ingredientNameText);
    ingredientUnitComboBox = qobject_cast<QComboBox*>(ui->ingredientUnitComboBox);
    PrepareComboBox(ingredientUnitComboBox);

    recalculateSpinBox=qobject_cast<QSpinBox*>(ui->recalculateSpinBox);
    recalculateUnitCBox=qobject_cast<QComboBox*>(ui->recalculateUnitCBox);
    PrepareComboBox(recalculateUnitCBox);
    recalculateUnitCBox->setCurrentIndex(int(currentRecipe.GetUnit()));
    recalculateUnitCBox->setDisabled(true);

    newRecipeUnitCBox= qobject_cast<QComboBox*>(ui->newRecipeUnitCBox);
    PrepareComboBox(newRecipeUnitCBox);
    newRecipeAmountSpinBox=qobject_cast<QSpinBox*>(ui->newRecipeSpinBox);
    newRecipeNameText=qobject_cast<QTextEdit*>(ui->newRecipeNameText);

    recipeListFileName="RecipeList.json";
}

MainWindow::~MainWindow()
{
    auto RemoveLayout = [&](QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
    {
        DeleteLayout(iteratorIndex.value());
    };
    ForEachInLayoutMap(RemoveLayout);

    deleteButtonToIngredientLayoutMap.clear();
    currentRecipe.ClearIngredientsList();

    delete ui;
    delete ingredientUnitComboBox;
    delete ingredientAmountSpinBox;
    delete ingredientNameText;
    delete recipeAmountSpinBox;
    delete recipeNameText;
    delete recipeUnitCBox;
    delete newRecipeUnitCBox;

    delete recipeLayout;
    delete recalculateSpinBox;
    delete recalculateUnitCBox;
}


void MainWindow::on_addIngredient_clicked()
{
    ReloadCurrentRecipe();

    QHBoxLayout* ingredientLayout=new QHBoxLayout();

    QTextEdit* ingredientName=new QTextEdit(ingredientNameText->toPlainText());
    ingredientName->setFixedSize(100,30);
    ingredientName->setDisabled(true);

    QSpinBox* ingredientAmount=new QSpinBox();
    ingredientAmount->setValue(ingredientAmountSpinBox->value());
    ingredientAmount->setDisabled(true);

    QComboBox* ingredientUnit=new QComboBox();
    PrepareComboBox(ingredientUnit);
    ingredientUnit->setCurrentIndex(ingredientUnitComboBox->currentIndex());
    ingredientUnit->setDisabled(true);

    QPushButton* deleteIngredientButton = new QPushButton("Usuń składnik");
    QObject::connect(deleteIngredientButton, &QPushButton::clicked, this, &MainWindow::DeleteIngredient);

    deleteButtonToIngredientLayoutMap.insert(deleteIngredientButton, ingredientLayout);
    currentRecipe.AddIngredient(ingredientNameText->toPlainText(), ingredientAmountSpinBox->value(), GetUnitFromCBox(ingredientUnitComboBox), deleteIngredientButton);

    ingredientLayout->addWidget(ingredientName);
    ingredientLayout->addWidget(ingredientAmount);
    ingredientLayout->addWidget(ingredientUnit);
    ingredientLayout->addWidget(deleteIngredientButton);

    recipeLayout->addLayout(deleteButtonToIngredientLayoutMap.find(deleteIngredientButton).value());
    ingredientLayout->setParent(recipeLayout);
}

void MainWindow::ForEachInLayoutMap(const std::function<void(QMap<QPushButton*, QHBoxLayout*>::const_iterator&)>& function)
{
    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = deleteButtonToIngredientLayoutMap.constBegin();
    QMap<QPushButton*, QHBoxLayout*>::const_iterator endIterator = deleteButtonToIngredientLayoutMap.constEnd();
        while (iteratorIndex !=endIterator)
        {
          function(iteratorIndex);
          iteratorIndex++;
        }
}

void MainWindow::AddLayoutFromMap(QMap<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex)
{
      QLayout* layout = iteratorIndex.value();
      recipeLayout->addLayout(layout);
}

void MainWindow::ReloadWidgetFromMap(QMap<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex)
{
    auto IngredientList = currentRecipe.GetIngredientList();
    Ingredient ingredient = IngredientList.find(iteratorIndex.key()).value();
    QLayout* layout = iteratorIndex.value();
    int layoutItemCount = layout->count();
    int layoutItemIndex=0;

    while(layoutItemIndex<layoutItemCount)
    {
      QWidget* widget = layout->itemAt(layoutItemIndex)->widget();
      if (QSpinBox* ingredientAmountSpinBox = qobject_cast<QSpinBox*>(widget))
          ingredientAmountSpinBox->setValue(ingredient.GetAmount());

      if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
         ingredientNameText->setText(ingredient.GetName());

      if(QComboBox* ingredientUnit = qobject_cast<QComboBox*>(widget))
          ingredientUnit->setCurrentIndex(int(ingredient.GetUnit()));

      layoutItemIndex++;
    }
}

void MainWindow::RevertRecalculation()
{
    currentRecipe.RevertToOriginalAmount();
    ReloadCurrentRecipe();
}

void MainWindow::PrepareComboBox(QComboBox* comboBox)
{
    comboBox->addItem("g");
    comboBox->addItem("kg");
    comboBox->addItem("l");
    comboBox->addItem("ml");
    comboBox->addItem("szt");
}


void MainWindow::on_ExitButton_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::DeleteIngredient()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QHBoxLayout* layout = deleteButtonToIngredientLayoutMap.take(button);

    DeleteLayout(layout);

    currentRecipe.DeleteIngredientByKey(button);
    deleteButtonToIngredientLayoutMap.remove(button);
}


Unit MainWindow::GetUnitFromCBox(QComboBox* comboBox)
{
    int index=comboBox->currentIndex();
    switch(index)
    {
    case 0:
        return Unit::g;
    case 1:
        return Unit::kg;
    case 2:
        return Unit::l;
    case 3:
        return Unit::ml;
    case 4:
        return Unit::szt;
    default:
        return Unit::szt;
    }
}


void MainWindow::on_recalculateButton_clicked()
{
    currentRecipe.ReloadIngredientsInMap(deleteButtonToIngredientLayoutMap);
    currentRecipe.RecalculateRecipe(recalculateSpinBox->value());
    ReloadCurrentRecipe();
    RefillRecipeLayout();
    currentRecipe.RevertToOriginalAmount();
}

void MainWindow::DeleteLayout(QLayout* layout)
{
    ClearLayout(layout);
    delete layout;
}

void MainWindow::ClearLayout(QLayout* layout)
{
    while (layout->count() != 0)
    {
        QLayoutItem* item = layout->takeAt(0);
        delete item->widget();
    }
}

void MainWindow::RefreshRecipeLayout()
{
   ClearLayout(recipeLayout);
}

void MainWindow::RefillRecipeLayout()
{
    ClearLayout(recipeLayout);

    auto AddLayout = [&](QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
    {
        AddLayoutFromMap(iteratorIndex);
    };

    ForEachInLayoutMap(AddLayout);
}

void MainWindow::ReloadCurrentRecipe()
{
    auto ReloadWidget = [&](QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
    {
        ReloadWidgetFromMap(iteratorIndex);
    };

    ForEachInLayoutMap(ReloadWidget);
}

void MainWindow::on_NewRecipeButton_clicked()
{
    Recipe r(newRecipeAmountSpinBox->value(), newRecipeNameText->toPlainText(), GetUnitFromCBox(newRecipeUnitCBox));
    currentRecipe.ClearRecipe();
    currentRecipe=r;
    SetRecipeWidgets();

}

void MainWindow::SetRecipeWidgets()
{
    recipeUnitCBox->setCurrentIndex(int(currentRecipe.GetUnit()));
    recipeUnitCBox->setDisabled(true);

    recipeAmountSpinBox->setValue(currentRecipe.GetAmount());

    recipeNameText->setText(currentRecipe.GetName());
}

void MainWindow::on_saveButton_clicked()
{
    RecipeFileManager saveFileManager;
    QJsonArray RecipeNameArray;

    saveFileManager.FileToQJsonArray(recipeListFileName, RecipeNameArray);

    saveFileManager.AddToRecipeNameList(currentRecipe.GetName());
    saveFileManager.RecipeNameListToQJsonArray(RecipeNameArray);

    saveFileManager.QJsonArrayToRecipeNameList(RecipeNameArray);

    saveFileManager.SaveQJsonArrayToFile(recipeListFileName, RecipeNameArray);
    saveFileManager.SaveRecipeToFile(currentRecipe);

}

