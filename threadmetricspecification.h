#pragma once

#ifndef THREADSPECIFICATION
#define THREADSPECIFICATION

#include <iostream>

class ThreadSpecification { // Расположение стандартные шагов резьбы.
 public:
    ThreadSpecification();
    ~ThreadSpecification();

    // Функция возвращаает шаг метрической резьбы соглсасно индексам.

    double metricThreadSpecification(const int index_bolt, const int index_thread_pitch) const;

 private:

    short index_ = 0;

    const int capacity_metric_ = 42;       // Количество типоразмеров метрического крепежа.
    const int capacity_thread_pitch_ = 6;  // Количесвто типоразмеров шагов метрического крепежа.

    double** p_metric_thread_pitch_ = nullptr;
};

#endif
