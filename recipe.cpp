#include "recipe.h"
#include <QJsonObject>

void Recipe::AddIngredient(QString name, float amount, Unit unitUsed, QPushButton* key)
{

    Ingredient* newIngredient = new Ingredient(name, amount, unitUsed);
    IngredientList.insert(key, newIngredient);


}
void Recipe::DeleteIngredient(Ingredient* ingredient)
{
    delete ingredient;
}

void Recipe::RecalculateRecipe(float newAmount)
{
    float ratio = recipeDesiredAmount / newAmount;
    recipeDesiredAmount = newAmount;
    float temporaryAmount;

    auto recalculateFormula = [&](Ingredient* ingredient) {
        temporaryAmount = ingredient->GetAmount();
        temporaryAmount = temporaryAmount / ratio;
        ingredient->SetAmount(temporaryAmount); };

    ForEachIngredient(recalculateFormula);
}

void Recipe::RevertToOriginalAmount()
{
    RecalculateRecipe(recipeProductAmount);
}

void Recipe::ForEachIngredient(const std::function<void(Ingredient*)>& func)
{
    QMap<QPushButton*, Ingredient*>::const_iterator iteratorIndex = IngredientList.constBegin();
    QMap<QPushButton*, Ingredient*>::const_iterator endIterator = IngredientList.constEnd();
    while (iteratorIndex != endIterator)
    {
        func(iteratorIndex.value());
        iteratorIndex++;
    }
}

float Recipe::GetAmount()
{
    return recipeDesiredAmount;
}

void Recipe::ClearRecipe()
{
    auto removeIngredient = [&](Ingredient* ingredient) {
        DeleteIngredient(ingredient);
    };

    ForEachIngredient(removeIngredient);
}

QMap<QPushButton*, Ingredient*> Recipe::GetIngredientList()
{
    return IngredientList;
}

Recipe::~Recipe()
{
    ClearRecipe();
}

QString Recipe::GetName()
{
    return recipeName;
}

Unit Recipe::GetUnit()
{
    return recipeUnit;
}

void Recipe::DeleteIngredientByKey(QPushButton* key)
{
    IngredientList.remove(key);
}

void Recipe::ReloadIngredientsInMap(QMap<QPushButton*, QHBoxLayout*> &otherMap)
{


    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorIndex = otherMap.constBegin();
    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorEnd = otherMap.constEnd();
    while (iteratorIndex != iteratorEnd)
    {
      QHBoxLayout* ingredientLayout=new QHBoxLayout();
      QHBoxLayout* layout = iteratorIndex.value();
      int currentItemIndex=0;
      while(layout->count()>0)
      {
          QWidget* widget = layout->takeAt(0)->widget();
          ingredientLayout->addWidget(widget);
          currentItemIndex++;
      }
      delete layout;
      otherMap.insert(iteratorIndex.key(), ingredientLayout);
      iteratorIndex++;
    }
}

void Recipe::ClearIngredientsList()
{
    IngredientList.clear();
}

void Recipe::CopyIngredientsList(QMap<QPushButton*, Ingredient*> otherList)
{
    ClearIngredientsList();

    QMap<QPushButton*, Ingredient*>::const_iterator iteratorIndex = otherList.constBegin();
    QMap<QPushButton*, Ingredient*>::const_iterator iteratorEnd = otherList.constEnd();
    while(iteratorIndex!=iteratorEnd)
    {
        IngredientList.insert(iteratorIndex.key(), iteratorIndex.value());
    }
}

void Recipe::ReadFromJson(const QJsonObject &json)
{
    if (json.contains("recipeName") && json["recipeName"].isString())
        recipeName = json["name"].toString();

    if (json.contains("recipeProductAmount") && json["recipeProductAmount"].isDouble())
        recipeProductAmount = json["level"].toInt();

    recipeDesiredAmount = recipeProductAmount;
}

void Recipe::WriteToJSon(QJsonObject &json)
{
    json["recipeName"] = recipeName;
    json["recipeProductAmount"] = recipeProductAmount;
    json["recipeDesiredAmount"] = recipeDesiredAmount;
}
