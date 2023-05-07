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

class WorkingDiametrMetric : public WorkingDiametr { // ������������ ������� ��������� ������������ �������.
 public:
    WorkingDiametrMetric();
    ~WorkingDiametrMetric();

    // ������� ��������� ���������, ��� ������ ������ (����������� �����, �6, �8, �10 � �.�.),
    // ������ ��� ������, � � ����������� ������ ���������� ������� ������� �������.

    double fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) override;

 private:
    double* p_fastener_working_diametr_ = nullptr;
	};


class WorkingDiametrInch : public WorkingDiametr { // ������������ ������� ��������� ��������� �������.
 public:
    WorkingDiametrInch();
    ~WorkingDiametrInch();

    // ������� ��������� ���������, ��� �������� ������ ������� ������� ���������.

    double fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) override;

    short size(); // ������� ���������� ������ �������.

 private:
    double* p_fastener_working_diametr_ = nullptr;
};


class FactoryWorkingDiametr {
 public:
    virtual ~FactoryWorkingDiametr() {}
    virtual std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() = 0;
};

class FactoryWorkinDiametrMetric : public FactoryWorkingDiametr { // ������� ��������� ������
//������� ��������� ������������ �������.
 public:
    std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() override;
};

class FactoryWorkingDiametrInch : public FactoryWorkingDiametr {  // ������� ��������� ������
//������� ��������� ��������� �������.
 public:
    std::unique_ptr<WorkingDiametr> createdWorkingDiametrObjects() override;
};


class FasetenerStrengthClass { // ������� ��������� � ����������� �� ������ ��������� �������.
 public:
    FasetenerStrengthClass();
    ~FasetenerStrengthClass();

    // ������� ��������� ����������� ������ ��������� ��� ��������� ������� �������� �������.

    short tieldStrengthFastener(const int index_type_load, const int strength_class);

 private:
    short* p_yield_strength_ = nullptr;
    const short m_capacity_ = 11; // ������ �������.
};


class CalculationOfStandardFasteners { // ������ ������������ �������.
 public:
    double calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);
};


class ChoiceOfStandardFasteners { // ������ ������������ �������.
 public:
    double calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
                                                          FasetenerStrengthClass& FastenersStrength);

 public:
    double changesDiametrFactor() { return m_diamtr_factor_ += 0.25; } // ������� ��������� ������������ ������� �������� �������� �������.

    double m_diamtr_factor_ = 1; // ����������� ������� �������� �������
};


class CalculationOfNonStandardThreadedFasteners {
 protected:
    const double m_thread_work_factor_ = 0.65; // ��������������� �����������.

    double threadSafetyMargin(short tield_strength_fastener, double result_fasteninf_calculation); // ������ ������ ���������.
};

class ShearFasteningCalculation : public CalculationOfNonStandardThreadedFasteners { // ������ �� ���� ������.
 public:
    double calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);
};

class CrushingFasteningCalculation : public CalculationOfNonStandardThreadedFasteners { // ������ �� ������ ������.
 public:
    CrushingFasteningCalculation();
    ~CrushingFasteningCalculation();

    double calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
        std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength);

 private:
    short numberOfWorkersCoil(CollectorData<Data::BoltCalculation, int>& Collector); // ���������� ������� ������ �������.
    int indexDiametrFasteners(CollectorData<Data::BoltCalculation, int>& Collector); // ���������� ������� ������� �������.

    double* p_list_of_metric_thread_pitches = nullptr; // ��� ������������ ������� � ��.
    double* p_list_of_inch_thread_pitches   = nullptr; // ��� ��������� ������� � ��.
};


class FastenerCalculationConfiguration {
 public:
    FastenerCalculationConfiguration();
    virtual ~FastenerCalculationConfiguration() {}
    virtual double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) = 0;

    std::string inchThreadcalculationresult(CollectorData<Data::BoltCalculation, int>& Collector); // ����� ����������
    // ������ ��������� ������� ����� ���������� �������.

 protected:
    // ����� ���������� ������� ������������ ������� �������� ������a.

    template<typename T>
    void searchForTheDesiredValueMetric(T&& metric_size_, const short search_terms, CollectorData<Data::BoltCalculation,
                                        int>& Collector);  // ��� ������������ �������.

    template<typename T>
    void searchForTheDesiredValueInch(T&& inch_size_, const double search_terms, CollectorData<Data::BoltCalculation,
                                        int>& Collector);  // ��� ��������� �������.

    FasetenerStrengthClass FastenerStrength; // ������ ��� ������ � ������� ��������� ���������.

    int m_metric_mounting_type_ = NULL;      // ��� ������������ �������.

    std::vector<std::unique_ptr<FactoryWorkingDiametr>>configuration_working_diametr_;

    std::map<short, short> listOfStandardFastenerSizesMetric(); // ������ ������������ �������.
    std::map<double,short> listOfStandardFastenerSizesInch();   // ������ ��������� �������.

		// ��������� �������� ��������� �������.

    std::string m_shifting_inch_[inch_fasteners_array_size_] {

        "1/8", "1/4", "3/8", "1/2", "5/8", "3/4", "1", "1 1/4", "1 1/2", "2"
    };
};

class ConfigurationCalculationOfStandardFasteners : public FastenerCalculationConfiguration { // ������ ������������ �������.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationSelectionOfStandardFasteners : public FastenerCalculationConfiguration { // ������ ������������ �������.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationCalculationOfNon_StandardFasteners : public FastenerCalculationConfiguration { // ������ �������������� �������.///////////////
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};

class ConfigurationSelectionOfNon_StandardFasteners : public FastenerCalculationConfiguration { // ������ �������������� �������.
 public:
    double configurationFastenrCalculation(CollectorData<Data::BoltCalculation, int>& Collector) override;
};


class FastenersCalculation { // ������������ ����� �������.
 public:
	 FastenersCalculation();

	 double boltConectionCalculation(CollectorData<Data::BoltCalculation, int>& Collector); // ����� ���������� ��������� �������.
	 std::string typesizeFastenersInch();                                                   // ��� ������� ��������� �������, 
	 // ����� ���������� ��������� �������� (������������� ����� ���������� ��������� ������� � ������� ������ boltConectionCalculation).

 private:
	 void clearTheFieldsCollector(CollectorData<Data::BoltCalculation, int>& Collector);

     std::vector<std::unique_ptr<FastenerCalculationConfiguration>>configuration_fastener_calculation;
	 std::string m_type_size_fasteners_inch_;

	 const short m_capacity_ = 4; // ������ �������.
};


class InputValidation { // ����� ���������� �������� ������ ����� (������������� ����� ��� ��� ��������� ������).
 public:
	 bool operator()(CollectorData<Data::BoltCalculation, int>& Collector);
};


class ResultCheck { // ����� ���������� ������������ ��������, � ������� ��������� ������� ��������� ���������� (�����������) ��� ����������� � ����������
	// metric_fasteners_size_, � ����� inch_fasteners_size_.
 public:
	 ~ResultCheck();

	 bool operator()(CollectorData<Data::BoltCalculation, int>& Collector);

 private:
	 CollectorData<Data::BoltCalculation, int>* pCollector = nullptr;
};

#endif // MODULBOLTCALCULATION
