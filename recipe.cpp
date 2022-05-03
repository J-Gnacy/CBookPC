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


void Recipe::ForEachIngredient(const std::function<void(Ingredient*)>& func)
{
    QMap<QPushButton*, Ingredient*>::const_iterator i = IngredientList.constBegin();
    while (i != IngredientList.constEnd())
    {
        func(i.value());
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

void Recipe::RefreshIngredients(QMap<QPushButton*, QHBoxLayout*> OtherMap)
{

    QPushButton keyButton;

    QMap<QPushButton*, QHBoxLayout*>::const_iterator i = OtherMap.constBegin();
    while (i != OtherMap.constEnd())
    {
      QHBoxLayout* layout = i.value();
      int currentItemIndex=0;
      while(currentItemIndex != layout->count())
      {
          QWidget* item = layout->takeAt(0)->widget();
          if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(item))
              spinBox->setValue(0);

          currentItemIndex++;
      }

        ++i;
    }


}

void Recipe::ClearIngredientsList()
{
    IngredientList.clear();
}

