#pragma once

#ifndef MODULBOLTCALCULATION
#define MODULBOLTCALCULATION

#define inch_fasteners_array_size_   10
#define metric_fasteners_array_size_ 16

#include <thread>
#include <algorithm>
#include <map>

#include "collectordata.h"

class WorkingDiametr {
 public:
    virtual ~WorkingDiametr() {}
    virtual double fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) = 0;;
	};

class WorkingDiametrMetric : public WorkingDiametr { // Расположение рабочих диаметров метрического крепежа.
 public:
    WorkingDiametrMetric();
    ~WorkingDiametrMetric();

    // Функция принимает аргументы, где первый индекс (обозначение болта, М6, М8, М10 и т.д.),
    // второй шаг резьбы, и в соответсвии данных возвращает рабочий диаметр крепежа.

    double fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) override;

 private:
    double* p_fastener_working_diametr_ = nullptr;
	};


class WorkingDiametrInch : public WorkingDiametr { // Расположение рабочих диаметров дюймового крепежа.
 public:
    WorkingDiametrInch();
    ~WorkingDiametrInch();

    // Функция принимает аргументы, где аргумент индекс массива рабочих диаметров.

    double fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) override;

    short size(); // Функция возвращает размер массива.

 private:
    double* p_fastener_working_diametr_ = nullptr;
};


class FactoryWorkingDiametr {
 public:
    virtual ~FactoryWorkingDiametr() {}
    virtual std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() = 0;
};

class FactoryWorkinDiametrMetric : public FactoryWorkingDiametr { // Создает экземпляр класса
//рабочих диаметров метрического крепежа.
 public:
    std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() override;
};

class FactoryWorkingDiametrInch : public FactoryWorkingDiametr {  // Создает экземпляр класса
//рабочих диаметров дюймового крепежа.
 public:
    std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() override;
};


class FasetenerStrengthClass { // Пределы текучести в зависимости от класса прочности крепежа.
 public:
    FasetenerStrengthClass();
    ~FasetenerStrengthClass();

    // Функция возращает допускаемый предел текучести для заданного крепежа согласно индексу.

    short tieldStrengthFastener(const int index_type_load, const int strength_class);

 private:
    short* p_yield_strength_ = nullptr;
    const short m_capacity_ = 11; // Размер массива.
};


class CalculationOfStandardFasteners { // Расчет стнадратного крепежа.
 public:
    double calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);
};


class ChoiceOfStandardFasteners { // Подбор стандартного крепежа.
 public:
    double calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
                                                          FasetenerStrengthClass& FastenersStrength);

 public:
    double changesDiametrFactor() { return m_diamtr_factor_ += 0.25; } // Функция изменения коэффициента подбора рабочего диаметра крепежа.

    double m_diamtr_factor_ = 1; // Коэффициент подбора диаметра крепежа
};


class CalculationOfNonStandardThreadedFasteners {
 protected:
    const double m_thread_work_factor_ = 0.65; // Вспомогательный коэффициент.

    double threadSafetyMargin(short tield_strength_fastener, double result_fasteninf_calculation); // Расчет запаса прочности.
};

class ShearFasteningCalculation : public CalculationOfNonStandardThreadedFasteners { // Расчет на срез резьбы.
 public:
    double calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);
};

class CrushingFasteningCalculation : public CalculationOfNonStandardThreadedFasteners { // Расчет на смятие резьбы.
 public:
    CrushingFasteningCalculation();
    ~CrushingFasteningCalculation();

    double calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);

 private:
    short numberOfWorkersCoil(CollectorData<Data::BoltCalculation, int>& Collector); // Количество рабочих витков крепежа.
    int indexDiametrFasteners(CollectorData<Data::BoltCalculation, int>& Collector); // Возвращает внешний диаметр крепежа.

    double* p_list_of_metric_thread_pitches = nullptr; // Шаг метрического крепежа в мм.
    double* p_list_of_inch_thread_pitches   = nullptr; // Шаг дюймового крепежа в мм.
};


class FastenerCalculationConfiguration {
 public:
    FastenerCalculationConfiguration();
    virtual ~FastenerCalculationConfiguration() {}
    virtual double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) = 0;

    std::string inchThreadcalculationresult(CollectorData<Data::BoltCalculation, int>& Collector); // Метод возвращает
    // размер дюймового крепежа после проведения подбора.

 protected:
    // Метод реализации подбора необходимого крепежа согласно расчетa.

    template<typename T>
    void searchForTheDesiredValueMetric(T&& metric_size_, const short search_terms, CollectorData<Data::BoltCalculation,
                                        int>& Collector);  // Для метрического крепежа.

    template<typename T>
    void searchForTheDesiredValueInch(T&& inch_size_, const double search_terms, CollectorData<Data::BoltCalculation,
                                        int>& Collector);  // Для дюймового крепежа.

    FasetenerStrengthClass FastenerStrength; // Объект для работы с данными текучести материала.

    int m_metric_mounting_type_ = NULL;      // Тип метрического крепежа.

    std::vector<std::unique_ptr<FactoryWorkingDiametr>>configuration_working_diametr_;

    std::map<short, short> listOfStandardFastenerSizesMetric(); // Подбор метрического крепежа.
    std::map<double,short> listOfStandardFastenerSizesInch();   // Подбор дюймового крепежа.

		// Табличные значения дюймового крепежа.

    std::string m_shifting_inch_[inch_fasteners_array_size_] {

        "1/8", "1/4", "3/8", "1/2", "5/8", "3/4", "1", "1 1/4", "1 1/2", "2"
    };
};

class ConfigurationCalculationOfStandardFasteners : public FastenerCalculationConfiguration { // Расчет стандартного крепежа.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationSelectionOfStandardFasteners : public FastenerCalculationConfiguration { // Подбор стандартного крепежа.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationCalculationOfNon_StandardFasteners : public FastenerCalculationConfiguration { // Расчет нестандартного крепежа.///////////////
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationSelectionOfNon_StandardFasteners : public FastenerCalculationConfiguration { // Подбор нестандартного крепежа.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};


class FastenersCalculation { // Интерфейсный класс расчета.
 public:
	 FastenersCalculation();

	 double boltConectionCalculation(CollectorData<Data::BoltCalculation, int>& Collector); // Метод возвращает результат расчета.
	 std::string typesizeFastenersInch();                                                   // При расчете дюймового крепежа, 
	 // метод возвращает табличное значения (Использования после проведения основного расчета с помощью метода boltConectionCalculation).

 private:
	 void clearTheFieldsCollector(CollectorData<Data::BoltCalculation, int>& Collector);

     std::vector<std::unique_ptr<FastenerCalculationConfiguration>>configuration_fastener_calculation;
	 std::string m_type_size_fasteners_inch_;

	 const short m_capacity_ = 4; // Размер вектора.
};


class InputValidation { // Класс реализации проверки данных ввода (используеться перед тем как проводить расчет).
 public:
	 bool operator()(CollectorData<Data::BoltCalculation, int>& Collector);
};


class ResultCheck { // Класс реализации отслеживания ситуации, в которой расчетный диаметр превышает допустимые (стандартные) что расположены в коллекциях
	// metric_fasteners_size_, а также inch_fasteners_size_.
 public:
	 ~ResultCheck();

	 bool operator()(CollectorData<Data::BoltCalculation, int>& Collector);

 private:
	 CollectorData<Data::BoltCalculation, int>* pCollector = nullptr;
};

#endif // MODULBOLTCALCULATION
