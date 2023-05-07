#ifndef SUBMODULAGRICULTURALMACHINER_H
#define SUBMODULAGRICULTURALMACHINER_H

#include <QMouseEvent>

#include "implementationmodulcalculationoftillagemachines.h"
#include "settingagriculturalsubmodul.h"

namespace Ui {
class AgriculturalMachinerCultivators;
}

class AgriculturalMachinerCultivators : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

 public:
    explicit AgriculturalMachinerCultivators(const int configuration = 0);
    ~AgriculturalMachinerCultivators();

     QPoint previousPosition();

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);

    void dataRecording();

    QPoint mPreviousPosition;
    Ui::AgriculturalMachinerCultivators *ui = nullptr;
    QIntValidator mValidator;
    AgromoduleCalculationConfiguration mConfiguration;

    CollectorData<Data::TractionCalculation, int>* pData = nullptr;

    QPixmap scheme[2] {

        QPixmap(":/new/prefix1/image_module/disk_.jpg"),
        QPixmap(":/new/prefix1/image_module/paw_cultivators_.jpg")
    };

 signals:
   void previousPositionChanged(QPoint previousPosition);
   void previousShowForm(int,int);                    // Сигнал для передачи данных позиции виджета.

 private slots:
   void setPreviousPosition (QPoint previousPosition);
   void resistanceCalculation();
   void dataOutput();
};

#endif // SUBMODULAGRICULTURALMACHINER_H
