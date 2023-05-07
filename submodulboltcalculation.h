#pragma once

#ifndef BOLTCALCULATION_H
#define BOLTCALCULATION_H

#include <QMouseEvent>
#include <QIntValidator>

#include "settingfastenercalculationparameters.h" // Реализация настройки параметров расчета крепежа.
#include "metricfastenerpitchparameters.h"        // Реализация установки нестандартного крепежа.
#include "implementationmodulboltcalculation.h"   // Реализация расчетного модуля крепежа.

namespace Ui {
class BoltCalculation;
}

class BoltCalculation : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

 public:
    explicit BoltCalculation(QWidget *parent = nullptr);
    ~BoltCalculation();

    QPoint previousPosition();

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);

    void workingWithMetricFastenerWidget(bool flag = true); // Управление видимостью виджетов отвечающих за метрический крепеж.
    void workingWithInchFastenerWidget  (bool flag = true); // Управление видимостью виджетов отвечающих за дюмовый крепеж.

    // Реализация перемещенния координат виджетов в зависимости от настройки самого расчета.
    void workingWithWidgetCoordinators  (short strength_class_y, short nut_length_y, short group_type_load_y, short margin_of_safety_y);
    void fillDatabase(); // Заполнение объекта сборщика CollectorData для проведения расчета.

    Ui::BoltCalculation *ui = nullptr;
    QPoint mPreviousPosition;

    QPixmap mImageScheme[2] { // Установка изображения.

        QPixmap(":/new/prefix1/image_module/bolted_connection.jpg"),
        QPixmap(":/new/prefix1/image_module/bolted_connection_01.jpg")
    };

    FastenerCalculationParameters* CalculationParameters = nullptr; // Указатель на объект параметров расчета.
    MetricFastenerPitchParameters* SettingsThreadPitch   = nullptr; // Указатель на объект настройки шага резьбы.

    QIntValidator mInputValidator; // Фильтрация ввода данных.

    // Конвертация данных дюймового крепежа (внешний диаметр) по индексу виджета QComboBox в целочисленные данные.
    std::vector<int>tool_to_convert_inches_to_millimeters {10, 13, 17, 21, 23, 26, 33, 42, 48, 60};

    // Конвертация данных дюймового крепежа (класс прочности) по индексу виджета QComboBox в целочисленные данные.
    std::vector<int>indicator_of_the_strength_class_of_inch_fasteners {5, 5, 5, 6, 7, 9, 10, 10};

    CollectorData<Data::BoltCalculation, int>* pDataBase = nullptr;

    int m_type_fasteners_                = 0; // Тип установленного крепежа.
    int m_calculation_bolted_connection_ = 0; // Тип соединения крепежа.
    int m_type_load_fasteners_           = 0; // Вспомогательная переменная для реализации расчетного индекса Data::BoltCalculation::calculation_bolted_connection.
    int m_bolt_load_index_               = 1; // Индекс типа нагрузки.
    int m_index_thread_pitch_            = 2; // Индекс типа шага резьбы.

 signals:
    void previousPositionChanged(QPoint previousPosition);
    void previousShowForm(int,int); // При возврате в главное меню, окно расчета закрываеться,
    // а его координаты передаються главному меню.

 private slots:
    void setPreviousPosition (QPoint previousPosition);

    // Установка виджетов, их расположение, видимость, в зависимости выставлненных настроек через объект CalculationParameters.
    void calculationSetupResult(int type_fasteners, int type_calculation, QString result_settings);

    // Установка необходимого шага в зависимости от объекта MetricFastenerPitchParameters.
    void threadPitchSetting    (QString thread_pitch, const int index_thread_pitch);
    void fastenerCalculation(); // Слот реализации расчета.
    void clearFields();         // Слот реализации очистки всех полей ввода данных.
};

#endif // BOLTCALCULATION_H
