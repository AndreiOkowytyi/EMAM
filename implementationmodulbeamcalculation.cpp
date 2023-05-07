#include "implementationmodulbeamcalculation.h"

void arrayIndexCheck(const int index, const short size_array) { if(index >= size_array) exit(1); }

StandartProfileDatabase::StandartProfileDatabase() {

    p_profile_ibeam_X_ = new int[m_size_ibeam_] {

        34200,   43800,   53000,   63300,   77300,   87800,   108700,  120100,  146300,  159800,
        184400,  218800,  274300,  285300,  324100,  410800,  498500,  424600,  480600,  606900,
        734300,  641300,  774800,  946300,  1147900, 1011100, 1185200, 1445900, 1708600, 1286900,
        1486700, 1785500, 2086500, 1497600, 1688300, 1913800, 2360400, 2842800, 2050700, 2295600,
        2724600, 3211000, 2305900, 2587700, 2896400, 3480700, 3644600, 4186600, 4760300, 5624800
    };

    p_profile_ibeam_Y_ = new int[m_size_ibeam_] {

        5800,   7000,   8700,   10000,  12300,  13300,  16700,  18000,  22200,  22950,
        26800,  32500,  41900,  41100,  47000,  61100,  75600,  59300,  67700,  87700,
        108200, 90900,  112500, 139700, 172800, 145400, 173600, 215900, 259000, 158800,
        187200, 229600, 272900, 159000, 185400, 214100, 270400, 328300, 218700, 251000,
        302500, 363200, 199000, 227800, 257400, 315100, 350600, 418300, 478800, 574900
    };

    p_profile_channel_X_ = new int[m_size_channel] {

        9100,   15000,  22500,  34900,  50800,  70400,  93800,  103000, 121000,
        133000, 153000, 193000, 243000, 310000, 389000, 486000, 603000, 763000
    };

    p_profile_channel_Y_ = new int[m_size_channel] {

        3000,  4000,  3300,  7400,  9800,  12900, 16400, 19600, 20600,
        24300, 25200, 31000, 39500, 46700, 54800, 64600, 76300, 89900
    };

    p_profile_corner_ = new int[m_size_corner_] {

        300,    400,    400,    600,    600,    700,    900,    700,    1000,   900,
        1200,   1500,   1200,   1600,   1900,   1500,   2000,   2500,   1900,   2500,
        3100,   3700,   3200,   3900,   4100,   5000,   6000,   5700,   6300,   7400,
        8600,   9700,   7200,   8600,   9900,   11200,  12400,  9000,   9800,   11300,
        12800,  12500,  14400,  16300,  18300,  16700,  17900,  20300,  25000,  29500,
        33800,  38000,  21800,  24800,  32200,  36000,  39800,  47000,  54200,  61100,
        45500,  50300,  59700,  66200,  72400,  78600,  90800,  102600, 114200, 125600,
        92500,  100400, 124600, 134400, 144200, 163400, 200400, 245600, 288600, 175200,
        198700, 258400, 288800, 318700, 348200, 391700, 434200, 462100, 530100
    };
}

StandartProfileDatabase::~StandartProfileDatabase() {

    delete[] p_profile_ibeam_X_;
    delete[] p_profile_ibeam_Y_;
    delete[] p_profile_channel_X_;
    delete[] p_profile_channel_Y_;
    delete[] p_profile_corner_;
}

int StandartProfileDatabase::getProfileIBeam(const int loadX, const int loadY, const int index) {

    arrayIndexCheck(index, this->m_size_ibeam_);

    if      (loadX) return p_profile_ibeam_X_[index];
    else if (loadY) return p_profile_ibeam_Y_[index];
    return 0;
}

int StandartProfileDatabase::getProfileChannel(const int loadX, const int loadY, const int index) {

    arrayIndexCheck(index, this->m_size_channel);

    if      (loadX) return p_profile_channel_X_[index];
    else if (loadY) return p_profile_channel_Y_[index];
    return 0;
}

int StandartProfileDatabase::getProfileCorner(const int index) {

    arrayIndexCheck(index, this->m_size_corner_);
    return p_profile_corner_[index];
}


DatabaseCharacteristicsMaterial::DatabaseCharacteristicsMaterial() {

    variation_load_.emplace_back(DatabaseCharacteristicsMaterial::bendinCharacteristicsMaterial);
    variation_load_.emplace_back(DatabaseCharacteristicsMaterial::shearCharacteristicsMaterial);

    p_bendin_characteristics_material_ = new int[m_size_array_] {

        210, 210, 265, 250, 300, 320, 330, 360
    };

    p_shear_characteristics_material_ = new int[m_size_array_] {

        105, 105, 135, 125, 150, 160, 165, 180
    };
}

DatabaseCharacteristicsMaterial::~DatabaseCharacteristicsMaterial() {

    delete[] p_bendin_characteristics_material_;
    delete[] p_shear_characteristics_material_ ;
}

int DatabaseCharacteristicsMaterial::loadType(const int index_type_load , const int index_characteristics ) {

    return variation_load_.at(index_type_load) (this, index_characteristics);
}

int DatabaseCharacteristicsMaterial::bendinCharacteristicsMaterial(DatabaseCharacteristicsMaterial* Object,const int index) {

    arrayIndexCheck(index, Object->m_size_array_);
    return Object->p_bendin_characteristics_material_[index];
}

int DatabaseCharacteristicsMaterial::shearCharacteristicsMaterial(DatabaseCharacteristicsMaterial* Object, const int index) {

    arrayIndexCheck(index, Object->m_size_array_);
    return Object->p_shear_characteristics_material_[index];
}


ConfigurationResistanceSection::ConfigurationResistanceSection() {

    this->configuration_resistance_section_.reserve(m_capacity_);

    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamrRectangularPipe>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamSquarePipe>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamRoundPipe>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamCircle>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamSquare>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamLeaf>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamIBeam>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamChannel>());
    configuration_resistance_section_.emplace(this->iterator(), std::make_unique<ResistanceBeamCorner>());
}

std::vector<std::unique_ptr<ResistanceSection>>::iterator ConfigurationResistanceSection::iterator() {

    auto it_configuration_ = this->configuration_resistance_section_.begin() + this->m_counter_;

    if (this->m_counter_ < this->m_capacity_)++m_counter_;
    else exit(1);
    return it_configuration_;
}

double ConfigurationResistanceSection::configutationCalculationResistanceSection(const int index,
    CollectorData<Data::BeamCalculation, int>& Collector) {

    return configuration_resistance_section_.at(index)->calculationResistanceSection(Collector);
}


void ResistanceSection::initializationSizeScheme(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->m_scheme_size_B_ = Collector.conclusionData(Data::BeamCalculation::size_B_);
    this->m_scheme_size_H_ = Collector.conclusionData(Data::BeamCalculation::size_H_);
    this->m_scheme_size_S_ = Collector.conclusionData(Data::BeamCalculation::size_S_);
}

double ResistanceBeamrRectangularPipe::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeScheme(Collector);

    if (Collector.conclusionData(Data::BeamCalculation::load_X_index_)) {

        return (std::pow(m_scheme_size_H_,3) * m_scheme_size_B_ -
            std::pow(m_scheme_size_H_ - m_scheme_size_S_ * 2, 3) * (m_scheme_size_B_ - m_scheme_size_S_ * 2)) /
                                                                                           (m_scheme_size_H_ * 6);

    }else if (Collector.conclusionData(Data::BeamCalculation::load_Y_index_)) {

        return (m_scheme_size_H_ * std::pow(m_scheme_size_B_, 3) -
            (m_scheme_size_H_ - m_scheme_size_S_ * 2) * std::pow(m_scheme_size_B_ - m_scheme_size_S_ * 2, 3)) /
                                                                                           (m_scheme_size_B_ * 6);
    }
    return 0;
}

double ResistanceBeamSquarePipe::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeScheme(Collector);

    return (std::pow(m_scheme_size_B_, 4) - std::pow(m_scheme_size_B_ - m_scheme_size_H_ * 2, 4)) /
                                                                                           (6 * m_scheme_size_B_);
}

double ResistanceBeamRoundPipe::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeScheme(Collector);

    return PI * (std::pow(m_scheme_size_B_, 4) - std::pow(m_scheme_size_B_ - m_scheme_size_H_ * 2, 4)) /
                                                                                            (32 * m_scheme_size_B_);
}

double ResistanceBeamCircle::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->m_scheme_size_B_ = Collector.conclusionData(Data::BeamCalculation::size_B_);

    return PI * std::pow(m_scheme_size_B_, 3) / 32;
}

double ResistanceBeamSquare::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->m_scheme_size_B_ = Collector.conclusionData(Data::BeamCalculation::size_B_);

    return std::pow(m_scheme_size_B_, 3) / 6;
}

double ResistanceBeamLeaf::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeScheme(Collector);

    if (Collector.conclusionData(Data::BeamCalculation::load_X_index_)) {

        return (std::pow(m_scheme_size_H_, 2) * m_scheme_size_B_) / 6;

    }else if (Collector.conclusionData(Data::BeamCalculation::load_Y_index_)) {

        return (std::pow(m_scheme_size_B_, 2) * m_scheme_size_H_) / 6;
    }
    return 0;
}

double ResistanceBeamIBeam::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    StandartProfileDatabase Database;

    return Database.getProfileIBeam(Collector.conclusionData(Data::BeamCalculation::load_X_index_),
        Collector.conclusionData(Data::BeamCalculation::load_Y_index_),
           Collector.conclusionData(Data::BeamCalculation::standart_profile_Ibeam_index_));
}

double ResistanceBeamChannel::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    StandartProfileDatabase Database;

    return Database.getProfileChannel(Collector.conclusionData(Data::BeamCalculation::load_X_index_),
        Collector.conclusionData(Data::BeamCalculation::load_Y_index_),
           Collector.conclusionData(Data::BeamCalculation::standart_profile_channel_index_));
}

double ResistanceBeamCorner::calculationResistanceSection(CollectorData<Data::BeamCalculation, int>& Collector) {

    StandartProfileDatabase Database;
    return Database.getProfileCorner(Collector.conclusionData(Data::BeamCalculation::standart_profile_corner_index_));
}


double SchemeBeamMomentCalculation::getReactionA() {

    return this->m_reaction_A_;
}

double SchemeBeamMomentCalculation::getReactionB() {

    return this->m_reaction_B_;
}

void SchemeBeamMomentCalculation::initializationSizeL1L2LoadF(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->m_load_meam_F_ = Collector.conclusionData(Data::BeamCalculation::load_F_);
    this->m_size_L1_     = Collector.conclusionData(Data::BeamCalculation::scheme_L1_);
    this->m_size_L2_     = Collector.conclusionData(Data::BeamCalculation::scheme_L2_);
}

double BeamTwoSupports::calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeL1L2LoadF(Collector);

    this->m_reaction_A_ = (m_load_meam_F_ * m_size_L2_ / 1000) / (m_size_L1_ / 1000 + m_size_L2_ / 1000);
    this->m_reaction_B_ = (m_load_meam_F_ * m_size_L1_ / 1000) / (m_size_L1_ / 1000 + m_size_L2_ / 1000);

    return this->m_reaction_A_ * m_size_L1_ / 1000;
}

double BeamTwoSupportsCantileverLoad::calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeL1L2LoadF(Collector);

    this->m_reaction_A_ = (m_load_meam_F_ *  m_size_L2_ / 1000) / (m_size_L1_ / 1000);
    this->m_reaction_B_ = (m_load_meam_F_ * (m_size_L1_ / 1000  +  m_size_L2_ / 1000)) / (m_size_L1_ / 1000);

    return this->m_load_meam_F_ * m_size_L2_ / 1000;
}

double BeamOneSupportsCantileverLoad::calculationMaxMomentBeam(CollectorData<Data::BeamCalculation, int>& Collector) {

    this->initializationSizeL1L2LoadF(Collector);

    return this->m_load_meam_F_ * m_size_L1_ / 1000;
}


ConfigurationSchemeBeam::ConfigurationSchemeBeam() {

    configuration_scheme_beam_.reserve(m_capacity_);

    configuration_scheme_beam_.emplace(this->iterator(), std::make_shared<BeamTwoSupports>());
    configuration_scheme_beam_.emplace(this->iterator(), std::make_shared<BeamTwoSupportsCantileverLoad>());
    configuration_scheme_beam_.emplace(this->iterator(), std::make_shared<BeamOneSupportsCantileverLoad>());
}

std::vector<std::shared_ptr<SchemeBeamMomentCalculation>>::iterator ConfigurationSchemeBeam::iterator() {

    auto it_configuration_ = this->configuration_scheme_beam_.begin() + this->m_counter_;

    if (this->m_counter_ < this->m_capacity_)++m_counter_;
    else exit(1);
    return it_configuration_;
}

double ConfigurationSchemeBeam::configutationCalculationMaxMomentBeam(const int index,
    CollectorData<Data::BeamCalculation, int>& Collector) {

    std::shared_ptr<SchemeBeamMomentCalculation>ptr_options_scheme_(configuration_scheme_beam_.at(index));

    this->m_max_reaction_load_   = ptr_options_scheme_->calculationMaxMomentBeam(Collector);
    this->m_reaction_supports_A_ = ptr_options_scheme_->getReactionA();
    this->m_reaction_supports_B_ = ptr_options_scheme_->getReactionB();

    return this->m_max_reaction_load_;
}

double ConfigurationSchemeBeam::getReacionsSupportsA() {

    return this->m_reaction_supports_A_;
}

double ConfigurationSchemeBeam::getReacionsSupportsB() {

    return this->m_reaction_supports_B_;
}


BeamStressCalculation::BeamStressCalculation(CollectorData<Data::BeamCalculation, int> &Collector) {

    CollectorDataBeam = &Collector;
}

double BeamStressCalculation::maxSressBeam() {

    return this->m_max_stress_beam_ = (SchemeBeam.configutationCalculationMaxMomentBeam(CollectorDataBeam->conclusionData
      (Data::BeamCalculation::scheme_option_index_), *CollectorDataBeam) * 1000) /
        ResistanceSection.configutationCalculationResistanceSection
          (CollectorDataBeam->conclusionData(Data::BeamCalculation::section_beam_index_), *CollectorDataBeam);
}

double BeamStressCalculation::marginOfSafery() {

    return CharacteristicsMaterial.loadType
      (CollectorDataBeam->conclusionData(Data::BeamCalculation::beam_load_index_),
         CollectorDataBeam->conclusionData(Data::BeamCalculation::material_index_)) / this->m_max_stress_beam_;
}

double BeamStressCalculation::reactionSupportsA() {

    return SchemeBeam.getReacionsSupportsA();
}

double BeamStressCalculation::reactionSupportsB() {

    return SchemeBeam.getReacionsSupportsB();
}


DataValidation::DataValidation() {

    this->validator_scheme_.reserve(this->m_capacity_scheme_);
    this->validator_resistance_.reserve(this->m_capacity_resistance_);

    this->validator_scheme_.emplace(iterator(Flag::validation_scheme_beam_, validator_scheme_),
        ValidationSchemeBeam::validationBeamTwoSupports);

    this->validator_scheme_.emplace(iterator(Flag::validation_scheme_beam_, validator_scheme_),
        ValidationSchemeBeam::validationBeamTwoSupportsCantileverLoad);

    this->validator_scheme_.emplace(iterator(Flag::validation_scheme_beam_, validator_scheme_),
        ValidationSchemeBeam::validationBeamOneSupportsCantileverLoad);


    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamrRectangularPipe);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamSquarePipe);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamRoundPipe);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamCircle);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamSquare);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        ValidationResistanceBeam::validationResistanceBeamLeaf);

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        [](CollectorData<Data::BeamCalculation,int>& Collector)->bool{ return true; });

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        [](CollectorData<Data::BeamCalculation,int>& Collector)->bool{ return true; });

    this->validator_resistance_.emplace(iterator(Flag::validation_resistance_, validator_resistance_),
        [](CollectorData<Data::BeamCalculation,int>& Collector)->bool{ return true; });
}

std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>::iterator DataValidation::iterator(Flag index,
    std::vector<bool(*)(CollectorData<Data::BeamCalculation, int>& Collector)>&container_) {

    auto it_configuration_ = container_.begin();

    if (index == Flag::validation_scheme_beam_) {

        std::advance(it_configuration_, this->m_counter_scheme_);

        if(this->m_counter_scheme_ < this->m_capacity_scheme_)++m_counter_scheme_;
        else exit(1);

    } else if(index == Flag::validation_resistance_) {

        std::advance(it_configuration_, this->m_counter_resistance_);

        if(this->m_counter_resistance_ < this->m_capacity_resistance_)++m_counter_resistance_;
        else exit(1);
    }
    return it_configuration_;
}

bool DataValidation::operator() (CollectorData<Data::BeamCalculation, int>& Collector) {

    if (this->validator_scheme_.at(Collector.conclusionData(Data::BeamCalculation::scheme_option_index_))(Collector) &&
        this->validator_resistance_.at(Collector.conclusionData(Data::BeamCalculation::section_beam_index_))(Collector))
               return true;

    else return false;
}


bool ValidationSchemeBeam::validationBeamTwoSupports(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::load_F_)    > 0 &&
        Collector.conclusionData(Data::BeamCalculation::scheme_L1_) > 0 &&
        Collector.conclusionData(Data::BeamCalculation::scheme_L2_) > 0) return true;

    else return false;
}

bool ValidationSchemeBeam::validationBeamTwoSupportsCantileverLoad(CollectorData<Data::BeamCalculation, int>& Collector) {

    return ValidationSchemeBeam::validationBeamTwoSupports(Collector);
}

bool ValidationSchemeBeam::validationBeamOneSupportsCantileverLoad(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::load_F_) > 0 &&
        Collector.conclusionData(Data::BeamCalculation::scheme_L1_) > 0) return true;

    else return false;
}

bool ValidationResistanceBeam::validationResistanceBeamrRectangularPipe(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::size_B_) > 0 &&
                          Collector.conclusionData(Data::BeamCalculation::size_H_) > 0 &&
        Collector.conclusionData(Data::BeamCalculation::size_S_) <
                          (Collector.conclusionData(Data::BeamCalculation::size_H_) / 2) &&
        Collector.conclusionData(Data::BeamCalculation::size_S_) <
                          (Collector.conclusionData(Data::BeamCalculation::size_B_) / 2) &&
        Collector.conclusionData(Data::BeamCalculation::size_S_) > 0) return true;

    else return false;
}

bool ValidationResistanceBeam::validationResistanceBeamSquarePipe(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::size_H_) <
            ((Collector.conclusionData(Data::BeamCalculation::size_B_) / 2)) &&
                 Collector.conclusionData(Data::BeamCalculation::size_B_) > 0 &&
                    Collector.conclusionData(Data::BeamCalculation::size_H_) > 0) return true;

    else return false;
}

bool ValidationResistanceBeam::validationResistanceBeamRoundPipe(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::size_H_) <
           (Collector.conclusionData(Data::BeamCalculation::size_B_) / 2) &&
                Collector.conclusionData(Data::BeamCalculation::size_B_) > 0 &&
                    Collector.conclusionData(Data::BeamCalculation::size_H_) > 0) return true;

    else return false;
}

bool ValidationResistanceBeam::validationResistanceBeamCircle(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::size_B_) > 0) return true;
    else return false;
}

bool ValidationResistanceBeam::validationResistanceBeamSquare(CollectorData<Data::BeamCalculation, int>& Collector) {

    return validationResistanceBeamCircle(Collector);
}

bool ValidationResistanceBeam::validationResistanceBeamLeaf(CollectorData<Data::BeamCalculation, int>& Collector) {

    if (Collector.conclusionData(Data::BeamCalculation::size_B_) > 0 &&
        Collector.conclusionData(Data::BeamCalculation::size_H_) > 0) return true;

    else return false;
}
