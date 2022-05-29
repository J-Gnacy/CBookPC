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
#include <QVBoxLayout>
#include "recipefilemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QScopedPointer<QString>dupa=QScopedPointer<QString>(new QString("dupa"));

    currentRecipe = QSharedPointer<Recipe>(new Recipe(100, "Przykladowy przepis", Unit::kg));

    infoLabel = QSharedPointer<QLabel>(ui->infoLabel);
    infoLabel->setText(currentRecipe->GetName());

    recipeLayout = QSharedPointer<QVBoxLayout>(qobject_cast<QVBoxLayout*>(ui->recipeArea->layout()));

    layoutManager.reset((new LayoutManager(currentRecipe.get(), recipeLayout.get(), infoLabel.get())));

    recipeUnitCBox.reset(ui->recipeUnitCBox);


    layoutManager->PrepareUnitComboBox(recipeUnitCBox.get());

    recipeAmountSpinBox.reset(ui->recipeAmountSpinBox);

    recipeNameText.reset(ui->recipeNameText);

    SetRecipeWidgets();

    ingredientAmountSpinBox.reset(ui->ingredientAmountSpinBox);
    ingredientNameText.reset(ui->ingredientNameText);
    ingredientUnitComboBox.reset(ui->ingredientUnitComboBox);
    layoutManager->PrepareUnitComboBox(ingredientUnitComboBox.get());

    recalculateSpinBox.reset(ui->recalculateSpinBox);
    recalculateUnitCBox.reset(ui->recalculateUnitCBox);
    layoutManager->PrepareUnitComboBox(recalculateUnitCBox.get());
    recalculateUnitCBox->setCurrentIndex(int(currentRecipe->GetUnit()));
    recalculateUnitCBox->setDisabled(true);

    newRecipeUnitCBox.reset(ui->newRecipeUnitCBox);
    layoutManager->PrepareUnitComboBox(newRecipeUnitCBox.get());
    newRecipeAmountSpinBox.reset(ui->newRecipeSpinBox);
    newRecipeNameText.reset(ui->newRecipeNameText);



    recipeListFileName="RecipeList.json";
}

MainWindow::~MainWindow()
{

}


void MainWindow::on_addIngredient_clicked()
{
    layoutManager->ReloadCurrentRecipe();

    QString ingredientName = ingredientNameText->toPlainText();

    bool ingredientAlreadyIn = currentRecipe->IsIngredientInRecipe(ingredientName);
    if(ingredientAlreadyIn)
    {
        ingredientName = ingredientName + QString::number((ButtonIngredientLayoutHash.count()));
    }

    int ingredientAmount = ingredientAmountSpinBox->value();
    Unit ingredientUnit=Recipe::GetUnitFromInt(ingredientUnitComboBox->currentIndex());


    Ingredient ing(ingredientName, ingredientAmount, ingredientUnit);

    layoutManager->AddIngredientLayout(ing);
}

void MainWindow::on_ExitButton_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_recalculateButton_clicked()
{
    currentRecipe->ReloadIngredientsInMap(ButtonIngredientLayoutHash);
    currentRecipe->RecalculateRecipe(recalculateSpinBox->value());
    layoutManager->ReloadCurrentRecipe();
    layoutManager->RefillRecipeLayout();
    currentRecipe->RevertToOriginalAmount();
}

void MainWindow::on_saveButton_clicked()
{
    SaveRecipe();
}

void MainWindow::on_LoadButton_clicked()
{
    LoadRecipeListToRecipeLayout();
}

void MainWindow::on_NewRecipeButton_clicked()
{
    Recipe* r = new Recipe(newRecipeAmountSpinBox->value(), newRecipeNameText->toPlainText(), GetUnitFromCBox(newRecipeUnitCBox));
    currentRecipe->ClearRecipe();
    currentRecipe=r;
    SetRecipeWidgets();
    ClearRecipeLayout();
    layoutManager->ClearIngredientLayoutHash();
    infoLabel->setText(currentRecipe->GetName());
}



//void MainWindow::ForEachInLayoutMap(const std::function<void(QHash<QPushButton*, QHBoxLayout*>::const_iterator&)>& function)
//{
//    QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = ButtonIngredientLayoutHash.constBegin();
//    QHash<QPushButton*, QHBoxLayout*>::const_iterator endIterator = ButtonIngredientLayoutHash.constEnd();
//        while (iteratorIndex !=endIterator)
//        {
//          function(iteratorIndex);
//          iteratorIndex++;
//        }
//}

//void MainWindow::ForEachInRecipeHash(const std::function<void(QHash<QPushButton*, QHBoxLayout*>::const_iterator&)>& function)
//{
//    QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = ButtonRecipeLayoutHash.constBegin();
//    QHash<QPushButton*, QHBoxLayout*>::const_iterator endIterator = ButtonRecipeLayoutHash.constEnd();
//        while (iteratorIndex !=endIterator)
//        {
//          function(iteratorIndex);
//          iteratorIndex++;
//        }
//}

//void MainWindow::AddLayoutFromMap(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex)
//{
//      QLayout* layout = iteratorIndex.value();
//      recipeLayout->addLayout(layout);
//}

//void MainWindow::AddLayoutFromIngredient(Ingredient ingredient)
//{
//    QHBoxLayout* ingredientLayout=new QHBoxLayout();

//    QTextEdit* ingredientNameTextEdit=new QTextEdit(ingredient.GetName());
//    ingredientNameTextEdit->setFixedSize(100,30);
//    ingredientNameTextEdit->setDisabled(true);

//    QSpinBox* ingredientAmountSpinBox=new QSpinBox();
//    ingredientAmountSpinBox->setValue(ingredient.GetAmount());
//    ingredientAmountSpinBox->setDisabled(true);

//    QComboBox* ingredientUnitComboBox=new QComboBox();
//    PrepareComboBox(ingredientUnitComboBox);
//    ingredientUnitComboBox->setCurrentIndex(int(ingredient.GetUnit()));
//    ingredientUnitComboBox->setDisabled(true);

//    QPushButton* deleteIngredientButton = new QPushButton("Usuń składnik");
//    QObject::connect(deleteIngredientButton, &QPushButton::clicked, this, &MainWindow::DeleteIngredient);

//    ButtonIngredientLayoutHash.insert(deleteIngredientButton, ingredientLayout);

//    ingredientLayout->addWidget(ingredientNameTextEdit);
//    ingredientLayout->addWidget(ingredientAmountSpinBox);
//    ingredientLayout->addWidget(ingredientUnitComboBox);
//    ingredientLayout->addWidget(deleteIngredientButton);

//    recipeLayout->addLayout(ButtonIngredientLayoutHash.find(deleteIngredientButton).value());
//    ingredientLayout->setParent(recipeLayout);
//}

//void MainWindow::IngredientVectorToRecipeLayout()
//{
//    ClearRecipeLayout();
//    auto ingredientVector = currentRecipe.GetIngredientVector();
//    int endIndex = ingredientVector.count();
//    int index = 0;
//    while(index<endIndex)
//    {
//        AddLayoutFromIngredient(ingredientVector[index]);
//        index++;
//    }
//}

//void MainWindow::ReloadWidgetFromMap(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex)
//{
//     auto IngredientList = currentRecipe.GetIngredientVector();

//     QLayout* layout = iteratorIndex.value();
//     int layoutItemCount = layout->count();
//     int layoutItemIndex=0;

//     QString name = GetIngredientNameByButton(iteratorIndex.key());
//     int ingredientIndex = currentRecipe.SearchIngredientByName(name);
//     auto ingredientVector = currentRecipe.GetIngredientVector();
//     Ingredient ingredient = ingredientVector[ingredientIndex];

//    while(layoutItemIndex<layoutItemCount)
//    {
//      QWidget* widget = layout->itemAt(layoutItemIndex)->widget();
//      if (QSpinBox* ingredientAmountSpinBox = qobject_cast<QSpinBox*>(widget))
//          ingredientAmountSpinBox->setValue(ingredient.GetAmount());

//      if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
//         ingredientNameText->setText(ingredient.GetName());

//      if(QComboBox* ingredientUnit = qobject_cast<QComboBox*>(widget))
//          ingredientUnit->setCurrentIndex(int(ingredient.GetUnit()));

//      layoutItemIndex++;
//    }
//}


//void MainWindow::ClearButtonLayoutMap()
//{
//    auto DeleteFromIndex = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator &iteratorIndex){
//        QPushButton* button = iteratorIndex.key();
//        QLayout* layout = iteratorIndex.value();

//        while (layout->count() != 0)
//        {
//            QWidget* widget = layout->takeAt(0)->widget();
//            if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
//                currentRecipe.DeleteIngredientByName(ingredientNameText->toPlainText());
//            if(widget)
//            {
//                widget->setParent(NULL);
//                widget->deleteLater();
//            }
//        }
//        ButtonIngredientLayoutHash.remove(button);
//        if(layout)
//        {
//            layout->setParent(NULL);
//            layout->deleteLater();
//        }
//    };
//    ForEachInLayoutMap(DeleteFromIndex);
//    ButtonIngredientLayoutHash.clear();
//}

//void MainWindow::ClearButtonRecipeHash()
//{
//    auto DeleteFromIndex = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator &iteratorIndex){
//        QPushButton* button = iteratorIndex.key();
//        QLayout* layout = iteratorIndex.value();

//        DeleteLayout(layout);
//        delete button;

//    };

//    ForEachInRecipeHash(DeleteFromIndex);
//    ButtonRecipeLayoutHash.clear();
//}



//void MainWindow::PrepareComboBox(QComboBox* comboBox)
//{
//    comboBox->addItem("g");
//    comboBox->addItem("kg");
//    comboBox->addItem("l");
//    comboBox->addItem("ml");
//    comboBox->addItem("szt");
//}

//void MainWindow::DeleteIngredient()
//{
//    QPushButton* button = qobject_cast<QPushButton*>(sender());
//    QHBoxLayout* layout = ButtonIngredientLayoutHash.take(button);

//    while (layout->count() != 0)
//    {
//        QWidget* widget = layout->takeAt(0)->widget();
//        if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
//            currentRecipe.DeleteIngredientByName(ingredientNameText->toPlainText());
//        if(widget){
//            widget->setParent(NULL);
//            widget->deleteLater();
//        }

//    }
//    ButtonIngredientLayoutHash.remove(button);
//    if(layout)
//    {
//        layout->setParent(NULL);
//        layout->deleteLater();
//    }
//}


//void MainWindow::DeleteLayout(QLayout* layout)
//{
//    ClearLayout(layout);
//    if(layout)
//    {
//        layout->setParent(NULL);
//        layout->deleteLater();
//    }
//}

//void MainWindow::ClearLayout(QLayout* layout)
//{
//    while (layout->count() != 0)
//    {
//       QWidget* widget = layout->takeAt(0)->widget();
//       if(QHBoxLayout* nestedLayout = qobject_cast<QHBoxLayout*>(widget))
//           ClearLayout(nestedLayout);

//       if(widget){
//           widget->setParent(NULL);
//           widget->deleteLater();
//       }
//    }
//}

//QString MainWindow::GetIngredientNameByButton(QPushButton* button)
//{
//    QHBoxLayout* layout = ButtonIngredientLayoutHash.value(button);

//    int index=0;
//    while (index<layout->count())
//    {
//        QWidget* widget = layout->itemAt(index)->widget();
//        if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
//            return ingredientNameText->toPlainText();
//        index++;
//    }
//    return "default";
//}

//void MainWindow::RefillRecipeLayout()
//{
//    ClearRecipeLayout();

//    auto AddLayout = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
//    {
//        AddLayoutFromMap(iteratorIndex);
//    };

//    ForEachInLayoutMap(AddLayout);
//}

//void MainWindow::ReloadCurrentRecipe()
//{
//    auto ReloadWidget = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
//    {
//        ReloadWidgetFromMap(iteratorIndex);
//    };

//    ForEachInLayoutMap(ReloadWidget);
//}

void MainWindow::LoadRecipeListToRecipeLayout()
{

    layoutManager->ClearAll();

    infoLabel->setText("Wybierz przepis:");
    RecipeFileManager LoadListManager;
    auto recipeNamesVector = LoadListManager.LoadRecipeNameList();

    layoutManager->PrintRecipeListFromNamesVector(recipeNamesVector);
//    int index = 0;
//    int endIndex = recipeNamesVector.count();

//    while(index<endIndex)
//    {
//        QLabel* selectableRecipeLabel = new QLabel();
//        selectableRecipeLabel->setText(recipeNamesVector[index]);
//        QPushButton* recipeChoiceButton = new QPushButton("Wybierz przepis");
//        QHBoxLayout* recipeChoiceLayout = new QHBoxLayout();
//        recipeChoiceLayout->addWidget(selectableRecipeLabel);
//        recipeChoiceLayout->addWidget(recipeChoiceButton);
//        recipeLayout->addLayout(recipeChoiceLayout);
//        QObject::connect(recipeChoiceButton, &QPushButton::clicked, this, &MainWindow::LoadRecipe);
//        ButtonRecipeLayoutHash.insert(recipeChoiceButton, recipeChoiceLayout);
//        index++;
//    }
}

void MainWindow::ClearRecipeLayout()
{
   layoutManager->ClearLayout(recipeLayout);
}

Unit MainWindow::GetUnitFromCBox(QComboBox* comboBox)
{
    int index=comboBox->currentIndex();
    return Recipe::GetUnitFromInt(index);
}

//void MainWindow::LoadRecipe()
//{

//    QPushButton* button = qobject_cast<QPushButton*>(sender());

//    QString name = layoutManager->FindRecipeNameByButton(button);

//    QString fileName = name+".json";

//    QFile recipeFile(fileName);

//    RecipeFileManager LoadRecipeManager;
//    LoadRecipeManager.OpenFileReadWrite(recipeFile);
//    QJsonObject jsonRecipe;

//    jsonRecipe=LoadRecipeManager.GetJsonObjectFromFile(recipeFile);

//    currentRecipe->ReadFromJson(jsonRecipe);

//    infoLabel->setText(currentRecipe->GetName());

//    layoutManager->ClearAll();

//    layoutManager->IngredientVectorToRecipeLayout();
//}

void MainWindow::SaveRecipe()
{
    RecipeFileManager saveFileManager;
    QJsonArray RecipeNameArray;

    saveFileManager.FileToQJsonArray(recipeListFileName, RecipeNameArray);

    saveFileManager.AddToRecipeNameList(currentRecipe->GetName());
    saveFileManager.RecipeNameListToQJsonArray(RecipeNameArray);

    saveFileManager.QJsonArrayToRecipeNameList(RecipeNameArray);

    saveFileManager.SaveQJsonArrayToFile(recipeListFileName, RecipeNameArray);
    saveFileManager.SaveRecipeToFile(currentRecipe);

}

void MainWindow::SetRecipeWidgets()
{
    recipeUnitCBox->setCurrentIndex(int(currentRecipe->GetUnit()));
    recipeUnitCBox->setDisabled(true);

    recipeAmountSpinBox->setValue(currentRecipe->GetAmount());

    recipeNameText->setText(currentRecipe->GetName());
}

void MainWindow::RevertRecalculation()
{
    currentRecipe->RevertToOriginalAmount();
    layoutManager->ReloadCurrentRecipe();
}

//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);

//    recipeLayout = qobject_cast<QVBoxLayout*>(ui->recipeArea->layout());
//    recipeUnitCBox = qobject_cast<QComboBox*>(ui->recipeUnitCBox);
//    PrepareComboBox(recipeUnitCBox);
//    recipeAmountSpinBox=qobject_cast<QSpinBox*>(ui->recipeAmountSpinBox);
//    recipeNameText=qobject_cast<QTextEdit*>(ui->recipeNameText);

//    SetRecipeWidgets();

//    ingredientAmountSpinBox=qobject_cast<QSpinBox*>(ui->ingredientAmountSpinBox);
//    ingredientNameText=qobject_cast<QTextEdit*>(ui->ingredientNameText);
//    ingredientUnitComboBox = qobject_cast<QComboBox*>(ui->ingredientUnitComboBox);
//    PrepareComboBox(ingredientUnitComboBox);

//    recalculateSpinBox=qobject_cast<QSpinBox*>(ui->recalculateSpinBox);
//    recalculateUnitCBox=qobject_cast<QComboBox*>(ui->recalculateUnitCBox);
//    PrepareComboBox(recalculateUnitCBox);
//    recalculateUnitCBox->setCurrentIndex(int(currentRecipe.GetUnit()));
//    recalculateUnitCBox->setDisabled(true);

//    newRecipeUnitCBox= qobject_cast<QComboBox*>(ui->newRecipeUnitCBox);
//    PrepareComboBox(newRecipeUnitCBox);
//    newRecipeAmountSpinBox=qobject_cast<QSpinBox*>(ui->newRecipeSpinBox);
//    newRecipeNameText=qobject_cast<QTextEdit*>(ui->newRecipeNameText);

//    infoLabel = qobject_cast<QLabel*>(ui->infoLabel);
//    infoLabel->setText(currentRecipe.GetName());

//    recipeListFileName="RecipeList.json";
//}

//MainWindow::~MainWindow()
//{
//    auto RemoveLayout = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
//    {
//        DeleteLayout(iteratorIndex.value());
//    };
//    ForEachInLayoutMap(RemoveLayout);

//    ButtonIngredientLayoutHash.clear();
//    currentRecipe.ClearIngredientsList();

//    delete ui;
//    delete ingredientUnitComboBox;
//    delete ingredientAmountSpinBox;
//    delete ingredientNameText;
//    delete recipeAmountSpinBox;
//    delete recipeNameText;
//    delete recipeUnitCBox;
//    delete newRecipeUnitCBox;

//    delete recipeLayout;
//    delete recalculateSpinBox;
//    delete recalculateUnitCBox;
//}


//void MainWindow::on_addIngredient_clicked()
//{
//    ReloadCurrentRecipe();

//    QString ingredientName = ingredientNameText->toPlainText();
//    bool ingredientAlreadyIn = currentRecipe.IsIngredientInRecipe(ingredientName);
//    if(ingredientAlreadyIn)
//    {
//        ingredientName = ingredientName + QString::number((ButtonIngredientLayoutHash.count()));
//    }

//    int ingredientAmount = ingredientAmountSpinBox->value();
//    Unit ingredientUnit=Recipe::GetUnitFromInt(ingredientUnitComboBox->currentIndex());

//    currentRecipe.AddIngredient(ingredientName, ingredientAmount, ingredientUnit);
//    AddLayoutFromIngredient((currentRecipe.GetIngredientVector()).constLast());
//}

//void MainWindow::on_ExitButton_clicked()
//{
//    QCoreApplication::quit();
//}

//void MainWindow::on_recalculateButton_clicked()
//{
//    currentRecipe.ReloadIngredientsInMap(ButtonIngredientLayoutHash);
//    currentRecipe.RecalculateRecipe(recalculateSpinBox->value());
//    ReloadCurrentRecipe();
//    RefillRecipeLayout();
//    currentRecipe.RevertToOriginalAmount();
//}

//void MainWindow::on_saveButton_clicked()
//{
//    SaveRecipe();
//}

//void MainWindow::on_LoadButton_clicked()
//{
//    LoadRecipeListToRecipeLayout();
//}

//void MainWindow::on_NewRecipeButton_clicked()
//{
//    Recipe r(newRecipeAmountSpinBox->value(), newRecipeNameText->toPlainText(), GetUnitFromCBox(newRecipeUnitCBox));
//    currentRecipe.ClearRecipe();
//    currentRecipe=r;
//    SetRecipeWidgets();
//    ClearRecipeLayout();
//    ButtonIngredientLayoutHash.clear();
//    infoLabel->setText(currentRecipe.GetName());
//}
