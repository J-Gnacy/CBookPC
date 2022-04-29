#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include "ingredient.h"
#include <functional>
#include "QObject"
#include <vector>
#include <QPushButton>
#include <QMap>

class Recipe
{
private:

    QMap<QPushButton*, Ingredient*> IngredientList;

    const float recipeProductAmount;

    float recipeDesiredAmount;

    QString recipeName;

    Unit recipeUnit;



public:

    Recipe(float desiredAmount, QString desiredName, Unit desiredUnit)
        :recipeProductAmount(desiredAmount), recipeDesiredAmount(desiredAmount), recipeName(desiredName), recipeUnit(desiredUnit){};

    Recipe& operator=(const Recipe& other);

    Recipe();

    void AddIngredient(QString name, float amount, Unit unitUsed, QPushButton* key);

    void ForEachIngredient(const std::function<void(Ingredient*)>& func);

    void DeleteIngredient(Ingredient* ingredient);

    void DeleteIngredientByKey(QPushButton* key);

    void RecalculateRecipe(float newAmount);

    float GetAmount();

    QString GetName();

    Unit GetUnit();

    QMap<QPushButton*, Ingredient*> GetIngredientList();

    ~Recipe();
};

#endif // RECIPE_H
