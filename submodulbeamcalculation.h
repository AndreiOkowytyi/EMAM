#pragma once

#ifndef BEAMCALCULATION_H
#define BEAMCALCULATION_H

#include <QMouseEvent>

#include "ui_submodulbeamcalculation.h"
#include "settingbeamcalculation.h"             // Файл для работы, с вариацией видимости виджетов "балки", и "сопротивления сечения".
#include "implementationmodulbeamcalculation.h" // Файл, с реализацией расчета балки.

namespace Ui {
class BeamCalculation;
}

class BeamCalculation : public QWidget {

    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged) // Работа с перемещением виджета.

 public:
    explicit BeamCalculation(QWidget *parent = nullptr);
    ~BeamCalculation();

     QPoint previousPosition();

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);

    Ui::BeamCalculation *ui;
    QPoint mPreviousPosition;

    QPixmap mPixBeam[3]{  // Изображения балки.

        QPixmap(":/new/prefix1/image_module/beam_1.jpg"),
        QPixmap(":/new/prefix1/image_module/beam_2.jpg"),
        QPixmap(":/new/prefix1/image_module/beam_3.jpg")
    };
    QPixmap mProfileMaterials[9]{  // Изображения сопротивления сечения балки.
        QPixmap(":/new/prefix1/image_module/section_1.jpg"),
        QPixmap(":/new/prefix1/image_module/section_2.jpg"),
        QPixmap(":/new/prefix1/image_module/section_3.jpg"),
        QPixmap(":/new/prefix1/image_module/section_4.jpg"),
        QPixmap(":/new/prefix1/image_module/section_5.jpg"),
        QPixmap(":/new/prefix1/image_module/section_6.jpg"),
        QPixmap(":/new/prefix1/image_module/section_7.jpg"),
        QPixmap(":/new/prefix1/image_module/section_8.jpg"),
        QPixmap(":/new/prefix1/image_module/section_9.jpg")
    };
    ConfigurationSection mConfigSection; // Объект вариации виджетов, для элементов сопротивления сечения.
    ConfigurationScheme  mConfigScheme;  // Объект вариации виджетов, для элементов схемы балки.

    CollectorData<Data::BeamCalculation, int> *pDataBase; // Указатель на сборщик данных что ввел пользователь.
    BeamStressCalculation *pBeamCalculation;
    // Указатель на объект основоного расчета.
signals:
   void previousPositionChanged(QPoint previousPosition);
   void previousShowForm(int,int);                    // Сигнал для передачи данных позиции виджета.

 public slots:
    void setPreviousPosition(QPoint previousPosition);

 private slots:
    void schemeBeamImages (const int index); // Слот для работы с изображением, сопротивления сечения.
    void sectionBeamImages(const int index); // Слот для работы с изображением, схема балки.
    void beamLoadCalculation();              // Слот выполнения основного расчета балки.
    void clearFields();                      // Слот очистики всех полей для ввода данных.
};

#endif // BEAMCALCULATION_H
