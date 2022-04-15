#include "recipe.h"

void Recipe::AddIngredient(QString name, float amount, Unit unitUsed)
{

    Ingredient* newIngredient = new Ingredient(name, amount, unitUsed);
    IngredientList.push_back(newIngredient);


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
    for (int i = 0; i < IngredientList.size(); i++)
    {
        func(IngredientList[i]);
    }
}

float Recipe::GetAmount()
{
    return recipeDesiredAmount;
}

std::vector<Ingredient*> Recipe::GetIngredientList()
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

