#include "recipechoicelayout.h"

RecipeChoiceLayout::RecipeChoiceLayout(QString name)
{
   recipeNameLabel = new QLabel(name);
   recipeChoiceButton = new QPushButton("Wybierz przepis");
   this->addWidget(recipeNameLabel);
   this->addWidget(recipeChoiceButton);
}
