#include "metricfastenerpitchparameters.h"
#include "stylewidgets.h"

MetricFastenerPitchParameters::MetricFastenerPitchParameters() {

   this->ui = new Ui::SettingMetricFastenerPitch;
   ui->setupUi(this);

   this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
   this->setMouseTracking(true);

   for(short index_thread_pitch_ = 0; index_thread_pitch_ < MAX_BUTTONS; ++index_thread_pitch_) {

    thread_pitch_[index_thread_pitch_] = new QPushButton(this->ui->background_);
    thread_pitch_[index_thread_pitch_]->close();
   }

   int index_ = 0;
   auto lambdaIndex = [&index_](){return ++index_;};

   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("0.5",  index_));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("0.75", lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("1",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("1.25", lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("1.5",  lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("1.75", lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("2",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("2.5",  lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("3",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("3.5",  lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("4",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("4.5",  lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("5",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("5.5",  lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("6",    lambdaIndex()));
   m_metric_fastener_outch_index_.emplace(std::pair<QString, int>("8",    lambdaIndex()));

   this->ui->name_widget_->setStyleSheet(SryleWidgets::styleLabelName());
   this->ui->upper_band_ ->setStyleSheet(SryleWidgets::styleCheifWidgets());
   this->ui->button_ok_  ->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonConfirmation());
   this->ui->close_      ->setStyleSheet(SryleWidgets::styleCloseButton());
   this->ui->background_ ->setStyleSheet(StyleWidjetsSettingsFasteners::styleBackground());

   QObject::connect(this->thread_pitch_[0],&QPushButton::clicked,[&](){

       clickedButtonPitchFasteners(0);
       m_index_settings_fastener_pitch_ = 0;
   });

   QObject::connect(this->thread_pitch_[1],&QPushButton::clicked,[&](){

       clickedButtonPitchFasteners(1);
       m_index_settings_fastener_pitch_ = 1;
   });

   QObject::connect(this->thread_pitch_[2],&QPushButton::clicked,[&](){

       clickedButtonPitchFasteners(2);
       m_index_settings_fastener_pitch_ = 2;
   });

   QObject::connect(this->thread_pitch_[3],&QPushButton::clicked,[this](){

       clickedButtonPitchFasteners(3);
       m_index_settings_fastener_pitch_ = 3;
   });

   QObject::connect(this->thread_pitch_[4],&QPushButton::clicked,[this](){

       clickedButtonPitchFasteners(4);
       m_index_settings_fastener_pitch_ = 4;
   });

   QObject::connect(this->thread_pitch_[5],&QPushButton::clicked,[this](){

       clickedButtonPitchFasteners(5);
       m_index_settings_fastener_pitch_ = 5;
   });

   QObject::connect(this->ui->close_,     SIGNAL(clicked()), SLOT(closePitchParameters()));
   QObject::connect(this->ui->button_ok_, SIGNAL(clicked()), SLOT(closePitchParameters()));
}

MetricFastenerPitchParameters::~MetricFastenerPitchParameters() {

    delete ui;
}

QPoint MetricFastenerPitchParameters::previousPosition() {

    return this->mPreviousPosition;
}

void MetricFastenerPitchParameters::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void MetricFastenerPitchParameters::mousePressEvent(QMouseEvent *event) {
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    }else if(event->button() == Qt::RightButton)  setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void MetricFastenerPitchParameters::mouseReleaseEvent(QMouseEvent *event) {

    // При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    }else if(event->button() == Qt::RightButton) return; // При отпускании правой кнопки мыши.

    return QWidget::mouseReleaseEvent(event);
}

void MetricFastenerPitchParameters::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void MetricFastenerPitchParameters::showSettingsThreadPitch(const short x, const short y, const short index) {

    closeAllButtons();
    m_index_settings_fastener_pitch_ = 0;
    this->move(x + 110, y + 200);
    this->show();
    formationOfTableOfSteps(index);
}

void MetricFastenerPitchParameters::formationOfTableOfSteps(const int index_bolt) {

   const short x = 5; // Координата х
   short y = 6;       // Координата у

   for(int index_thread_pitch_ = 0; index_thread_pitch_ < MAX_BUTTONS; ++index_thread_pitch_) {

       double metric_thread_pitch_ = this->mThreadMetric.metricThreadSpecification(index_bolt, index_thread_pitch_);

       if(metric_thread_pitch_ == 0) break;
       else {

           if(index_thread_pitch_ == 0) thread_pitch_[index_thread_pitch_]->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonClicked());
           else thread_pitch_[index_thread_pitch_]->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonDefault());

           thread_pitch_[index_thread_pitch_]->setText(QString::number(metric_thread_pitch_));

           this->thread_pitch_[index_thread_pitch_]->setGeometry(x, y+=24, 125, 24);

           this->thread_pitch_[index_thread_pitch_]->show();
       }
   }
}

void MetricFastenerPitchParameters::closePitchParameters() {

    this->hide();
    metricFastenerPitch();
}

void MetricFastenerPitchParameters::clickedButtonPitchFasteners(const int index) {

    thread_pitch_[index]->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonClicked());
    for(short x = 0; x < MAX_BUTTONS; ++x) {

        if(x == index) continue;
         thread_pitch_[x]->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonDefault());
    }
}

void MetricFastenerPitchParameters::standartPitchOfThisMetricFastener(const int index_metric_fastener) {

    m_index_settings_fastener_pitch_ = 0;
    thread_pitch_[m_index_settings_fastener_pitch_]->setText(QString::number(mThreadMetric.metricThreadSpecification(index_metric_fastener, 0)));
    metricFastenerPitch();
}

void MetricFastenerPitchParameters::metricFastenerPitch() {

    auto result = m_metric_fastener_outch_index_.find(thread_pitch_[m_index_settings_fastener_pitch_]->text());
    emit settingsEmitFastenerPitch(result->first, result->second);
}

void MetricFastenerPitchParameters::closeAllButtons() {

    for(short x = 0; x < MAX_BUTTONS; ++x) thread_pitch_[x]->close();
}
