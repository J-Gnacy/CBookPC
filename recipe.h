#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include "Ingredient.h"
#include <functional>
#include "QObject"
#include <vector>

class Recipe
{
private:

    std::vector<Ingredient*>IngredientList;

    const float recipeProductAmount;

    float recipeDesiredAmount;

    QString recipeName;

    Unit recipeUnit;

    void ForEachIngredient(const std::function<void(Ingredient*)>& func);

public:

    Recipe(float desiredAmount, QString desiredName, Unit desiredUnit)
        :recipeProductAmount(desiredAmount), recipeDesiredAmount(desiredAmount), recipeName(desiredName), recipeUnit(desiredUnit){};


    void AddIngredient(QString name, float amount, Unit unitUsed);


    void DeleteIngredient(Ingredient* ingredient);

    void RecalculateRecipe(float newAmount);

    float GetAmount();

    QString GetName();

    std::vector<Ingredient*> GetIngredientList();

    ~Recipe();
};

#endif // RECIPE_H
