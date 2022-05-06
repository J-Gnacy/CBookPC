#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iterator>
#include <vector>


 Recipe currentRecipe(100, "Przykładowy przepis", Unit::kg);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    recipeLayout = qobject_cast<QVBoxLayout*>(ui->recipeArea->layout());
    ingredientUnitComboBox = qobject_cast<QComboBox*>(ui->ingredientUnitComboBox);
    PrepareComboBox(ingredientUnitComboBox);

    recipeUnitCBox = qobject_cast<QComboBox*>(ui->recipeUnitCBox);
    PrepareComboBox(recipeUnitCBox);
    recipeUnitCBox->setCurrentIndex(int(currentRecipe.GetUnit()));
    recipeUnitCBox->setDisabled(true);

    newRecipeUnitCBox= qobject_cast<QComboBox*>(ui->newRecipeUnitCBox);
    PrepareComboBox(newRecipeUnitCBox);

    ingredientAmountSpinBox=qobject_cast<QSpinBox*>(ui->ingredientAmountSpinBox);
    ingredientNameText=qobject_cast<QTextEdit*>(ui->ingredientNameText);

    recipeAmountLabel=qobject_cast<QLabel*>(ui->recipeAmountLabel);
    recipeNameLabel=qobject_cast<QLabel*>(ui->recipeNameLabel);

    recipeNameLabel->setText(currentRecipe.GetName());
    recipeAmountLabel->setText(QString::number(currentRecipe.GetAmount()));

    recalculateSpinBox=qobject_cast<QSpinBox*>(ui->recalculateSpinBox);

    recalculateUnitCBox=qobject_cast<QComboBox*>(ui->recalculateUnitCBox);
    PrepareComboBox(recalculateUnitCBox);
    recalculateUnitCBox->setCurrentIndex(int(currentRecipe.GetUnit()));
    recalculateUnitCBox->setDisabled(true);


}

MainWindow::~MainWindow()
{
    QMap<QPushButton*, QHBoxLayout*>::const_iterator i = deleteButtonToIngredientLayoutMap.constBegin();
        while (i != deleteButtonToIngredientLayoutMap.constEnd())
        {
          DeleteLayout(i.value());
          i++;
        }

    deleteButtonToIngredientLayoutMap.clear();
    currentRecipe.ClearIngredientsList();

    delete ui;
    delete ingredientUnitComboBox;
    delete ingredientAmountSpinBox;
    delete ingredientNameText;
    delete recipeAmountLabel;
    delete recipeNameLabel;
    delete recipeUnitCBox;
    delete newRecipeUnitCBox;
    delete scrollArea;
    delete recipeLayout;
    delete recalculateSpinBox;
    delete recalculateUnitCBox;



}


void MainWindow::on_addIngredient_clicked()
{


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
    ingredientUnit->setFixedSize(50,50);

    QPushButton* deleteIngredientButton = new QPushButton("Usuń składnik");
    QObject::connect(deleteIngredientButton, &QPushButton::clicked, this, &MainWindow::DeleteIngredient);
    deleteIngredientButton->setMaximumHeight(50);
    deleteIngredientButton->setMaximumWidth(100);

    deleteButtonToIngredientLayoutMap.insert(deleteIngredientButton, ingredientLayout);
    currentRecipe.AddIngredient(ingredientNameText->toPlainText(), ingredientAmountSpinBox->value(), GetUnitFromCBox(ingredientUnitComboBox), deleteIngredientButton);
    auto list = currentRecipe.GetIngredientList();

    ingredientLayout->addWidget(ingredientName);
    ingredientLayout->addWidget(ingredientAmount);
    ingredientLayout->addWidget(ingredientUnit);
    ingredientLayout->addWidget(deleteIngredientButton);

    recipeLayout->addLayout(deleteButtonToIngredientLayoutMap.find(deleteIngredientButton).value());
    ingredientLayout->setParent(recipeLayout);


}

void MainWindow::ForEachInLayoutMap(const std::function<void>& function)
{

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

    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = deleteButtonToIngredientLayoutMap.constBegin();
    QMap<QPushButton*, QHBoxLayout*>::const_iterator endIterator = deleteButtonToIngredientLayoutMap.constEnd();
        while (iteratorIndex !=endIterator)
        {
          QLayout* layout = iteratorIndex.value();
          recipeLayout->addLayout(layout);
          iteratorIndex++;
        }
}

void MainWindow::ReloadCurrentRecipe()
{

    auto IngredientList = currentRecipe.GetIngredientList();

    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = deleteButtonToIngredientLayoutMap.constBegin();
    QMap<QPushButton*, QHBoxLayout*>::const_iterator endIterator = deleteButtonToIngredientLayoutMap.constEnd();
        while (iteratorIndex !=endIterator)
        {
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

            iteratorIndex++;
        }
}
