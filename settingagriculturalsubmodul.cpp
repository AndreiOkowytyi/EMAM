#include "settingagriculturalsubmodul.h"

void ConfigurationCultivators::configurationAgroModul(Ui::AgriculturalMachinerCultivators* pAgricultureModul) {

    pAgricultureModul->groupBox_Disc_->close();
    pAgricultureModul->availability_of_ice_rinks_->move(10, 235);
    pAgricultureModul->estimation_->move(25, 270);
    pAgricultureModul->clear_fields_->move(145, 270);
    pAgricultureModul->label_result_resistance_rink_->close();
    pAgricultureModul->result_resistance_rink_->close();
}

void ConfigurationDisc::configurationAgroModul(Ui::AgriculturalMachinerCultivators* pAgricultureModul) {

    pAgricultureModul->groupBox_paw_width_->close();
    pAgricultureModul->groupBox_Disc_->move(pAgricultureModul->groupBox_paw_width_->pos());
    pAgricultureModul->availability_of_ice_rinks_->move(10, 275);
    pAgricultureModul->estimation_->move(25, 310);
    pAgricultureModul->clear_fields_->move(145, 310);
    pAgricultureModul->label_result_resistance_lancet_paw_->close();
    pAgricultureModul->result_resistance_lancet_paw_->close();

     pAgricultureModul->label_result_resistance_rink_->move(
                 pAgricultureModul->label_result_resistance_lancet_paw_->pos());
     pAgricultureModul->result_resistance_rink_->move(
                 pAgricultureModul->result_resistance_lancet_paw_->pos());
}

AgromoduleCalculationConfiguration::AgromoduleCalculationConfiguration() {

    this->m_configuration_agro_modul.reserve(this->reserve);

    short index_iterator = 0;

    auto iterator = [&](){

        auto iterator_ = m_configuration_agro_modul.begin();
        std::advance(iterator_, index_iterator);
        if(index_iterator < this->reserve) ++index_iterator;
        return iterator_;
    };

    this->m_configuration_agro_modul.emplace(iterator(), std::make_unique<ConfigurationCultivators>());
    this->m_configuration_agro_modul.emplace(iterator(), std::make_unique<ConfigurationDisc>());
}

void AgromoduleCalculationConfiguration::configuration(const short index_configuration,
                                            Ui::AgriculturalMachinerCultivators* pAgricultureModul) {

    this->m_configuration_agro_modul[index_configuration]->configurationAgroModul(pAgricultureModul);
}
