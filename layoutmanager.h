#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H
#include <QHash>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <recipe.h>
#include "ingredientlayout.h"
#include "recipechoicelayout.h"


class LayoutManager: public QObject
{
    Q_OBJECT

public:
    LayoutManager()=delete;

    LayoutManager(Recipe* recipe, QVBoxLayout* layout, QLabel* label)
        : currentRecipe(recipe), mainLayout(layout), infoLabel(label) {};

    QHash<QPushButton*, QHBoxLayout*> ButtonIngredientLayoutHash;

    QHash<QPushButton*, QHBoxLayout*> ButtonRecipeLayoutHash;

    void AddIngredientLayout(Ingredient ingredient);

    void DeleteIngredient();

    void PrepareUnitComboBox(QComboBox* comboBox);

    QString FindRecipeNameByButton(QPushButton* button);

    void ClearLayout(QLayout* layout);

    void DeleteLayout(QLayout* layout);

    void ClearIngredientLayoutHash();

    void ClearRecipeLayoutHash();

    void ClearAll();

    void IngredientVectorToRecipeLayout();

    void RefillRecipeLayout();

    void ReloadCurrentRecipe();

    void PrintRecipeListFromNamesVector(QVector<QString>names);

private:

    Recipe* currentRecipe;

    QVBoxLayout* mainLayout;

    QLabel* infoLabel;



    void ForEachInHash(QHash<QPushButton*, QHBoxLayout*>&Hash, const std::function<void(QHash<QPushButton*, QHBoxLayout*>::const_iterator&)>& function);

    void AddLayoutFromMap(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex, QVBoxLayout* parentLayout);



    void IngredientToLayoutByIterator(QHash<QPushButton*, QHBoxLayout*>::const_iterator& iteratorIndex);



    QString GetIngredientNameByButton(QPushButton* button);

    void LoadRecipe();






};

#endif // LAYOUTMANAGER_H
