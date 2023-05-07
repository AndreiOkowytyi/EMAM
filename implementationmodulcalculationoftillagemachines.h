#pragma once

#ifndef CALCULATIONOFTILLAGE_MACHINE
#define CALCULATIONOFTILLAGE_MACHINE

#include "collectordata.h"

template <typename T1, typename T2>
class SettlementPair { // Класс хранения двух значений, как правило наименьшего и наибольшего значения.
 public:
     SettlementPair() = default;
     SettlementPair(const T1 min_count, const T2 max_count);
     SettlementPair(const SettlementPair<T1, T2>& pair);
     SettlementPair<T1, T2> operator= (const SettlementPair<T1, T2>& pair);

     T1 minCount() { return this->min_count_; }
     T2 maxCount() { return this->max_count_; }

 private:
     T1 min_count_;
     T2 max_count_;
};

template <typename T1, typename T2> SettlementPair<T1, T2>::SettlementPair(const T1 min_count, const T2 max_count)
    : min_count_(min_count), max_count_(max_count) {}

template <typename T1, typename T2> SettlementPair<T1, T2>::SettlementPair(const SettlementPair<T1, T2>& pair) {

    this->min_count_ = pair.min_count_;
    this->max_count_ = pair.max_count_;
}

template <typename T1, typename T2> SettlementPair<T1, T2> SettlementPair<T1, T2>::operator=(const SettlementPair<T1, T2>& pair) {

    this->min_count_ = pair.min_count_;
    this->max_count_ = pair.max_count_;
    return *this;
}


class CalculationFaucet { // Класс содержит массив SettlementPair, в котором расположены результаты расчета (Сборщик результатов расчета).
 public:

    // С помощью TypePair осуществляеться как доступ к результатам mresultCalculationPair, так и их запись
    // dataInputPair.

    enum class TypePair {

        resistance_of_one_boom_share_for_tillage_, // Сопротивление одной стрельчастой лапы, Н.
        resistance_of_one_tillage_disc_,           // Сопротивление одного диска, Н.
        full_traction_resistance_,                 // Общее сопротивление агрегата в Н.
        power_KW_,                                 // Необходимая мощность кВт.
        horsepower_,                               // Мощность в лошадинных силах.

        count_                                     // Количество элементов в TypePair.
    };

    SettlementPair<int, int> resultCalculation(TypePair index);
    void dataInputPair(TypePair index, SettlementPair<int, int> pair);

 private:
    SettlementPair<int, int> mresultCalculationPair[static_cast<int>(TypePair::count_)];
};


class SoilCharacteristicsAndItsResistance { // Класс содержит данные сопротивляемости почвы.
 public:
     // Метод возвращает пару, в которой первое значение наименьшее сопротивление почвы заданной категории,
     // а также наибольшее.
     SettlementPair<int, int> soilResistance(CollectorData<Data::TractionCalculation, int>& collectordata);

 private:
     // Расчет категории почвы.
     void categoryDefinition(CollectorData<Data::TractionCalculation, int>& collectordata);

     short m_category_ = NULL;                // Категория почвы в зависимости от глубины обработки.
     const int min_earth_resistance_[3][3] {  // Наименьшее сопротивление.

         20000, 25000, 35000,
         35000, 40000, 50000,
         55000, 60000, 100000
     };
     const int max_earth_resistance_[3][3] {  // Наибольшее сопротивление.

         25000, 35000, 45000,
         55000, 60000, 85000,
         77000, 90000, 130000
     };
};


class TractionCategory { // Класс, определяет тяговую категории сцепного устройства в зависимости от мощности трактора.
 public:
     SettlementPair<std::string, std::string> requiredTractionCategory(const short powerInKW);

 private:
     void catygoriaDetermination(const short powerInKW);

     short m_index_traction_categoty_[2] {NULL, NULL};
     bool  m_flag_[2] {true, true};

     std::string mTractionCategoty[5] {"1", "1N", "2N/2", "3N/3", "4N/4"}; // Перечень тяговой категории трактора.

     const short m_pair_power_[5][2] {

         0,   30,       // Первая категория (1).
         15,  48,       // Категория 1N.
         30,  92,       // Категория 2N/2.
         60,  185,      // Категория 3N/3.
         110, SHRT_MAX  // Категория 4N/4.
     };
};


class BoomShareForTillage { // Расчет сопротивления одной стрельчастой лапы.
 public:
     SettlementPair<int, int> resistanceWorkinTool(CollectorData<Data::TractionCalculation,
         int>& collectordata, CalculationFaucet& resultСollector);
};

class TillageDisc { // Расчет сопротивления одного диска лущильника.
 public:
     SettlementPair<int, int> resistanceWorkinTool(CollectorData<Data::TractionCalculation,
         int>& collectordata, CalculationFaucet& resultСollector);
};


class SettlementBlock { // Расчетный блок сопротивляемости рабочих органов агрегата.
 public:
     virtual ~SettlementBlock() {}

     virtual SettlementPair<int, int>resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation,
                     int>& collectordata, CalculationFaucet& resultСollector) = 0;
};

class SettlementBlockPlowSweep : public SettlementBlock { // Расчет всех рабочих органов типа стрельчастая лапа культиватора.
 public:
     SettlementPair<int, int>resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation,
                     int>& collectordata, CalculationFaucet& resultСollector) override;
};

class SettlementBlockTillageDisc : public SettlementBlock { // Расчет всех рабочих органов типа диск лущильника.
 public:
     SettlementPair<int, int>resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation, int>& collectordata,
         CalculationFaucet& resultСollector) override;
};

class BlockPlowSweepTillageDisc : public SettlementBlock { // Комбинированный расчет рабочих органов агрегата, на котором
    // установлены как стрельчастые лапы, так и диски.
 public:
    SettlementPair<int, int>resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation, int>& collectordata,
        CalculationFaucet& resultСollector) override;
};


class TractionCalculationOfAllWorkingBodies { // Класс отвечает за общий тяговый расчет агрегата.
 public:
     TractionCalculationOfAllWorkingBodies();

     void tractionCalculation(CollectorData<Data::TractionCalculation, int>& collectordata, CalculationFaucet& resultСollector);

 private:
     const short m_count_settlement_block_ = 3;

     std::vector<std::unique_ptr<SettlementBlock>>mCalculationResistance;
};


class TractionForceCalculation { // Интерфейсный класс расчета тягового агрегата.
 public:
     void requiredPowerOfTheTractionUnit(CollectorData<Data::TractionCalculation, int>& collectordata,
         CalculationFaucet& resultСollector);
};


class Validation {
 public:
     // Проверка данных расчета культиватора.

     static bool validationPlowSweep(CollectorData<Data::TractionCalculation, int>& collectordata);

     // Проверка данных расчета бороны.

     static bool validationTillageDisc(CollectorData<Data::TractionCalculation, int>& collectordata);

     // Проверка данных комбинированного расчета.

     static bool validationPlowSweepTillageDisc(CollectorData<Data::TractionCalculation, int>& collectordata);
};


class InputValidationCalculation { // Проверка данных что ввел пользователь.
 public:
    InputValidationCalculation();
    bool validation(CollectorData<Data::TractionCalculation, int>& collectordata);

 private:
    const short m_count_configuration = 3;
    std::vector<bool(*)(CollectorData<Data::TractionCalculation, int>& collectordata)>m_configuration_validation;
};

#endif // CALCULATIONOFTILLAGEMACHINE
