#include "layoutmanager.h"
#include <QPushButton>
#include <QLabel>
#include "recipefilemanager.h"
#include <QFile>

void LayoutManager::AddIngredientLayout(Ingredient ingredient)
{
    IngredientLayout* ingredientLayout = new IngredientLayout(ingredient);
    PrepareUnitComboBox(ingredientLayout->ingredientUnitComboBox);
    ButtonIngredientLayoutHash.insert(ingredientLayout->deleteIngredientButton, ingredientLayout);
    QObject::connect(ingredientLayout->deleteIngredientButton, &QPushButton::clicked, this, &LayoutManager::DeleteIngredient);

    currentRecipe->AddIngredient(ingredient.GetName(), ingredient.GetAmount(), ingredient.GetUnit());

    mainLayout->addLayout(ingredientLayout);
}

void LayoutManager::DeleteIngredient()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QHBoxLayout* layout = ButtonIngredientLayoutHash.take(button);

    while (layout->count() != 0)
    {
        QWidget* widget = layout->takeAt(0)->widget();
        if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
            currentRecipe->DeleteIngredientByName(ingredientNameText->toPlainText());
        if(widget){
            widget->setParent(NULL);
            widget->deleteLater();
        }
    }
    ButtonIngredientLayoutHash.remove(button);
    if(layout)
    {
        layout->setParent(NULL);
        layout->deleteLater();
    }
}

void LayoutManager::PrepareUnitComboBox(QComboBox* comboBox)
{
    comboBox->addItem("g");
    comboBox->addItem("kg");
    comboBox->addItem("l");
    comboBox->addItem("ml");
    comboBox->addItem("szt");
}

void LayoutManager::ForEachInHash(QHash<QPushButton*, QHBoxLayout*>&Hash, const std::function<void(QHash<QPushButton*, QHBoxLayout*>::const_iterator&)>& function)
{
    QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = Hash.constBegin();
    QHash<QPushButton*, QHBoxLayout*>::const_iterator endIterator = Hash.constEnd();
        while (iteratorIndex !=endIterator)
        {
          function(iteratorIndex);
          iteratorIndex++;
        }
}

void LayoutManager::AddLayoutFromMap(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex, QVBoxLayout* parentLayout)
{
      QLayout* layout = iteratorIndex.value();
      parentLayout->addLayout(layout);
}

void LayoutManager::IngredientToLayoutByIterator(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex)
{
     QLayout* layout = iteratorIndex.value();
     int layoutItemCount = layout->count();
     int layoutItemIndex=0;

     QString name = GetIngredientNameByButton(iteratorIndex.key());
     int ingredientIndex = currentRecipe->SearchIngredientByName(name);
     auto ingredientVector = currentRecipe->GetIngredientVector();
     Ingredient ingredient = ingredientVector[ingredientIndex];

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

void LayoutManager::IngredientVectorToRecipeLayout()
{
    QVector<Ingredient*>ingredientVector = currentRecipe->GetIngredientVector();
    int endIndex = ingredientVector.count();
    int index = 0;
    while(index<endIndex)
    {
        AddIngredientLayout(ingredientVector[index]);
        index++;
    }
}

QString LayoutManager::GetIngredientNameByButton(QPushButton* button)
{
    QHBoxLayout* layout = ButtonIngredientLayoutHash.value(button);

    int index=0;
    while (index<layout->count())
    {
        QWidget* widget = layout->itemAt(index)->widget();
        if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
            return ingredientNameText->toPlainText();
        index++;
    }
    return "default";
}


void LayoutManager::ClearLayout(QLayout* layout)
{
    while (layout->count() != 0)
    {
       QWidget* widget = layout->takeAt(0)->widget();
       if(QHBoxLayout* nestedLayout = qobject_cast<QHBoxLayout*>(widget))
           ClearLayout(nestedLayout);

       if(widget){
           widget->setParent(NULL);
           widget->deleteLater();
       }
    }
}

void LayoutManager::DeleteLayout(QLayout* layout)
{
    ClearLayout(layout);
    if(layout)
    {
        layout->setParent(NULL);
        layout->deleteLater();
    }
}

void LayoutManager::ClearIngredientLayoutHash()
{
    auto DeleteFromIndex = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator &iteratorIndex){
        QPushButton* button = iteratorIndex.key();
        QLayout* layout = iteratorIndex.value();

        while (layout->count() != 0)
        {
            QWidget* widget = layout->takeAt(0)->widget();
            if(QTextEdit* ingredientNameText = qobject_cast<QTextEdit*>(widget))
                currentRecipe->DeleteIngredientByName(ingredientNameText->toPlainText());
            if(widget)
            {
                widget->setParent(NULL);
                widget->deleteLater();
            }
        }
        ButtonIngredientLayoutHash.remove(button);
        if(layout)
        {
            layout->setParent(NULL);
            layout->deleteLater();
        }
    };
    ForEachInHash(ButtonIngredientLayoutHash, DeleteFromIndex);
    ButtonIngredientLayoutHash.clear();
}

void LayoutManager::ClearRecipeLayoutHash()
{
    auto DeleteFromIndex = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator &iteratorIndex){
        QPushButton* button = iteratorIndex.key();
        QLayout* layout = iteratorIndex.value();

        DeleteLayout(layout);
        delete button;

    };

    ForEachInHash(ButtonRecipeLayoutHash, DeleteFromIndex);
    ButtonRecipeLayoutHash.clear();
}

void LayoutManager::ClearAll()
{
    ClearRecipeLayoutHash();
    ClearIngredientLayoutHash();
}

QString LayoutManager::FindRecipeNameByButton(QPushButton* button)
{
    QLayout* layout = ButtonRecipeLayoutHash.value(button);
    QString name;

    while(layout->count()>0)
    {
        QWidget* widget = layout->takeAt(0)->widget();
        if(QLabel* label = qobject_cast<QLabel*>(widget))
            name=label->text();
    }

    return name;
}

void LayoutManager::RefillRecipeLayout()
{
    ClearLayout(mainLayout);

    auto AddLayout = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
    {
        AddLayoutFromMap(iteratorIndex, mainLayout);
    };

    ForEachInHash(ButtonIngredientLayoutHash, AddLayout);
}

void LayoutManager::ReloadCurrentRecipe()
{
    auto ReloadWidget = [&](QHash<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex)
    {
        IngredientToLayoutByIterator(iteratorIndex);
    };

    ForEachInHash(ButtonIngredientLayoutHash, ReloadWidget);
}

void LayoutManager::PrintRecipeListFromNamesVector(QVector<QString>names)
{
    int index = 0;
    int endIndex = names.count();

    while(index<endIndex)
    {
        RecipeChoiceLayout* recipeChoiceLayout = new RecipeChoiceLayout(names[index]);
        mainLayout->addLayout(recipeChoiceLayout);
        QObject::connect(recipeChoiceLayout->recipeChoiceButton, &QPushButton::clicked, this, &LayoutManager::LoadRecipe);
        ButtonRecipeLayoutHash.insert(recipeChoiceLayout->recipeChoiceButton, recipeChoiceLayout);
        index++;
    }
}

void LayoutManager::LoadRecipe()
{

    QPushButton* button = qobject_cast<QPushButton*>(sender());

    QString name = FindRecipeNameByButton(button);

    QString fileName = name+".json";

    QFile recipeFile(fileName);

    RecipeFileManager LoadRecipeManager;
    LoadRecipeManager.OpenFileReadWrite(recipeFile);
    QJsonObject jsonRecipe;

    jsonRecipe=LoadRecipeManager.GetJsonObjectFromFile(recipeFile);

    currentRecipe->ReadFromJson(jsonRecipe);

    infoLabel->setText(currentRecipe->GetName());

    ClearAll();

    IngredientVectorToRecipeLayout();
}
