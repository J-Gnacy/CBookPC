#ifndef RECIPEFILEMANAGER_H
#define RECIPEFILEMANAGER_H

#include <QJsonObject>
#include "recipe.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>


class RecipeFileManager
{
public:
    RecipeFileManager();

    void OpenFileReadWrite(QFile &file);

    QString ReadFromFile(QString filename);

    template<typename saved>
    void SaveToFile(QString filename, saved text);

    template<typename saved>
    void AddToFile(QString filename, saved text);

    void ClearRecipeNameList();

    void AddToRecipeNameList(QString name);

    bool FileToQJsonArray(QString recipeListFileName, QJsonArray &RecipeNameArray);

    QJsonObject GetJsonObjectFromFile(QFile &file);

    void QJsonArrayToRecipeNameList(QJsonArray RecipeNameArray);

    QVector<Ingredient*> QJsonArrayToIngredientVector(QJsonArray ingredientArray);

    void RecipeNameListToQJsonArray(QJsonArray &RecipeNameArray);

    void AddNameToQJsonArray(QJsonArray &RecipeNameArray, QString name);

    bool IsQStringInQJsonArray(QJsonArray &RecipeNameArray, QString name);

    void SaveQJsonArrayToFile(QString filename, QJsonArray RecipeNameArray);

    void SaveRecipeToFile(Recipe* recipe);

    QVector<QString> GetRecipeNameList();

    QVector<QString> LoadRecipeNameList();


private:
    QVector<QString> RecipeNameList;

};

#endif // RECIPEFILEMANAGER_H
