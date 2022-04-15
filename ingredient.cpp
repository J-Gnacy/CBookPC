#include "ingredient.h"

QString Ingredient::GetName()
{
    return name;
}

Unit Ingredient::GetUnit()
{
    return usedUnit;
}

void Ingredient::SetAmount(float newAmount)
{
    amount = newAmount;
}

float Ingredient::GetAmount()
{
    return amount;
}
