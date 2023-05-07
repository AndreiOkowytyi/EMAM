#pragma once

#ifndef STANDARTPROFILEDATABASE
#define STANDARTPROFILEDATABASE

#include "collectordata.h"

// Функция проверки границ массива.

void arrayIndexCheck(const int index, const short size_array);

/*
Реализация базы данных стандартных профилей.
- методы get возвращает данные массива в зависимости от типа нагрузки по индексу.
*/

class StandartProfileDatabase {
 public:
    StandartProfileDatabase ();
    ~StandartProfileDatabase();

    int getProfileIBeam  (const int loadX, const int loadY, const int index);
    int getProfileChannel(const int loadX, const int loadY, const int index);
    int getProfileCorner (const int index);

 private:

    int* p_profile_ibeam_X_   = nullptr;   // База профилей двутавра по оси Х.
    int* p_profile_ibeam_Y_   = nullptr;   // База профилей двутавра по оси Y.
    int* p_profile_channel_X_ = nullptr;   // База профилей швеллера по оси Х.
    int* p_profile_channel_Y_ = nullptr;   // База профилей швеллера по оси Y.
    int* p_profile_corner_    = nullptr;   // Базa профилей уголка.

    // Размеры массивов.

    const short m_size_ibeam_  = 50;
    const short m_size_channel = 18;
    const short m_size_corner_ = 89;
};


class DatabaseCharacteristicsMaterial { // База данных характерстик материала.
 public:
    DatabaseCharacteristicsMaterial ();
    ~DatabaseCharacteristicsMaterial();

    int loadType(const int index_type_load, const int index_characteristics);

 private:
    std::vector<int(*)(DatabaseCharacteristicsMaterial*, const int)>variation_load_;

    int* p_bendin_characteristics_material_ = nullptr;
    int* p_shear_characteristics_material_  = nullptr;

    const short m_size_array_ = 8; // Размер массива.

    static int bendinCharacteristicsMaterial(DatabaseCharacteristicsMaterial* Object, const int index);
    static int shearCharacteristicsMaterial (DatabaseCharacteristicsMaterial* Object, const int index);
};


class ResistanceSection { // Абстрактный класс расчета сопротивления сечения профиля балки.
 public:
    virtual double calculationResistanceSection(CollectorData<Data::BeamCalculation, int> &Collector) = 0;

    virtual ~ResistanceSection() {}

 protected:
    void initializationSizeScheme(CollectorData<Data::BeamCalculation, int>& Collector);

    double m_scheme_size_B_ = 0;
    double m_scheme_size_H_ = 0;
    double m_scheme_size_S_ = 0;
};

class ResistanceBeamrRectangularPipe : public ResistanceSection { // Расчет сопротивления прямоугольной трубы сечения балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamSquarePipe : public ResistanceSection { // Расчет сопротивления квадратной трубы сечения балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamRoundPipe : public ResistanceSection { // Расчет сопротивления круглой трубы сечения балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamCircle : public ResistanceSection { // Расчет сопротивления круглого сечения балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamSquare : public ResistanceSection { // Расчет сопротивления квадратного сечения балки.
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamLeaf : public ResistanceSection { // Расчет полосы.
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamIBeam : public ResistanceSection { // Расчет сопротивления двутавра балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamChannel : public ResistanceSection { // Расчет сопротивления швеллера балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class ResistanceBeamCorner : public ResistanceSection { // Расчет сопротивления уголка балки
 public:
    double calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) override;
};


class ConfigurationResistanceSection { // Класс расчета сопротивления сечения.
 public:
    ConfigurationResistanceSection();

    // Метод configutationCalculationResistanceSection, в зависимости от индекса, производит вызов
    // необходимого полиморфного класса с помощью которого производит расчет сопротивления сечения, возвращая это значение.

    double configutationCalculationResistanceSection(const int index, CollectorData<Data::BeamCalculation, int>& Collector);

 private:
    std::vector<std::unique_ptr<ResistanceSection>>::iterator iterator();

    std::vector<std::unique_ptr<ResistanceSection>>configuration_resistance_section_;

    short       m_counter_  = 0;
    const short m_capacity_ = 9; // Размер вектора.
};

// Абстрактный класс расчета максимального момента в балке.

class SchemeBeamMomentCalculation {
 public:
    virtual double calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) = 0;

    virtual ~SchemeBeamMomentCalculation() {}

    double getReactionA(); // Метод возвращает результат реакции опоры А.
    double getReactionB(); // Метод возвращает результат реакции опоры В.

 protected:
    void initializationSizeL1L2LoadF(CollectorData<Data::BeamCalculation, int>& Collector);

    double m_load_meam_F_= 0;                    // Нагрузка на балку
    double m_reaction_A_ = 0, m_reaction_B_ = 0; // Реакция опоры А, В.
    double m_size_L1_    = 0, m_size_L2_    = 0; // Пролеты балки.
};

class BeamTwoSupports : public SchemeBeamMomentCalculation {
 public:
    double calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class BeamTwoSupportsCantileverLoad : public SchemeBeamMomentCalculation {
 public:
    double calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) override;
};

class BeamOneSupportsCantileverLoad : public SchemeBeamMomentCalculation {
 public:
    double calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) override;
};


class ConfigurationSchemeBeam { // Класс расчета максимального момента в балке.
 public:
    ConfigurationSchemeBeam();

    /*
    Метод configutationCalculationMaxMomentBeam с помощью индекса вызывает необходимый класс расчета максимального момента
    в балке, и записывает его в переменную max_reaction_load_, после происходит вызов getReactionA и getReactionB класса
    SchemeBeamMomentCalculation, данные которого записываються в переменные reaction_supports_A_, reaction_supports_В_
    данного класса.
    */

    double configutationCalculationMaxMomentBeam(const int index, CollectorData<Data::BeamCalculation, int>& Collector);

    double getReacionsSupportsA(); // Метод возвращает результат реакции опоры А.
    double getReacionsSupportsB(); // Метод возвращает результат реакции опоры В.

 private:
    std::vector<std::shared_ptr<SchemeBeamMomentCalculation>>configuration_scheme_beam_;

    std::vector<std::shared_ptr<SchemeBeamMomentCalculation>>::iterator iterator();

    short       m_counter_  = 0;
    const short m_capacity_ = 3; // Размеры вектора.

    double m_reaction_supports_A_ = 0, m_reaction_supports_B_ = 0;
    double m_max_reaction_load_   = 0;
};


class BeamStressCalculation { // Класс основной формулы расчета.
 public:
    BeamStressCalculation(CollectorData<Data::BeamCalculation, int>&Collector);

    double maxSressBeam     (); // Расчет, и возврат максимального напряжения в балке, выражено в МПА.
    double marginOfSafery   (); // Расчет, и возврат запаса прочности балки.
    double reactionSupportsA(); // Возврат реакции опоры А.
    double reactionSupportsB(); // Возврат реакции опоры В.

 private:
    CollectorData<Data::BeamCalculation, int> *CollectorDataBeam;

    ConfigurationSchemeBeam         SchemeBeam;
    ConfigurationResistanceSection  ResistanceSection;
    DatabaseCharacteristicsMaterial CharacteristicsMaterial;

    double m_max_stress_beam_ = 0; // Максимальное напряжения в балке, выражено в МПа.
};


class DataValidation { // Проверка данных на правильность ввода пользователем.
 public:
    DataValidation();

    bool operator() (CollectorData<Data::BeamCalculation, int>& Collector);

 private:
    enum class Flag {validation_scheme_beam_, validation_resistance_};

    std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>::iterator iterator(Flag index,
        std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>&container_);

    std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>validator_scheme_;
    std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>validator_resistance_;

    short m_counter_scheme_ = 0, m_counter_resistance_ = 0;

    // Размеры вектора.

    const short m_capacity_scheme_     = 3;
    const short m_capacity_resistance_ = 9;
};

class ValidationSchemeBeam {
 public:
    static bool validationBeamTwoSupports              (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных балки двух опор.
    static bool validationBeamTwoSupportsCantileverLoad(CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных балки двух опор, консоль.
    static bool validationBeamOneSupportsCantileverLoad(CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных консольной балки.
};

class ValidationResistanceBeam {
 public:
    static bool validationResistanceBeamrRectangularPipe(CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных прямоугольной трубы
    static bool validationResistanceBeamSquarePipe      (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных квадратной трубы
    static bool validationResistanceBeamRoundPipe       (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных круглой трубы
    static bool validationResistanceBeamCircle          (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных круга
    static bool validationResistanceBeamSquare          (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных квадрата
    static bool validationResistanceBeamLeaf            (CollectorData<Data::BeamCalculation, int>& Collector); // Проверка данных полосы
};

#endif
