#pragma once

#ifndef SETTINGBEAMCALCULATION_H
#define SETTINGBEAMCALCULATION_H

#include "ui_submodulbeamcalculation.h"

// Абстарктный класс для работы с элементами (виджетами) ввода, а также информационными виджетами,
// их видимости, в зависимости от индекса что был получен от QComboBox.

class SeсtionBeamCalculation {
 public:
    virtual ~SeсtionBeamCalculation(){}
    virtual void settingSectionForm(Ui::BeamCalculation* form) = 0;

 protected:
    void formСonfiguratioStandart(Ui::BeamCalculation* form);                       // Закрытие QComboBox, стандартных профилей балки (швеллер, двутавр, уголок).
    void formСonfigurationB      (Ui::BeamCalculation* form, bool position = true); // Выставление видимости, элементов ввода для схемы балки позиции B.
    void formСonfigurationH      (Ui::BeamCalculation* form, bool position = true); // Выставление видимости, элементов ввода для схемы балки позиции Н.
    void formConfigurationS      (Ui::BeamCalculation* form, bool position = true); // Выставление видимости, элементов ввода для схемы балки позиции S.
    void formСonfigurationAxisXY (Ui::BeamCalculation* form, bool position = true); // Выставление видимости нагрузки, по оси Х/Y.
};

class SectionBeamrRectangularPipe : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - прямоугольная труба.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamSquarePipe : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - квадратная труба.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamRoundPipe : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - круглая труба.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamCircle : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - круг.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamSquare : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - квадрат.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamLeaf : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - полоса.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamIBeam : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - двутавр.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamChannel : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - швеллер.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};

class SectionBeamCorner : public SeсtionBeamCalculation { // Работа с конфигурацией сопротивления сечения - уголок.
 public:
    void settingSectionForm(Ui::BeamCalculation* form) override;
};


class ConfigurationSection {
 public:
    ConfigurationSection();
    void configImage(const int index, Ui::BeamCalculation *temp); // Вызов необходимого полиморфного класса через полученный индекс.

 private:
    const short capacity_ = 9; // Размер вектора.
    short value_          = 0; // Работа с итератором, в функции iterator().

    // Вектор, для размешения полиморфных классов SeсtionBeamCalculation (сопротивления сечения).

    std::vector<std::unique_ptr<SeсtionBeamCalculation>>options_;

    // Работа с итератором. Функция обеспечивает заполнение вектора, с помощью итератора.

    std::vector<std::unique_ptr<SeсtionBeamCalculation>>::iterator iterator();
};


class SchemeBeamCalculation { // Поломорфный класс работы с виджетами параметров схемы балки.
 public:
    virtual ~SchemeBeamCalculation(){}
    virtual void settingSchemeForm(Ui::BeamCalculation* form) = 0;

 protected:
    void formConfigurationL1(Ui::BeamCalculation* form, bool position = true);
    void formConfigurationL2(Ui::BeamCalculation* form, bool position = true);
};

class SchemeTwoSupports: public SchemeBeamCalculation { // Работа с конфигурацией схемы балки - балка на двух опорах.
 public:
    void settingSchemeForm(Ui::BeamCalculation* form) override;
};

class SchemeTwoSupportsConsole: public SchemeBeamCalculation { // Работа с конфигурацией схемы балки - балка на двух опорах с консолью.
 public:
    void settingSchemeForm(Ui::BeamCalculation* form) override;
};

class SchemeConsole: public SchemeBeamCalculation { // Работа с конфигурацией схемы балки - балка на одной опоре (консоль).
 public:
    void settingSchemeForm(Ui::BeamCalculation* form) override;
};


class ConfigurationScheme {
 public:
    ConfigurationScheme();
    void configImage(const int index, Ui::BeamCalculation *temp); // Вызов необходимого полиморфного класса через полученный индекс.

 private:
    const short capacity_ = 3; // Размер вектора.
    short value_          = 0; // Работа с итератором, в функции iterator().

    std::vector<std::unique_ptr<SchemeBeamCalculation>>options_;
    std::vector<std::unique_ptr<SchemeBeamCalculation>>::iterator iterator();
};

#endif // SETTINGBEAMCALCULATION_H
