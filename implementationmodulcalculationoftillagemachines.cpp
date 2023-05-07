#include "implementationmodulcalculationoftillagemachines.h"

SettlementPair<int, int> CalculationFaucet::resultCalculation(TypePair index) {

    return this->mresultCalculationPair[static_cast<int>(index)];
}

void CalculationFaucet::dataInputPair(TypePair index, SettlementPair<int, int> pair) {

    this->mresultCalculationPair[static_cast<int>(index)] = pair;
}


void SoilCharacteristicsAndItsResistance::categoryDefinition(CollectorData<Data::TractionCalculation, int>& collectordata) {

    short maximum_working_depth_ = collectordata.conclusionData(Data::TractionCalculation::maximum_working_depth_);

    if (maximum_working_depth_ <= 250)
        this->m_category_ = 0;
    else if (maximum_working_depth_ > 250 || maximum_working_depth_ <= 400)
        this->m_category_ = 1;
    else m_category_ = 2;
}

SettlementPair<int, int>
SoilCharacteristicsAndItsResistance::soilResistance(CollectorData<Data::TractionCalculation, int>& collectordata) {

    this->categoryDefinition(collectordata);
    return SettlementPair<int, int> (
        this->min_earth_resistance_[this->m_category_][collectordata.conclusionData(Data::TractionCalculation::soil_type_)],
        this->max_earth_resistance_[this->m_category_][collectordata.conclusionData(Data::TractionCalculation::soil_type_)]);
}


SettlementPair<std::string, std::string> TractionCategory::requiredTractionCategory(const short powerInKW) {

    this->catygoriaDetermination(powerInKW);
    if(this->m_index_traction_categoty_[0] >= this->m_index_traction_categoty_[1])
        return SettlementPair<std::string, std::string>(this->mTractionCategoty[this->m_index_traction_categoty_[0]],
        " ");

    else return SettlementPair<std::string, std::string>(this->mTractionCategoty[this->m_index_traction_categoty_[0]],
        this->mTractionCategoty[this->m_index_traction_categoty_[1]]);
}

void TractionCategory::catygoriaDetermination(const short powerInKW) {

    auto lambda = [&](const short begin, const short end, const int index) {

        if (begin <= powerInKW && powerInKW <= end) {

            if (this->m_flag_[0]) {

                m_flag_[0] = false;
                this->m_index_traction_categoty_[0] = index;

            } else {
                m_flag_[1] = false;
                m_index_traction_categoty_[1] = index; }
        };
    };

    for (int x = 0; x <= 4; ++x) {

        lambda(this->m_pair_power_[x][0], this->m_pair_power_[x][1], x);
        if (!m_flag_[0] && !m_flag_[1]) break;
    }
    m_flag_[0] = m_flag_[1] = true;
}


SettlementPair<int, int>
BoomShareForTillage::resistanceWorkinTool(CollectorData<Data::TractionCalculation, int>& collectordata,
                                                                            CalculationFaucet& resultСollector) {

    SoilCharacteristicsAndItsResistance earthResistance;

    resultСollector.dataInputPair(CalculationFaucet::TypePair::resistance_of_one_boom_share_for_tillage_, SettlementPair<int, int> (

        static_cast<int>((static_cast<double>(collectordata.conclusionData(Data::TractionCalculation::paw_width_) *
            collectordata.conclusionData(Data::TractionCalculation::maximum_working_depth_)) /
               1000000) * earthResistance.soilResistance(collectordata).minCount()),

        static_cast<int>((static_cast<double>(collectordata.conclusionData(Data::TractionCalculation::paw_width_) *
            collectordata.conclusionData(Data::TractionCalculation::maximum_working_depth_)) /
               1000000) * earthResistance.soilResistance(collectordata).maxCount()
        )));
    return resultСollector.resultCalculation(CalculationFaucet::TypePair::resistance_of_one_boom_share_for_tillage_);
}

SettlementPair<int, int>
TillageDisc::resistanceWorkinTool(CollectorData<Data::TractionCalculation, int>& collectordata, CalculationFaucet& resultСollector) {

    SoilCharacteristicsAndItsResistance earthResistance;

    resultСollector.dataInputPair(CalculationFaucet::TypePair::resistance_of_one_tillage_disc_, SettlementPair<int, int> (

        static_cast<int>((static_cast<double>(collectordata.conclusionData(Data::TractionCalculation::diametr_disc_) *
           collectordata.conclusionData(Data::TractionCalculation::maximum_working_depth_)) /
              1000000) * earthResistance.soilResistance(collectordata).minCount() *
                std::sin(collectordata.conclusionData(Data::TractionCalculation::angle_of_attack_of_the_disc_) * 0.0175)) * 2.7,

        static_cast<int>((static_cast<double>(collectordata.conclusionData(Data::TractionCalculation::diametr_disc_) *
            collectordata.conclusionData(Data::TractionCalculation::maximum_working_depth_)) /
               1000000) * earthResistance.soilResistance(collectordata).maxCount() *
                  (std::sin(collectordata.conclusionData(Data::TractionCalculation::angle_of_attack_of_the_disc_) * 0.0175))) * 2.7
        ));
    return resultСollector.resultCalculation(CalculationFaucet::TypePair::resistance_of_one_tillage_disc_);
}


SettlementPair<int, int>
SettlementBlockPlowSweep::resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation,
    int>& collectordata, CalculationFaucet& resultСollector) {

    BoomShareForTillage plowSweep;
    return SettlementPair<int, int> (

        plowSweep.resistanceWorkinTool(collectordata, resultСollector).minCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_paws_),

        plowSweep.resistanceWorkinTool(collectordata, resultСollector).maxCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_paws_)
        );
}

SettlementPair<int, int> SettlementBlockTillageDisc::resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation,
    int>& collectordata, CalculationFaucet& resultСollector) {

    TillageDisc Disc;

    return SettlementPair<int, int> (

        Disc.resistanceWorkinTool(collectordata, resultСollector).minCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_disc_),

        Disc.resistanceWorkinTool(collectordata, resultСollector).maxCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_disc_)
        );
}

SettlementPair<int, int> BlockPlowSweepTillageDisc::resistanceOfAllWorkingBodiesOfTheUnit(CollectorData<Data::TractionCalculation,
    int>& collectordata, CalculationFaucet& resultСollector) {

    BoomShareForTillage plowSweep;
    TillageDisc Disc;

    return SettlementPair<int, int> (

        (plowSweep.resistanceWorkinTool(collectordata, resultСollector).minCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_paws_)) +
              Disc.resistanceWorkinTool(collectordata, resultСollector).minCount() *
                 collectordata.conclusionData(Data::TractionCalculation::number_of_disc_),

        (plowSweep.resistanceWorkinTool(collectordata, resultСollector).maxCount() *
           collectordata.conclusionData(Data::TractionCalculation::number_of_paws_)) +
              Disc.resistanceWorkinTool(collectordata, resultСollector).maxCount() *
                 collectordata.conclusionData(Data::TractionCalculation::number_of_disc_)
        );
}


TractionCalculationOfAllWorkingBodies::TractionCalculationOfAllWorkingBodies() {

    mCalculationResistance.reserve(3);

    short count = 0;

    auto Iterator = [&]() {

        auto iterator_ = mCalculationResistance.begin();
        std::advance(iterator_, count);
        if (count < this->m_count_settlement_block_) ++count;
        return iterator_;
    };

    mCalculationResistance.emplace(Iterator(), std::make_unique<SettlementBlockPlowSweep>());
    mCalculationResistance.emplace(Iterator(), std::make_unique<SettlementBlockTillageDisc>());
    mCalculationResistance.emplace(Iterator(), std::make_unique<BlockPlowSweepTillageDisc>());
}

void TractionCalculationOfAllWorkingBodies::tractionCalculation(CollectorData<Data::TractionCalculation,
    int>& collectordata, CalculationFaucet& resultСollector) {

    int traction_calculation = 0;

    if (collectordata.conclusionData(Data::TractionCalculation::flag_roller_))

        traction_calculation = collectordata.conclusionData(Data::TractionCalculation::total_weight_of_the_unit_) * 9.8 * 0.4 +
        collectordata.conclusionData(Data::TractionCalculation::implement_working_width_) * 800;

    else traction_calculation = collectordata.conclusionData(Data::TractionCalculation::total_weight_of_the_unit_) * 9.8 * 0.4;


    resultСollector.dataInputPair(CalculationFaucet::TypePair::full_traction_resistance_, SettlementPair<int, int>(

        traction_calculation + mCalculationResistance[collectordata.conclusionData(Data::TractionCalculation::calculation_type_index_)]->
           resistanceOfAllWorkingBodiesOfTheUnit(collectordata, resultСollector).minCount(),

        traction_calculation + mCalculationResistance[collectordata.conclusionData(Data::TractionCalculation::calculation_type_index_)]->
           resistanceOfAllWorkingBodiesOfTheUnit(collectordata, resultСollector).maxCount()
        ));
}


void TractionForceCalculation::requiredPowerOfTheTractionUnit(CollectorData<Data::TractionCalculation, int>& collectordata,
    CalculationFaucet& resultСollector) {

    TractionCalculationOfAllWorkingBodies TreactionCalculation_H;
    TreactionCalculation_H.tractionCalculation(collectordata, resultСollector);

    resultСollector.dataInputPair(CalculationFaucet::TypePair::power_KW_, SettlementPair<int,int>(

        (resultСollector.resultCalculation(CalculationFaucet::TypePair::full_traction_resistance_).minCount() *
           (collectordata.conclusionData(Data::TractionCalculation::maximum_working_speed_) * 1000 / 60)) / 60 / 1000,

        (resultСollector.resultCalculation(CalculationFaucet::TypePair::full_traction_resistance_).maxCount() *
            (collectordata.conclusionData(Data::TractionCalculation::maximum_working_speed_) * 1000 / 60)) / 60 / 1000
        ));

    resultСollector.dataInputPair(CalculationFaucet::TypePair::horsepower_, SettlementPair<int, int>(

        resultСollector.resultCalculation(CalculationFaucet::TypePair::power_KW_).minCount() * 1.36,
        resultСollector.resultCalculation(CalculationFaucet::TypePair::power_KW_).maxCount() * 1.36
        ));
}


bool Validation::validationPlowSweep(CollectorData<Data::TractionCalculation, int>& collectordata) {

    if (collectordata.conclusionData(Data::TractionCalculation::paw_width_) > 0) return true;
    else return false;
}

bool Validation::validationTillageDisc(CollectorData<Data::TractionCalculation, int>& collectordata) {

    if (collectordata.conclusionData(Data::TractionCalculation::diametr_disc_) > 0) return true;
    else return false;
}

bool Validation::validationPlowSweepTillageDisc(CollectorData<Data::TractionCalculation, int>& collectordata) {

    if (validationPlowSweep(collectordata) && validationTillageDisc(collectordata)) return true;
    else return false;
}


InputValidationCalculation::InputValidationCalculation() {

    m_configuration_validation.reserve(this->m_count_configuration);

    short count = 0;

    auto Iterator = [&]() {

        auto iterator_ = m_configuration_validation.begin();
        std::advance(iterator_, count);
        if (count < this->m_count_configuration) ++count;
        return iterator_;
    };

    m_configuration_validation.emplace(Iterator(), Validation::validationPlowSweep);
    m_configuration_validation.emplace(Iterator(), Validation::validationTillageDisc);
    m_configuration_validation.emplace(Iterator(), Validation::validationPlowSweepTillageDisc);
}

bool InputValidationCalculation::validation(CollectorData<Data::TractionCalculation, int>& collectordata) {

    if (collectordata.conclusionData(Data::TractionCalculation::total_weight_of_the_unit_) > 0 &&
        m_configuration_validation.at(collectordata.conclusionData(Data::TractionCalculation::calculation_type_index_))
            (collectordata))
                return true;
    else return false;
}
