#include "recipefilemanager.h"
#include <QJsonDocument>


RecipeFileManager::RecipeFileManager()
{

}

void RecipeFileManager::OpenFileReadWrite(QFile &file)
{
    if(!file.open(QIODevice::ReadWrite))
    {
        qCritical()<<file.errorString();
    }
}

QString RecipeFileManager::ReadFromFile(QString filename)
{
    QFile file(filename);

    if(!file.exists())
        return "File "+filename+" doesn't exist\n";

    OpenFileReadWrite(file);
    QTextStream stream(&file);
    QString fileText=stream.readAll();
    file.close();
    return fileText;
}

template<typename saved>
void RecipeFileManager::SaveToFile(QString filename, saved text)
{
    QFile file(filename);
    OpenFileReadWrite(file);
    QTextStream stream(&file);
    stream<<text;
    file.close();
}

template<typename saved>
void RecipeFileManager::AddToFile(QString filename, saved text)
{
    QFile file(filename);
    if(!file.exists())
        SaveToFile(filename, text);
    else
    {
        OpenFileReadWrite(file);
        QTextStream stream(&file);
        QString fileText=stream.readAll();
        stream<<fileText<<"\n"<<text;
    }

    file.close();
}

void RecipeFileManager::ClearRecipeNameList()
{
    RecipeNameList.clear();
}

void RecipeFileManager::AddToRecipeNameList(QString name)
{
    RecipeNameList.append(name);
}

QJsonObject RecipeFileManager::GetJsonObjectFromFile(QFile &file)
{
    QByteArray load;
    load=file.readAll();
    QJsonDocument jsonRecipeDoc=(QJsonDocument::fromJson(load));
    return jsonRecipeDoc.object();
}

void RecipeFileManager::QJsonArrayToRecipeNameList(QJsonArray RecipeNameArray)
{
    for(int recipeIndex=0; recipeIndex < RecipeNameArray.size(); recipeIndex++)
    {
        QString name;
        QJsonObject objectWithName = RecipeNameArray[recipeIndex].toObject();
        if(objectWithName.contains("name") && objectWithName["name"].isString())
        {
            name = objectWithName["name"].toString();
        }

        bool containsName=RecipeNameList.contains(name);
        if(name!=""&&!containsName)
           RecipeNameList.append(name);
    }
}

bool RecipeFileManager::FileToQJsonArray(QString recipeListFileName, QJsonArray &RecipeNameArray)
{
    bool succesStatus=false;
    QFile recipeListFile(recipeListFileName);
    if(recipeListFile.exists())
    {
        OpenFileReadWrite(recipeListFile);
        QJsonObject jsonRecipeListObj=GetJsonObjectFromFile(recipeListFile);
        recipeListFile.close();
        bool recipeListInFile=jsonRecipeListObj.contains("RecipeList");
        if(recipeListInFile)
        {
            RecipeNameArray=jsonRecipeListObj["RecipeList"].toArray();
            succesStatus=true;
        }
    }
    return succesStatus;
}

void RecipeFileManager::RecipeNameListToQJsonArray(QJsonArray& RecipeNameArray)
{
    for (const QString &recipeName : RecipeNameList)
    {
        if(!IsQStringInQJsonArray(RecipeNameArray, recipeName))
        {
            AddNameToQJsonArray(RecipeNameArray, recipeName);
        }
    }
}

void RecipeFileManager::AddNameToQJsonArray(QJsonArray &RecipeNameArray, QString name)
{
    QJsonObject recipeHeaderJsonObject;
    recipeHeaderJsonObject["name"]=name;
    RecipeNameArray.append(recipeHeaderJsonObject);
}

bool RecipeFileManager::IsQStringInQJsonArray(QJsonArray &RecipeNameArray, QString name)
{
    int nameCounter=0;
    int counterBorder=RecipeNameArray.size();
    while(nameCounter<counterBorder)
    {
        QJsonObject nameObject=RecipeNameArray[nameCounter].toObject();
        QString searchedName = nameObject["name"].toString();
        if(name==searchedName)
        {
            return true;
        }
        nameCounter++;
    }
    return false;
}

void RecipeFileManager::SaveQJsonArrayToFile(QString filename, QJsonArray RecipeNameArray)
{
    QJsonObject jsonRecipeListObj;
    QJsonDocument jsonRecipeListDoc;
    jsonRecipeListObj["RecipeList"] = RecipeNameArray;
    jsonRecipeListDoc.setObject(jsonRecipeListObj);
    SaveToFile(filename, jsonRecipeListDoc.toJson(QJsonDocument::Indented));
}

void RecipeFileManager::SaveRecipeToFile(Recipe &recipe)
{
    QJsonObject jsonRecipeObj;
    QJsonDocument jsonRecipeDoc;
    recipe.WriteToJSon(jsonRecipeObj);
    jsonRecipeDoc.setObject(jsonRecipeObj);
    QString filename = recipe.GetName() + ".json";
    SaveToFile(filename, jsonRecipeDoc.toJson(QJsonDocument::Indented));
}

