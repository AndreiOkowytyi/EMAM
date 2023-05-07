#include "implementationmodulboltcalculation.h"

WorkingDiametrMetric::WorkingDiametrMetric() {

	p_fastener_working_diametr_ = new double[metric_fasteners_array_size_] {

		0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.45, 3.1, 3.7, 4.3, 4.9, 5.5, 6.1, 6.7, 7.3, 9.8
	};
}

WorkingDiametrMetric::~WorkingDiametrMetric() {

	delete[] p_fastener_working_diametr_;
}

// Расчет, и возврат рабочего диаметра метрического крепежа.

double WorkingDiametrMetric::fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) { 

	if (Collector.conclusionData(Data::BoltCalculation::step_) >= metric_fasteners_array_size_) exit(1);

	return static_cast<double>(Collector.conclusionData(Data::BoltCalculation::bolt_M_diametr_index_)) - 
		p_fastener_working_diametr_[Collector.conclusionData(Data::BoltCalculation::step_)];
}


WorkingDiametrInch::WorkingDiametrInch() {

    p_fastener_working_diametr_ = new double[inch_fasteners_array_size_] {

        8.8, 11.5, 15, 18.7, 20.8, 24.2, 30.3, 39, 44.8, 56.3
    };
}

WorkingDiametrInch::~WorkingDiametrInch() {
	
	delete[] p_fastener_working_diametr_;
}

double WorkingDiametrInch::fastenerWorkingDiametr(CollectorData<Data::BoltCalculation, int>& Collector) {

	if (Collector.conclusionData(Data::BoltCalculation::bolt_G_index) >= inch_fasteners_array_size_) exit(1);

	return p_fastener_working_diametr_[Collector.conclusionData(Data::BoltCalculation::bolt_G_index)];
}

short WorkingDiametrInch::size() { return inch_fasteners_array_size_; }


std::unique_ptr<WorkingDiametr> FactoryWorkinDiametrMetric::createdWorkingDiametrObjects() {

	return std::make_unique<WorkingDiametrMetric>();
}

std::unique_ptr<WorkingDiametr> FactoryWorkingDiametrInch::createdWorkingDiametrObjects() {

	return std::make_unique<WorkingDiametrInch>();
}


FasetenerStrengthClass::FasetenerStrengthClass() {

	// Клaсс прочности (данные в МПа):
	// 3.6, 4.6, 4.8 5.6, 5.8, 6.6, 6.8, 8.8, 9.8, 10.9, 12.9

	p_yield_strength_ = new short[this->m_capacity_] {

		180, 240, 320, 300, 400, 460, 480, 640, 720, 900, 1000
	};
}

FasetenerStrengthClass::~FasetenerStrengthClass() {

	delete[] p_yield_strength_;
}

short FasetenerStrengthClass::tieldStrengthFastener(const int index_type_load, const int strength_class) {

	if (strength_class >= this->m_capacity_) exit(1);                             // Проверка, не выходит ли индекс за массив.

	if      (index_type_load == 0) return p_yield_strength_[strength_class];      // На рвзрыв.
	else if (index_type_load == 1) return p_yield_strength_[strength_class] / 2;  // На срез.

	return 0;
}


double CalculationOfStandardFasteners::calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
	std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength) {

	return ((PI * std::pow(working_diametr->fastenerWorkingDiametr(Collector), 2) / 4) * 
		FastenersStrength.tieldStrengthFastener(Collector.conclusionData(Data::BoltCalculation::bolt_load_index_),
			Collector.conclusionData(Data::BoltCalculation::strength_class_))) * Collector.conclusionData
			    (Data::BoltCalculation::number_of_bolts_connection_) / Collector.conclusionData(Data::BoltCalculation::load_bolt_connectin_);
}


double ChoiceOfStandardFasteners::calculationBeamConnects(CollectorData<Data::BoltCalculation, int>& Collector,
	FasetenerStrengthClass& FastenersStrength) {

	if (Collector.conclusionData(Data::BoltCalculation::changes_diamter_factor_)) this->changesDiametrFactor();

	return std::sqrt((4 * static_cast<double>(Collector.conclusionData(Data::BoltCalculation::load_bolt_connectin_)) / 
		Collector.conclusionData(Data::BoltCalculation::number_of_bolts_connection_)) / (PI * FastenersStrength.tieldStrengthFastener
		   (Collector.conclusionData(Data::BoltCalculation::bolt_load_index_), Collector.conclusionData(Data::BoltCalculation::strength_class_)) / 
			   Collector.conclusionData(Data::BoltCalculation::margin_of_safary_))) * this->m_diamtr_factor_;
}


double CalculationOfNonStandardThreadedFasteners::threadSafetyMargin(short tield_strength_fastener, double result_fasteninf_calculation) {

	return tield_strength_fastener / result_fasteninf_calculation;
};


double ShearFasteningCalculation::calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
	std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength) {

	Collector.dataCollection(Data::BoltCalculation::bolt_load_index_, 1); // Установка флага типа нагрузки, для правильного проведения расчета.

	double margin_of_safary_ = this->threadSafetyMargin(FastenersStrength.tieldStrengthFastener(Collector.
		conclusionData(Data::BoltCalculation::bolt_load_index_), Collector.conclusionData(Data::BoltCalculation::strength_class_)), 
		   ((static_cast<double>(Collector.conclusionData(Data::BoltCalculation::load_bolt_connectin_)) / Collector.conclusionData
		       (Data::BoltCalculation::number_of_bolts_connection_)) / (PI * working_diametr->fastenerWorkingDiametr(Collector) *
	               Collector.conclusionData(Data::BoltCalculation::length_female_screw_) * 0.7 * this->m_thread_work_factor_)));

	Collector.dataCollection(Data::BoltCalculation::bolt_load_index_, 0);

	return margin_of_safary_;
}


CrushingFasteningCalculation::CrushingFasteningCalculation() {

	p_list_of_metric_thread_pitches = new double[metric_fasteners_array_size_] {

		0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 8 
	};

	p_list_of_inch_thread_pitches = new double[inch_fasteners_array_size_] {

		0.9, 1.3, 1.3, 1.8, 1.8, 1.8, 2.2, 2.2, 2.3, 2.3
	};
}

CrushingFasteningCalculation::~CrushingFasteningCalculation() {

	delete[] p_list_of_metric_thread_pitches;
	delete[] p_list_of_inch_thread_pitches;
}

short CrushingFasteningCalculation::numberOfWorkersCoil(CollectorData<Data::BoltCalculation, int>& Collector) {

	if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 0)
		return static_cast<short>(Collector.conclusionData(Data::BoltCalculation::length_female_screw_) /
		    p_list_of_metric_thread_pitches[Collector.conclusionData(Data::BoltCalculation::step_)]);

	else if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 1)
		return static_cast<short>(Collector.conclusionData(Data::BoltCalculation::length_female_screw_) /
		    p_list_of_inch_thread_pitches[Collector.conclusionData(Data::BoltCalculation::bolt_G_index)]);
	return 0;
}

int CrushingFasteningCalculation::indexDiametrFasteners(CollectorData<Data::BoltCalculation, int>& Collector) {

	if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 0) 
		return Collector.conclusionData(Data::BoltCalculation::bolt_M_diametr_index_);

	else if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 1) 
		return Collector.conclusionData(Data::BoltCalculation::bolt_G_diametr_index_);
	return 0;
}

double CrushingFasteningCalculation::calculationBoltConnects(CollectorData<Data::BoltCalculation, int>& Collector,
	std::unique_ptr<WorkingDiametr> working_diametr, FasetenerStrengthClass& FastenersStrength) {

	return this->threadSafetyMargin(FastenersStrength.tieldStrengthFastener(Collector.conclusionData(Data::BoltCalculation::bolt_load_index_),
		Collector.conclusionData(Data::BoltCalculation::strength_class_)), (((4 * static_cast<double>
			(Collector.conclusionData(Data::BoltCalculation::load_bolt_connectin_))) / 
			    Collector.conclusionData(Data::BoltCalculation::number_of_bolts_connection_)) / (((std::pow(indexDiametrFasteners(Collector), 2) -
				  std::pow(working_diametr->fastenerWorkingDiametr(Collector), 2)) * numberOfWorkersCoil(Collector) * m_thread_work_factor_)))) * 0.8;
}


FastenerCalculationConfiguration::FastenerCalculationConfiguration() {

	this->configuration_working_diametr_.emplace_back(std::make_unique<FactoryWorkinDiametrMetric>());
	this->configuration_working_diametr_.emplace_back(std::make_unique<FactoryWorkingDiametrInch>());
}

std::map<short, short> FastenerCalculationConfiguration::listOfStandardFastenerSizesMetric() {

	std::map<short, short>metric_fasteners_size_;

	metric_fasteners_size_.emplace(std::pair<short, short>(6,   2));
	metric_fasteners_size_.emplace(std::pair<short, short>(8,   3));
	metric_fasteners_size_.emplace(std::pair<short, short>(10,  4));
	metric_fasteners_size_.emplace(std::pair<short, short>(12,  5));
	metric_fasteners_size_.emplace(std::pair<short, short>(14,  6));
	metric_fasteners_size_.emplace(std::pair<short, short>(16,  6));
	metric_fasteners_size_.emplace(std::pair<short, short>(18,  7));
	metric_fasteners_size_.emplace(std::pair<short, short>(20,  7));
	metric_fasteners_size_.emplace(std::pair<short, short>(22,  7));
	metric_fasteners_size_.emplace(std::pair<short, short>(24,  8));
	metric_fasteners_size_.emplace(std::pair<short, short>(27,  8));
	metric_fasteners_size_.emplace(std::pair<short, short>(30,  9));
	metric_fasteners_size_.emplace(std::pair<short, short>(33,  9));
	metric_fasteners_size_.emplace(std::pair<short, short>(36,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(39,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(40,  8));
	metric_fasteners_size_.emplace(std::pair<short, short>(42,  11));
	metric_fasteners_size_.emplace(std::pair<short, short>(45,  11));
	metric_fasteners_size_.emplace(std::pair<short, short>(48,  12));
	metric_fasteners_size_.emplace(std::pair<short, short>(52,  12));
	metric_fasteners_size_.emplace(std::pair<short, short>(55,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(56,  13));
	metric_fasteners_size_.emplace(std::pair<short, short>(60,  13));
	metric_fasteners_size_.emplace(std::pair<short, short>(62,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(64,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(65,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(68,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(70,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(72,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(75,  10));
	metric_fasteners_size_.emplace(std::pair<short, short>(76,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(80,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(85,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(90,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(95,  14));
	metric_fasteners_size_.emplace(std::pair<short, short>(100, 14));
	metric_fasteners_size_.emplace(std::pair<short, short>(105, 14));
	metric_fasteners_size_.emplace(std::pair<short, short>(110, 14));
	metric_fasteners_size_.emplace(std::pair<short, short>(115, 14));
	metric_fasteners_size_.emplace(std::pair<short, short>(120, 14));
	metric_fasteners_size_.emplace(std::pair<short, short>(125, 15));
	metric_fasteners_size_.emplace(std::pair<short, short>(130, 15));

	return metric_fasteners_size_;
}

std::map<double, short> FastenerCalculationConfiguration::listOfStandardFastenerSizesInch() {

	WorkingDiametrInch DiametrInch;

	short index_data_ = NULL;

	const short capacity_ = DiametrInch.size();

    auto lambdaData = [&]() {

        if(index_data_ <= capacity_ - 1) return ++index_data_;
        else return index_data_;
    };

	std::map<double, short> inch_fasteners_size_;

	inch_fasteners_size_.emplace(std::pair<double, short>(9.5,  index_data_));
	inch_fasteners_size_.emplace(std::pair<double, short>(13,   lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(16.4, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(20.7, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(22.7, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(26.2, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(33.1, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(41.7, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(47.6, lambdaData()));
	inch_fasteners_size_.emplace(std::pair<double, short>(59.4, lambdaData()));

	return inch_fasteners_size_;
}

std::string FastenerCalculationConfiguration::inchThreadcalculationresult(CollectorData<Data::BoltCalculation, int>& Collector) { 
	
	if (Collector.conclusionData(Data::BoltCalculation::bolt_G_index) >= inch_fasteners_array_size_) exit(1);

	return this->m_shifting_inch_[Collector.conclusionData(Data::BoltCalculation::bolt_G_index)];
}

template<typename T>
void FastenerCalculationConfiguration::searchForTheDesiredValueMetric(T&& metric_size_, const short search_terms,
	CollectorData<Data::BoltCalculation, int>& Collector) {

	auto result_search = metric_size_.find(search_terms); // Поиск расчетного значения в коллекции.

	auto lambda = [&]() { 

		this->m_metric_mounting_type_ = static_cast<int>(result_search->first); 

		if (Collector.conclusionData(Data::BoltCalculation::bolt_load_index_) == 0) {

			// При расчете на разрыв, производиться запись данных в нижеперечисленные ячейки класса сборщика.

			Collector.dataCollection(Data::BoltCalculation::bolt_M_diametr_index_, static_cast<const int>(result_search->first));
			Collector.dataCollection(Data::BoltCalculation::step_,                 static_cast<const int>(result_search->second));
		}
	};

	if (result_search != metric_size_.end()) { // При нахождении в коллекции расчетного значения.

		// Условия поиска подразумевают переход на следующую ячейку коллекции, так как расчетное значение это рабочий диаметр.

		std::advance(result_search, 1);

		if (result_search != metric_size_.end()) lambda();
		else Collector.dataCollection(Data::BoltCalculation::out_of_reach_, 1); // Активация флага об ошибке подбора значения.
		// и дальнейшая его обработка в классе ResultCheck.

	} else { // Если значение отсутствует,производиться его добавление в коллекцию.

		metric_size_.emplace(std::pair<short, short>(search_terms, NULL));

		result_search = metric_size_.find(search_terms);

		std::advance(result_search, 1);

		if (result_search != metric_size_.end()) lambda();
		else Collector.dataCollection(Data::BoltCalculation::out_of_reach_, 1);

		std::advance(result_search, -1);

		metric_size_.erase(result_search);
	}
}

template<typename T>
void FastenerCalculationConfiguration::searchForTheDesiredValueInch(T&& inch_size_, const double search_terms,
	CollectorData<Data::BoltCalculation, int>& Collector) {

	auto result_search = inch_size_.find(search_terms);

	auto lambda = [&]() {

			Collector.dataCollection(Data::BoltCalculation::bolt_G_diametr_index_, static_cast<const int>(result_search->first));
			Collector.dataCollection(Data::BoltCalculation::bolt_G_index,          static_cast<const int>(result_search->second));
	};

	if (result_search != inch_size_.end()) {

		std::advance(result_search, 1);

		if (result_search != inch_size_.end()) lambda();
		else Collector.dataCollection(Data::BoltCalculation::out_of_reach_, 1);

	} else {

		inch_size_.emplace(std::pair<double, short>(search_terms, NULL));

		result_search = inch_size_.find(search_terms);

		std::advance(result_search, 1);

		if (result_search != inch_size_.end()) lambda();
		else Collector.dataCollection(Data::BoltCalculation::out_of_reach_, 1);

		std::advance(result_search, -1);

		inch_size_.erase(result_search);
	}
}


double ConfigurationCalculationOfStandardFasteners::configurationFastenrCalculation(CollectorData<Data::BoltCalculation,
                                                                                                         int>& Collector) {

	CalculationOfStandardFasteners StandartFasteners; // Объект выполнения расчета, возвращая запас прочности.

	return StandartFasteners.calculationBeamConnects
	    (Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
			->createdWorkingDiametrObjects(), FastenerStrength);
}


double ConfigurationSelectionOfStandardFasteners::configurationFastenrCalculation(CollectorData<Data::BoltCalculation,
                                                                                                           int>& Collector) {

	ChoiceOfStandardFasteners ChoiceFasteners; // Объект выполнения подбора в зависимости от типа нагрузки и других данных.

	double result_ = ChoiceFasteners.calculationBeamConnects(Collector, FastenerStrength);

	if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 0) {

		// В зависимости от типа крепежа выполняеться подбор из коллекции стандартных типоразмеров с помощью функций
		// searchForTheDesiredValueMetric, searchForTheDesiredValueInch.

		searchForTheDesiredValueMetric(std::move(listOfStandardFastenerSizesMetric()), static_cast<short>(result_), Collector);
		return this->m_metric_mounting_type_;

	} else {

		searchForTheDesiredValueInch(std::move(listOfStandardFastenerSizesInch()), result_, Collector);
		return 0;
	}
}


double ConfigurationCalculationOfNon_StandardFasteners::configurationFastenrCalculation(CollectorData<Data::BoltCalculation,
                                                                                                          int>& Collector) {

	double result_[3];

	CalculationOfStandardFasteners StandartFasteners;   // Объект выполнения расчета, возвращая запас прочности.
	ShearFasteningCalculation      ShearCalculation;    // Объект расчета резьбы на срез.
	CrushingFasteningCalculation   CrushingCalculation; // Объект расчета резьбы на смятие.

	result_[0] = StandartFasteners.calculationBeamConnects
	(Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
		->createdWorkingDiametrObjects(), FastenerStrength);

    std::thread StreamShearFasteners([&]() {result_[1] = ShearCalculation.calculationBoltConnects
	(Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
		->createdWorkingDiametrObjects(), FastenerStrength); });

    std::thread StreamCrushingFastening([&]() {result_[2] = CrushingCalculation.calculationBoltConnects
	(Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
		->createdWorkingDiametrObjects(), FastenerStrength); });

	StreamShearFasteners.   join();
	StreamCrushingFastening.join();

	auto min_result_ = std::min_element(std::begin(result_), std::end(result_)); // Поиск минимального значения запаса прочности.

	return *min_result_;
}


double ConfigurationSelectionOfNon_StandardFasteners::configurationFastenrCalculation(CollectorData<Data::BoltCalculation,
                                                                                                            int>& Collector) {

	ChoiceOfStandardFasteners    ChoiceFasteners;     // Объект выполнения подбора в зависимости от типа нагрузки и других данных.
	ShearFasteningCalculation    ShearCalculation;    // Объект расчета резьбы на срез.
	CrushingFasteningCalculation CrushingCalculation; // Объект расчета резьбы на смятие.

	std::map<short, short> metric_fasteners_size_ (std::move(listOfStandardFastenerSizesMetric()));
	std::map<double,short> inch_fasteners_size_   (std::move(listOfStandardFastenerSizesInch()));

	double result_array_[2];
	bool flag_ = true;

	while (flag_) {

	    double result_ = ChoiceFasteners.calculationBeamConnects(Collector, FastenerStrength);

	    if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 0) {

			searchForTheDesiredValueMetric(metric_fasteners_size_,static_cast<short>(result_), Collector);

	    } else searchForTheDesiredValueInch(inch_fasteners_size_, result_, Collector);

        std::thread StreamShearFasteners([&]() {result_array_[0] = ShearCalculation.calculationBoltConnects
	    	  (Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
		    	->createdWorkingDiametrObjects(), FastenerStrength); });

        std::thread StreamCrushingFastening([&]() {result_array_[1] = CrushingCalculation.calculationBoltConnects
		  (Collector, configuration_working_diametr_[Collector.conclusionData(Data::BoltCalculation::type_fasteners_)]
			->createdWorkingDiametrObjects(), FastenerStrength); });

	    StreamShearFasteners.   join();
	    StreamCrushingFastening.join();

		// Проверка условия на запас прочности резьбового крепежа.

	    if (result_array_[0] >= Collector.conclusionData(Data::BoltCalculation::margin_of_safary_) &&
			  result_array_[1] >= Collector.conclusionData(Data::BoltCalculation::margin_of_safary_)) flag_ = false;

		else Collector.dataCollection(Data::BoltCalculation::changes_diamter_factor_, 1); // Если условия не выполнены, происходит
		// установка флага повышения дополнительного коэффициента расчета рабочего диаметра.

		if(Collector.conclusionData(Data::BoltCalculation::out_of_reach_) == 1) return 0; // Если значение выходит за пределы коллекции, 
		// поиск значения, и подбор завершаеться.
	}

	Collector.dataCollection(Data::BoltCalculation::changes_diamter_factor_, 0);

	if (Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 0) return this->m_metric_mounting_type_;

	else return 0;
}


FastenersCalculation::FastenersCalculation() {

	short index_iterator_ = 0;
	auto indexIterator = [&]() {

		auto iterator = configuration_fastener_calculation.begin();
		std::advance(iterator, index_iterator_);
		if (index_iterator_ <= this->m_capacity_ - 1) ++index_iterator_;
		return iterator;
	};

	configuration_fastener_calculation.reserve(this->m_capacity_);

	configuration_fastener_calculation.emplace(indexIterator(), std::make_unique<ConfigurationCalculationOfStandardFasteners>());
	configuration_fastener_calculation.emplace(indexIterator(), std::make_unique<ConfigurationSelectionOfStandardFasteners>());
	configuration_fastener_calculation.emplace(indexIterator(), std::make_unique<ConfigurationCalculationOfNon_StandardFasteners>());
	configuration_fastener_calculation.emplace(indexIterator(), std::make_unique<ConfigurationSelectionOfNon_StandardFasteners>());
}

void FastenersCalculation::clearTheFieldsCollector(CollectorData<Data::BoltCalculation, int>& Collector) {

	Collector.dataCollection(Data::BoltCalculation::changes_diamter_factor_, 0);
}

std::string FastenersCalculation::typesizeFastenersInch() { return this->m_type_size_fasteners_inch_; }

double FastenersCalculation::boltConectionCalculation(CollectorData<Data::BoltCalculation, int>& Collector) {

	double result =  configuration_fastener_calculation[Collector.conclusionData(Data::BoltCalculation::calculation_bolted_connection_)]->
		configurationFastenrCalculation(Collector);

    if(Collector.conclusionData(Data::BoltCalculation::type_fasteners_) == 1 &&
            Collector.conclusionData(Data::BoltCalculation::calculation_bolted_connection_) == 3 ||
                Collector.conclusionData(Data::BoltCalculation::calculation_bolted_connection_) == 1)

        this->m_type_size_fasteners_inch_ = configuration_fastener_calculation
                [Collector.conclusionData(Data::BoltCalculation::calculation_bolted_connection_)]
                     ->inchThreadcalculationresult(Collector);

	clearTheFieldsCollector(Collector);

	return result;
}


bool InputValidation::operator()(CollectorData<Data::BoltCalculation, int>& Collector) {

	bool result_of_checking_[2];

	if (Collector.conclusionData(Data::BoltCalculation::load_bolt_connectin_) > 0) result_of_checking_[0] = true;
	else result_of_checking_[0] = false;

	if (Collector.conclusionData(Data::BoltCalculation::bolt_load_index_) == 0)

		if (Collector.conclusionData(Data::BoltCalculation::length_female_screw_) > 0) result_of_checking_[1] = true;
		else result_of_checking_[1] = false;
	else result_of_checking_[1] = true;

	if (result_of_checking_[0] && result_of_checking_[1]) return true;
	else return false;
}
		

bool ResultCheck::operator()(CollectorData<Data::BoltCalculation, int>& Collector) {

	this->pCollector = &Collector;
	if (Collector.conclusionData(Data::BoltCalculation::out_of_reach_) == 0) return true;
	else return false;
}

ResultCheck::~ResultCheck() {

	if(pCollector != nullptr)
	pCollector->dataCollection(Data::BoltCalculation::out_of_reach_, 0);
}
