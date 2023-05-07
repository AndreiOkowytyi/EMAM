#pragma once

#ifndef DESIGNCOMFIGURATIONOFTHEAGROTECHNIKALMODULE_H
#define DESIGNCOMFIGURATIONOFTHEAGROTECHNIKALMODULE_H

#include <vector>
#include "ui_submodulagriculturalmachiner.h"

class ConfigurationAgroModule {
 public:
    virtual ~ConfigurationAgroModule() {}
    virtual void configurationAgroModul(Ui::AgriculturalMachinerCultivators* pAgricultureModul) = 0;
};

class ConfigurationCultivators : public ConfigurationAgroModule {
 public:
    void configurationAgroModul(Ui::AgriculturalMachinerCultivators* pAgricultureModul) override;
};

class ConfigurationDisc : public ConfigurationAgroModule {
 public:
    void configurationAgroModul(Ui::AgriculturalMachinerCultivators* pAgricultureModul) override;
};


class AgromoduleCalculationConfiguration {
public:
    AgromoduleCalculationConfiguration();
    void configuration(const short index_configuration, Ui::AgriculturalMachinerCultivators* pAgricultureModul);

private:
    std::vector<std::unique_ptr<ConfigurationAgroModule>>m_configuration_agro_modul;
    const int reserve = 3;
};

#endif // DESIGNCOMFIGURATIONOFTHEAGROTECHNIKALMODULE_H
