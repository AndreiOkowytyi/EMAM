#pragma once

#ifndef CALLOFCALCULATIONMODULES_H
#define CALLOFCALCULATIONMODULES_H

#include "submodulbeamcalculation.h"      // Реализация модуля расчета балки.
#include "submodulboltcalculation.h"      // Реализация модуля расчета болтового соединения.
#include "submodulagriculturalmachiner.h" // Реализация модуля расчета тягового сопротивления культиватора.

class CallModules;

class InterfaceCalling : public QWidget { // Класс общего интерфейса главного окна.
    Q_OBJECT

 public:
    virtual ~InterfaceCalling() {}
    virtual void createSubmodul(const int x, const int y) = 0;
    void setCallModules(CallModules* callModul);

 protected:
    template <class T> void callAndConnect(T* pSubmodul, const int x, const int y);
    CallModules* pCallModules = nullptr;

 signals:
    void secondaryMove(int, int); // Принимает координаты от окна при его закрытие
// и передает его сигналу класса CallModules который передает данные главному окну.
};

class EngineeringModul : public InterfaceCalling { // Абстрактный класс отвечающий за  интерфейс
// машиностроительного модуля.
 public:
    virtual ~EngineeringModul() {}
    virtual void createSubmodul(const int x, const int y) = 0;

 protected:
    BeamCalculation* pEnginModulBeamCalculation = nullptr;
    BoltCalculation* pEnginModulBoltCalculation = nullptr;
};

class SubmoduleBeamCalculation : public EngineeringModul { // Класс отвечающий за создание
// подмодуля рассчета балок.
 public:
    void createSubmodul(const int x, const int y) override;
};

class SubmoduleBoltCalculation : public EngineeringModul { // Класс отвечающий за создание
// подмодуля рассчета болтового крепежа.
 public:
     void createSubmodul(const int x, const int y) override;
};


class AgrotechnicalModul : public InterfaceCalling { // Абстрактный класс отвечающий за  интерфейс
// агротехнического модуля.
 public:
    virtual ~AgrotechnicalModul() {}
    virtual void createSubmodul(const int x, const int y) = 0;

 protected:
    AgriculturalMachinerCultivators* pAgroModulMachinerCultivators = nullptr;
};

class SubmodulCultivatorCalculation : public AgrotechnicalModul { // Класс отвечающий за создание
// подмодуля рассчета тягового усилия культиватора.
 public:
    void createSubmodul(const int x, const int y) override;
};

class SubmodulDiscatorCalculation : public AgrotechnicalModul { // Класс отвечающий за создание
// подмодуля рассчета тягового усилия лущильника.
 public:
    void createSubmodul(const int x, const int y) override;
};

class SubmodulCombinedCalculation : public AgrotechnicalModul { // Класс отвечающий за создание
// подмодуля рассчета комплексного тягового усилия.
 public:
   void createSubmodul(const int x, const int y) override;
};


class CallModules : public QWidget { // Класс отвечающий за унификацию интерфейса. (Паттерн фасад)
    Q_OBJECT
 public:
    CallModules();
    void callModul(const int modul, const int submodul, const int x, const int y); // Вызов необходимого окна
    // на основе двухмерного массива.

 private:
    std::vector<std::shared_ptr<InterfaceCalling>>engineeringModul;
    std::vector<std::shared_ptr<InterfaceCalling>>agrotechnicalModul;
    std::vector<std::vector<std::shared_ptr<InterfaceCalling>>>modul;

 signals:
    void position(int,int);
};

#endif // CALLOFCALCULATIONMODULES_H
