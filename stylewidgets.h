#pragma once

#ifndef STYLEWIDGETS_H
#define STYLEWIDGETS_H

#include <QString>

class  SryleWidgets { // Общие стили, что касаються всех форм.
 public:
    static QString styleCheifWidgets();      // Стиль основнго виджета.
    static QString styleLabel();             // Стиль нижней оранжевой полосы.
    static QString styleLabelName();         // Стиль наименования.
    static QString styleButton();            // Стиль кнопки ОК по умолчанию
    static QString styleCloseButton();       // Стиль кнопки закрыть по умолчанию.
    static QString styleColapsseButton();    // Стиль кнопки свернуть
    static QString styleButtonDefault();     // Стиль обычных кнопок по умолчанию
    static QString styleButtonClicked();     // Стиль обычных кнопки при нажатии.
    static QString styleBackDefault();       // Стиль кнопки назад.
    static QString styleBottonHand();        // Стиль нижней полосы модуля BeamCalculation.
    static QString styleButtonCalculation(); // Стиль кнопки произведения расчетов, и очистки полей ввода.
};

class StyleWidjetsSettingsFasteners { // Стили что касаються подмодуля расчета крепежа.
 public:
    static QString styleButtonClicked();      // Стиль  кнопки при нажатии.
    static QString styleButtonDefault();      // Стиль обычных кнопок по умолчанию
    static QString styleButtonConfirmation(); // Стиль кнопки "ОК".
    static QString styleBackground();         // Стиль главного виджета.
    static QString styleSettings();           // Стиль кнопки настройки расчета крепежа.
    static QString styleImageScheme();        // Изминения изображения схемы расчета крепежа.
};

#endif // STYLEWIDGETS_H
