#pragma once

#ifndef FASTENERCALCULATIONPARAMETERS_H
#define FASTENERCALCULATIONPARAMETERS_H

#include <QMouseEvent>
#include <QString>

#include "ui_settingfastenercalculationparameters.h"

// Класс реализация меню настройки расчета болтового крепежа.

class FastenerCalculationParameters : public QWidget { // Окно выбора метиза

    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

 public:
    explicit FastenerCalculationParameters();
    ~FastenerCalculationParameters();

     QPoint previousPosition();
     void showWidget();

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);

    void changedButton(QPushButton* active_button, QPushButton* passively_button); // Метод реализации переключения кнопок,
    // путем измeнения стиля.

    Ui::Form *ui = nullptr;
    QPoint mPreviousPosition;

    int m_type_fasteners_   = NULL; // Тип крепежа что подлежит расчету.
    int m_type_calculation_ = NULL; // Тип расчета (Подбор-Расчет).

    QString m_result_setting_[2] // Значения по умолчанию.
    {
        "Метрический крепеж",
        "Расчет"
    };

 signals:
   void previousPositionChanged(QPoint previousPosition);
   void settings(int type_fasteners, int type_calculation, QString result_settings); // Сигнал результата настройки
   // что передаються на главное окно расчета.

 public slots:
   void setPreviousPosition(QPoint previousPosition);
   void closeSettings(); // Релазицая слота закрытия окна. Метод посылает сигнал о результате выставленных настройках.
};

#endif // FASTENERCALCULATIONPARAMETERS_H
