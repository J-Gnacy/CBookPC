#include "recipe.h"

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

QMap<QPushButton*, Ingredient*> Recipe::GetIngredientList()
{
    return IngredientList;
}

Recipe::~Recipe()
{
    auto removeIngredients = [&](Ingredient* ingredient) {
        DeleteIngredient(ingredient);
    };

    ForEachIngredient(removeIngredients);

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

void Recipe::ReloadIngredientsInMap(QMap<QPushButton*, QHBoxLayout*> &OtherMap)
{


    QMap<QPushButton*, QHBoxLayout*>::const_iterator i = OtherMap.constBegin();
    QMap<QPushButton*, QHBoxLayout*>::const_iterator iteratorEnd = OtherMap.constEnd();
    while (i != iteratorEnd)
    {
      QHBoxLayout* ingredientLayout=new QHBoxLayout();
      QHBoxLayout* layout = i.value();
      int currentItemIndex=0;
      while(layout->count()>0)
      {
          QWidget* widget = layout->takeAt(0)->widget();

         /* if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget))
              spinBox->setValue(13);*/

          ingredientLayout->addWidget(widget);

          currentItemIndex++;
      }
      delete layout;
      OtherMap.insert(i.key(), ingredientLayout);
      i++;
    }
}

void Recipe::ClearIngredientsList()
{
    IngredientList.clear();
}

