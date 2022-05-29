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
#include <layoutmanager.h>

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

    void on_NewRecipeButton_clicked();

    void on_saveButton_clicked();

    void on_LoadButton_clicked();

private:
    Ui::MainWindow *ui;

    QScopedPointer<LayoutManager> layoutManager;

    QSharedPointer<Recipe> currentRecipe;
    QSharedPointer<QLabel> infoLabel;
    QSharedPointer<QVBoxLayout> recipeLayout;

    QScopedPointer<QComboBox> ingredientUnitComboBox;
    QScopedPointer<QSpinBox> ingredientAmountSpinBox;
    QScopedPointer<QTextEdit> ingredientNameText;

    QScopedPointer<QSpinBox> recipeAmountSpinBox;
    QScopedPointer<QTextEdit> recipeNameText;
    QScopedPointer<QComboBox> recipeUnitCBox;

    QScopedPointer<QComboBox> newRecipeUnitCBox;
    QScopedPointer<QSpinBox> newRecipeAmountSpinBox;
    QScopedPointer<QTextEdit> newRecipeNameText;


    QScopedPointer<QSpinBox> recalculateSpinBox;
    QScopedPointer<QComboBox> recalculateUnitCBox;

    QString recipeListFileName;

    void RevertRecalculation();
    void ClearRecipeLayout();
    Unit GetUnitFromCBox(QComboBox* comboBox);
    void LoadRecipeListToRecipeLayout();
    void LoadRecipe();

    void SaveRecipe();

    void SetRecipeWidgets();



    QString GetIngredientNameByButton(QPushButton* button);


};
#endif // MAINWINDOW_H
