#ifndef RECIPE_H
#define RECIPE_H

#include <iostream>
#include "ingredient.h"
#include <functional>
#include "QObject"
#include <vector>
#include <QPushButton>
#include <QMap>
#include <QSpinBox>
#include <QBoxLayout>
#include <QList>

class Recipe
{
private:

    QVector<Ingredient*> IngredientVector;

    float recipeProductAmount;

    float recipeDesiredAmount;

    QString recipeName;

    Unit recipeUnit;


public:

    Recipe(float desiredAmount, QString desiredName, Unit desiredUnit)
        :recipeProductAmount(desiredAmount), recipeDesiredAmount(desiredAmount), recipeName(desiredName), recipeUnit(desiredUnit){};

    Recipe& operator=(const Recipe& other)
    {
        CopyIngredientsVector(other.IngredientVector);
        recipeProductAmount=other.recipeProductAmount;
        recipeDesiredAmount=other.recipeProductAmount;
        recipeName=other.recipeName;
        recipeUnit=other.recipeUnit;

        return *this;
    };

    Recipe(Recipe& other)
        :recipeProductAmount(other.GetAmount()), recipeDesiredAmount(other.GetAmount()), recipeName(other.GetName()), recipeUnit(other.GetUnit()){};

    Recipe();

    void AddIngredient(QString name, float amount, Unit unitUsed);

    void ForEachIngredient(const std::function<void(Ingredient*)>& func);

    void DeleteIngredient(Ingredient* ingredient);

    bool IsIngredientInRecipe(QString name);

    int SearchIngredientByName(QString name);

    void DeleteIngredientByName(QString name);

    void RecalculateRecipe(float newAmount);

    void RevertToOriginalAmount();

    float GetAmount();

    void ClearRecipe();

    QString GetName();

    Unit GetUnit();

    static Unit GetUnitFromInt(int index);

    QVector<Ingredient*> GetIngredientVector();

    ~Recipe();

    void ReloadIngredientsInMap(QHash<QPushButton*, QHBoxLayout*> &otherMap);

    void ClearIngredientsList();

    void CopyIngredientsVector(QVector<Ingredient*> otherVector);

    void ReadFromJson(const QJsonObject &json);

    void WriteToJSon(QJsonObject &json);

};

#endif // RECIPE_H
