#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMouseEvent>
#include <QMainWindow>
#include <QPushButton>

#include "callofcalculationmodules.h" // Подключение файла, для создания вычислительных модулей.

#define NOT_ACTIVATED 500

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPoint previousPosition();

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);

    // Видимость кнопок.
    void configurationVisibilityEnginModul(bool visibility);
    void configurationVisibilityAgroModul (bool visibility);

    // Выставление стиля кнопок по default.
    void defaultButtonEnginModul(); // Кнопки инженерного модуля.
    void defaultButtonAgroModul();  // Кнопки агро модуля.

    QPushButton *pagricultural_machiner_cultivators_;
    QPushButton *pagricultural_machiner_discharrow_;
    QPushButton *pagricultural_machiner_group_calculation_;

    Ui::MainWindow *ui;
    QPoint mPreviousPosition;

    // Индекс векторов, для вызова необходиомй функции.
    short m_index_modul_       = NOT_ACTIVATED;
    short m_index_calculation_ = NOT_ACTIVATED;

     CallModules callModules;

 signals:
    void previousPositionChanged(QPoint previousPosition);

 public slots:
    void setPreviousPosition(QPoint previousPosition);

 private slots:
    void activeConfigurationVisibilityEnginModul(); // Вызов элементов инженерного раздела.
    void activeConfigurationVisibilityAgroModul();  // Вызов элементов агро раздела.
    void callTheCalculationFunction();              // Вызов функции открытие окна необходимого расчетного модуля.
    void showThisForm(int x, int y);
};

#endif // MAINWINDOW_H
