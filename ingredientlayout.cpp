#include "ingredientlayout.h"

IngredientLayout::IngredientLayout(Ingredient ingredient)
{
    ingredientNameTextEdit=new QTextEdit(ingredient.GetName());
    ingredientNameTextEdit->setFixedSize(100,30);
    ingredientNameTextEdit->setDisabled(true);

    ingredientAmountSpinBox=new QSpinBox();
    ingredientAmountSpinBox->setValue(ingredient.GetAmount());
    ingredientAmountSpinBox->setDisabled(true);

    ingredientUnitComboBox=new QComboBox();
    ingredientUnitComboBox->setCurrentIndex(int(ingredient.GetUnit()));
    ingredientUnitComboBox->setDisabled(true);

    deleteIngredientButton = new QPushButton("Usuń składnik");

    this->addWidget(ingredientNameTextEdit);
    this->addWidget(ingredientAmountSpinBox);
    this->addWidget(ingredientUnitComboBox);
    this->addWidget(deleteIngredientButton);

}

IngredientLayout::~IngredientLayout()
{
//    delete deleteIngredientButton;
//    delete ingredientNameTextEdit;
//    delete ingredientAmountSpinBox;
//    delete ingredientUnitComboBox;

}
