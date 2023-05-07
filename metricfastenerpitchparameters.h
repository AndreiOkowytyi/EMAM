#pragma once

#ifndef METRICFASTENERPITCHPARAMETERS_H
#define METRICFASTENERPITCHPARAMETERS_H

#include <QMouseEvent>
#include <QString>
#include <map>

#include "threadmetricspecification.h"   // Расположение шагов метрического крепежа.
#include "ui_settingmetricfastenerpitch.h"

#define MAX_BUTTONS 6 // Максимально возможное количество клавиш.

class MetricFastenerPitchParameters : public QWidget {

    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

 public:
    explicit MetricFastenerPitchParameters();
    ~MetricFastenerPitchParameters();

    void showSettingsThreadPitch(const short x, const short y, const short index);
    void standartPitchOfThisMetricFastener(const int index_metric_fastener);

 private:
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);
    QPoint previousPosition();

    void clickedButtonPitchFasteners(const int index);  // Метод переключения активности созданных клавиш.
    void formationOfTableOfSteps(const int index_bolt); // Метод формирования списка клавиш с обозначением шага резьбы.
    void metricFastenerPitch();                         // Метод отправляет сигнал основному окну с результатом шага резьбы.
    void closeAllButtons();                             // Метод производит закрытие всех клавиш.

    QPoint mPreviousPosition;
    QPushButton* thread_pitch_[MAX_BUTTONS];

    Ui::SettingMetricFastenerPitch* ui = nullptr;

    ThreadSpecification mThreadMetric; // Объект в котором расположены как стандартные, так и нестандартные шаги метричекого крепежа.

    short m_index_settings_fastener_pitch_ = 0; // Переменная для работы с массивом указателей thread_pitch_.

    // Расположение необходимого индекса шага резьбы для работы с вычислительным модулем.

    std::map<QString, int>m_metric_fastener_outch_index_;

 signals:
    void previousPositionChanged  (QPoint previousPosition);
    void settingsEmitFastenerPitch(QString, const int); // Передача установленного шага метрического крепежа на главное окно расчета.

 private slots:
    void setPreviousPosition(QPoint previousPosition);
    void closePitchParameters(); // Закрытие окна при нажатии "ОК" / "Закрыть".

};

#endif // METRICFASTENERPITCHPARAMETERS_H
