#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "recipe.h"
#include "ingredient.h"
#include <vector>
#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addIngredient_clicked();

    void on_ExitButton_clicked();

    void on_recalculateButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QPushButton*, QHBoxLayout*> deleteButtonToIngredientLayoutMap;
    QComboBox* ingredientUnitComboBox=nullptr;
    QSpinBox* ingredientAmountSpinBox=nullptr;
    QTextEdit* ingredientNameText=nullptr;
    QLabel* recipeAmountLabel=nullptr;
    QLabel* recipeNameLabel=nullptr;
    QComboBox* recipeUnitCBox=nullptr;
    QComboBox* newRecipeUnitCBox=nullptr;
    QScrollArea* scrollArea=nullptr;
    QVBoxLayout* recipeLayout=nullptr;
    QSpinBox* recalculateSpinBox=nullptr;
    QComboBox* recalculateUnitCBox=nullptr;

    void ForEachInLayoutMap(const std::function<void>& function);
    void PrepareComboBox(QComboBox* comboBox);
    void DeleteIngredient();
    void DeleteLayout(QLayout* layout);
    void ClearLayout(QLayout* layout);
    void RefillRecipeLayout();
    Unit GetUnitFromCBox(QComboBox* comboBox);
    void RefreshRecipeLayout();
    void ReloadCurrentRecipe();

};
#endif // MAINWINDOW_H
