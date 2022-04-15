#ifndef INGREDIENT_H
#define INGREDIENT_H
#include <iostream>
#include <QObject>


enum class Unit { g, kg, l, ml };

class Ingredient
{
private:
    friend class Recipe;

       QString name;
       float amount;
       Unit usedUnit;

       Ingredient(QString newName, float newAmount, Unit newUnit)
           : name(newName), amount(newAmount), usedUnit(newUnit){}

       Ingredient(Ingredient* ingredient)
           : name(ingredient->name), amount(ingredient->amount), usedUnit(ingredient->usedUnit){}

public:
    QString GetName();

    void SetAmount(float newAmount);

    float GetAmount();

    Unit GetUnit();
};

#endif // INGREDIENT_H
