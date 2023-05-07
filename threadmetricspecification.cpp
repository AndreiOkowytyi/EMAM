#include "threadmetricspecification.h"

ThreadSpecification::ThreadSpecification() {

    auto index =[&]() { return ++index_; };

    p_metric_thread_pitch_ = new double* [this->capacity_metric_];

    p_metric_thread_pitch_[this->index_] = new double [this->capacity_thread_pitch_]{ 1,    0.5,  0.75, 0,   0,    0 };   // M6
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 1.25, 0.5,  0.75, 1,   0,    0 };   // M8
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 1.5,  0.5,  0.75, 1,   1.25, 0 };   // M10
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 1.75, 0.5,  0.75, 1,   1.25, 1.5 }; // M12
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 2,    0.5,  0.75, 1,   1.25, 1.5 }; // M14
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 2,    0.5,  0.75, 1,   1.5,  0 };   // M16
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 2.5,  0.5,  0.75, 1,   1.5,  2 };   // M18
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 2.5,  0.5,  0.75, 1,   1.5,  2 };   // M20
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 2.5,  0.5,  0.75, 1,   1.5,  2 };   // M22
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 3,    0.75, 1,    1.5, 2,    0 };   // M24
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 3,    0.75, 1,    1.5, 2,    0 };   // M27
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 3.5,  0.75, 1,    1.5, 2,    3 };   // M30
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 3.5,  0.75, 1,    1.5, 2,    3 };   // M33
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1,    1.5,  2,   3,    0 };   // M36
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1.5,  1.5,  2,   3,    0 };   // M39
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 3,    1.5,  2,    0,   0,    0 };   // M40
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4.5,  1,    1.5,  2,   3,    4 };   // M42
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4.5,  1,    1.5,  2,   3,    4 };   // M45
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 5,    1,    1.5,  2,   3,    4 };   // M48
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 5,    1,    1.5,  2,   3,    4 };   // M52
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1.5,  2,    3,   0,    0 };   // M55
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 5.5,  1,    1.5,  2,   3,    4 };   // M56
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 5.5,  1,    1.5,  2,   3,    4 };   // M60
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1.5,  2,    3,   0,    0 };   // M62
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1,    1.5,  2,   3,    4 };   // M64
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1.5,  2,    3,   0,    0 };   // M65
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1,    1.5,  2,   3,    4 };   // M68
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M70
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1,    1.5,  2,   3,    4 };   // M72
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 4,    1.5,  2,    3,   0,    0 };   // M75
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1,    1.5,  2,   3,    4 };   // M76
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1,    1.5,  2,   3,    4 };   // M80
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M85
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M90
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M95
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M100
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M105
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M110
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M115
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 6,    1.5,  2,    3,   4,    0 };   // M120
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 8,    1.5,  2,    3,   4,    6 };   // M125
    p_metric_thread_pitch_[index()] = new double [this->capacity_thread_pitch_]{ 8,    1.5,  2,    3,   4,    6 };   // M130
}

ThreadSpecification::~ThreadSpecification() {

    for (short index = 0; index < this->capacity_metric_; ++index) delete[] p_metric_thread_pitch_[index];
    delete[] p_metric_thread_pitch_;
}

double ThreadSpecification::metricThreadSpecification(const int index_bolt, const int index_thread_pitch) const {

    if (index_bolt >= this->capacity_metric_ || index_thread_pitch >= this->capacity_thread_pitch_) exit(1);
    return p_metric_thread_pitch_[index_bolt][index_thread_pitch];
}
