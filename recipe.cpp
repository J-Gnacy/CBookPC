#include "recipe.h"
#include <QJsonObject>
#include <QJsonArray>

void Recipe::AddIngredient(QString name, float amount, Unit unitUsed)
{
    Ingredient* newIngredient = new Ingredient(name, amount, unitUsed);
    IngredientVector.push_back(newIngredient);
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
    for (int i = 0; i < IngredientVector.size(); i++)
    {
        func(IngredientVector[i]);
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

QVector<Ingredient*> Recipe::GetIngredientVector()
{
    return IngredientVector;
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

bool Recipe::IsIngredientInRecipe(QString name)
{
    for (int i = 0; i < IngredientVector.size(); i++)
    {
        if(name==IngredientVector[i]->GetName())
        {
            return true;
        }
    }
    return false;
}

int Recipe::SearchIngredientByName(QString name)
{
    for (int i = 0; i < IngredientVector.size(); i++)
    {
        if(name==IngredientVector[i]->GetName())
        {
            return i;
        }
    }
}

void Recipe::DeleteIngredientByName(QString name)
{

    if(IsIngredientInRecipe(name))
        IngredientVector.removeAt(SearchIngredientByName(name));

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
    IngredientVector.clear();
}

void Recipe::CopyIngredientsVector(QVector<Ingredient*> otherVector)
{
    ClearIngredientsList();    
    for (int i = 0; i < otherVector.size(); i++)
    {
        IngredientVector.push_back(otherVector[i]);
    }
}

void Recipe::ReadFromJson(const QJsonObject &json)
{
    if (json.contains("recipeName") && json["recipeName"].isString())
        recipeName = json["name"].toString();

    if (json.contains("recipeProductAmount") && json["recipeProductAmount"].isDouble())
        recipeProductAmount = json["level"].toInt();

    if(json.contains("recipeUnit") && json["recipeUnit"].isDouble())
    {
        int preconvertedUnit = json["recipeUnit"].toInt();
    }

    recipeDesiredAmount = recipeProductAmount;
}

void Recipe::WriteToJSon(QJsonObject &json)
{
    json["recipeName"] = recipeName;
    json["recipeProductAmount"] = recipeProductAmount;
    json["recipeDesiredAmount"] = recipeDesiredAmount;
    json["recipeUnit"] =  int(recipeUnit);

    QJsonArray ingredientArray;
    int count = IngredientVector.size();
    for(const Ingredient& ingredient : IngredientVector)
    {
        QJsonObject ingredientObject;

        ingredientObject["ingredientName"] = ingredient.name;
        ingredientObject["ingredientAmount"] = ingredient.amount;
        ingredientObject["ingredientUnit"] = int(ingredient.usedUnit);

        ingredientArray.append(ingredientObject);
    }
    json["ingredientsList"] = ingredientArray;
}
