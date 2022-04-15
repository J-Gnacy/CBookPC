#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>

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

private:
    Ui::MainWindow *ui;
    QComboBox* ingredientUnitComboBox=nullptr;
    QSpinBox* ingredientAmountSpinBox=nullptr;
    QTextEdit* ingredientNameText=nullptr;
    QLabel* recipeAmountLabel=nullptr;
    QLabel* recipeNameLabel=nullptr;
    void PrepareComboBox(QComboBox* comboBox);
};
#endif // MAINWINDOW_H
