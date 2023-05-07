#include "callofcalculationmodules.h"

void InterfaceCalling::setCallModules(CallModules* callModul) {

    this->pCallModules = callModul;
    QObject::connect(this, SIGNAL(secondaryMove(int,int)),pCallModules, SIGNAL(position(int,int)));
}

template<class T>
void InterfaceCalling::callAndConnect(T* pSubmodul, const int x, const int y) {

    pSubmodul->show();
    pSubmodul->move(x, y);
    QObject::connect(pSubmodul, SIGNAL(previousShowForm(int,int)), this, SIGNAL(secondaryMove(int, int)));
}

void SubmoduleBeamCalculation::createSubmodul(const int x, const int y) {

    pEnginModulBeamCalculation = new BeamCalculation;
    this->callAndConnect(pEnginModulBeamCalculation, x, y);
}

void SubmoduleBoltCalculation::createSubmodul(const int x, const int y) {

    pEnginModulBoltCalculation = new BoltCalculation;
    this->callAndConnect(pEnginModulBoltCalculation, x, y);
}


void SubmodulCultivatorCalculation::createSubmodul(const int x, const int y) {

    pAgroModulMachinerCultivators = new AgriculturalMachinerCultivators(0);
    this->callAndConnect(pAgroModulMachinerCultivators, x, y);
}

 void SubmodulDiscatorCalculation::createSubmodul(const int x, const int y) {

    pAgroModulMachinerCultivators = new AgriculturalMachinerCultivators(1);
    this->callAndConnect(pAgroModulMachinerCultivators, x, y);
 }

 void SubmodulCombinedCalculation::createSubmodul(const int x, const int y) {

    pAgroModulMachinerCultivators = new AgriculturalMachinerCultivators(2);
    this->callAndConnect(pAgroModulMachinerCultivators, x, y);
 }


 CallModules::CallModules() {

     this->engineeringModul.emplace_back(std::make_shared<SubmoduleBeamCalculation>());
     this->engineeringModul.emplace_back(std::make_shared<SubmoduleBoltCalculation>());

     this->agrotechnicalModul.emplace_back(std::make_shared<SubmodulCultivatorCalculation>());
     this->agrotechnicalModul.emplace_back(std::make_shared<SubmodulDiscatorCalculation>());
     this->agrotechnicalModul.emplace_back(std::make_shared<SubmodulCombinedCalculation>());

     this->modul.emplace_back(engineeringModul);
     this->modul.push_back(agrotechnicalModul);
 }

 void CallModules::callModul(const int modul, const int submodul, const int x, const int y) {

     this->modul[modul][submodul]->setCallModules(this);
     this->modul[modul][submodul]->createSubmodul(x, y);
 }
