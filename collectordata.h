#pragma once

#ifndef COLLECTORDATA
#define COLLECTORDATA

#define PI 3.14

#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <string>

// Перечисление используеться в качестве ключа для записи данных в класс CollectorData (Сборщик данных).

namespace Data {

    enum class BeamCalculation {

        load_F_,                         // Индекс нагрузки на балку
        scheme_option_index_,            // Индекс схемы балки. Одна опора, две опоры и т.д.
        scheme_L1_,                      // Индекс размера L1
        scheme_L2_,                      // Индекс размера L2
        material_index_,                 // Индекс применяемого материала
        beam_load_index_,                // Индекс типа нагрузки. Изгиб, срез и т.д.
        section_beam_index_,             // Индекс сечения балки. Квадратная труба, прямоугольная труба, и т.д.
        standart_profile_Ibeam_index_,   // Индекс перечня стандартных профилей двутавра.
        standart_profile_channel_index_, // Индекс перечня стандартных профилей швеллера.
        standart_profile_corner_index_,  // Индекс перечня стандартных профилей уголка.
        size_B_,                         // Индекс размера В сечения балки
        size_H_,                         // Индекс размера Н сечения балки
        size_S_,                         // Индекс размера S сечения балки
        load_X_index_,                   // Индекс нагрузки сечения по оси Х
        load_Y_index_,                   // Индекс нагрузки сечения по оси Y

        end_enum_flags_                  // Количетво значений в контейнере
    };

    enum class BoltCalculation {

        calculation_bolted_connection_,  // Индекс расчета болтового соединения (подбор / расчет).
        load_bolt_connectin_,            // Индекс нагрузки на болтовое соединение (нагрузка выражена в Н).
        strength_class_,                 // Индекс класса прочности болта (индекс) .
        number_of_bolts_connection_,     // Индекс количества болтов в соединении (количество шт).
        bolt_M_diametr_index_,           // Инедкс диаметра крепежа (болта) выражен в мм.
        bolt_G_diametr_index_,           // Индекс диаметра дюймовой резьбы выражен в мм (через мэп или же взят из QComboBox).
        bolt_G_index,                    // Индекс массива дюймовой резьбы.
        bolt_load_index_,                // Индекс типа нагрузки на болт (на срез == 1, на разрыв == 0).
        step_,                           // Шаг крепежа (конвертирован через бинарное дерево - индекс).
        length_female_screw_,            // Индекс длины гайки (в мм).
        margin_of_safary_,               // Запас прочности соединения.
        changes_diamter_factor_,         // Индекс изменения коэфициента рабочего диаметра (false = 0, true = 1).
        type_fasteners_,                 // Тип соединения (метрический крпеж = 0, дюймовый = 1).
        out_of_reach_,                   // Индекс выхода результата расчетов за пределы стандартных размеров (0 - false; 1 - true;)

        end_enum_flags_                  // Количетво значений в контейнере
    };

 enum class TractionCalculation {

        paw_width_,                   // Ширина лапы, мм. 
        number_of_paws_,              // Количество лап.
        diametr_disc_,                // Диаметр дисков, мм.
        number_of_disc_,              // Количество дисков.
        implement_working_width_,     // Ширина захвата агрегата.
        flag_roller_,                 // Наличие  катка (1 - true, 0 - false).
        angle_of_attack_of_the_disc_, // Угол атаки диска.
        total_weight_of_the_unit_,    // Общей вес агрегата, кг.
        maximum_working_speed_,       // Максимальная рабочая скорость, км/ч.
        maximum_working_depth_,       // Максимальная глубина брабротки, мм.
        soil_type_,                   // Индекс типа почвы.
        calculation_type_index_,      // Инднекс типа расчета.

        end_enum_flags_               // Количетво значений в контейнере
    };
}

/*
Класс CollectorData записывает данные по индексу что ввел пользователь для хранения, обработки, и расчетов.

- метод dataCollection записывает данные по ключу с помощью перечисления.
- метод conclusionData предоставляет данные по ключу.
*/

template<typename capacity, typename data>
class CollectorData {
 public:
    CollectorData(capacity reserve);
    ~CollectorData();

    void dataCollection(capacity index, const data value);
    data conclusionData(capacity index);

 private:
    data* array_;
};

template<typename capacity, typename data>
CollectorData<capacity, data>::CollectorData(capacity reserve) {

    array_ = new data[static_cast<int>(reserve)];
    for (short x = 0; x < static_cast<int>(reserve); ++x) array_[x] = 0;
}

template <typename capacity, typename data>
CollectorData<capacity, data>::~CollectorData() {

    delete[] array_;
}

template <typename capacity, typename data>
void CollectorData<capacity, data>::dataCollection(capacity index, const data value) {

    this->array_[static_cast<int>(index)] = value;
}

template <typename capacity, typename data>
data CollectorData<capacity, data>::conclusionData(capacity index) {

    return this->array_[static_cast<int>(index)];
}

#endif
