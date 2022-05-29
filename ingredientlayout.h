#ifndef INGREDIENTLAYOUT_H
#define INGREDIENTLAYOUT_H

#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include "ingredient.h"

class IngredientLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    IngredientLayout()=delete;
    IngredientLayout(Ingredient ingredient);
    ~IngredientLayout();


    QLayout* parentLayout;
    QPushButton* deleteIngredientButton;
    QTextEdit* ingredientNameTextEdit;
    QSpinBox* ingredientAmountSpinBox;
    QComboBox* ingredientUnitComboBox;

};

#endif // INGREDIENTLAYOUT_H
