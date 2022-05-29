#ifndef RECIPECHOICELAYOUT_H
#define RECIPECHOICELAYOUT_H

#include <QHBoxLayout>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "recipe.h"

class RecipeChoiceLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    RecipeChoiceLayout()=delete;

    RecipeChoiceLayout(QString name);

    QLabel* recipeNameLabel;
    QPushButton* recipeChoiceButton;
};

#endif // RECIPECHOICELAYOUT_H
