#include "submodulboltcalculation.h"
#include "ui_submodulboltcalculation.h"
#include "stylewidgets.h"

BoltCalculation::BoltCalculation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoltCalculation)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    mInputValidator.setRange(0, 1000000); // Выставляем ограничения ввода пользователем.

    this->ui->load_fasteners_->setValidator(&mInputValidator);
    this->ui->nut_length_    ->setValidator(&mInputValidator);

    this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
    this->setMouseTracking(true);

    this->CalculationParameters = new FastenerCalculationParameters;
    this->SettingsThreadPitch   = new MetricFastenerPitchParameters;
    this->pDataBase = new CollectorData<Data::BoltCalculation, int>(Data::BoltCalculation::end_enum_flags_);

    this->ui->type_size_metric_fasteners_->setMaxVisibleItems(9);
    this->ui->number_bolt_     ->setRange(1, 1000);
    this->ui->margin_of_safety_->setRange(1, 1000);

    this->ui->shear_->setChecked(true); // Установка QRadioButton активным по умолчанию.

    // Закрытие лишних окон.
    this->ui->inch_strength_class_       ->close();
    this->ui->type_size_inch_fasteners_  ->close();
    this->ui->label_margin_of_safety_    ->close();
    this->ui->margin_of_safety_          ->close();
    this->ui->label_nut_length_          ->close();
    this->ui->nut_length_                ->close();
    this->ui->message_result_calculation_->close();

    // Установка стилей.
    this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
    this->ui->name_program_->setStyleSheet(SryleWidgets::styleLabelName());
    this->ui->collapse_    ->setStyleSheet(SryleWidgets::styleColapsseButton());
    this->ui->close_       ->setStyleSheet(SryleWidgets::styleCloseButton());
    this->ui->upper_band_  ->setStyleSheet(SryleWidgets::styleCheifWidgets());
    this->ui->back_        ->setStyleSheet(SryleWidgets::styleBackDefault());
    this->ui->estimation_  ->setStyleSheet(SryleWidgets::styleButtonCalculation());
    this->ui->clear_fields_->setStyleSheet(SryleWidgets::styleButtonCalculation());

    this->ui->settings_fasteners_calculation_->setStyleSheet(StyleWidjetsSettingsFasteners::styleSettings());
    this->ui->thread_type_                   ->setStyleSheet(StyleWidjetsSettingsFasteners::styleSettings());

    // Установка цвета, для основного виджета.

    QPalette color;
    color.setColor(this->backgroundRole(),Qt::white);
    this->setPalette(color);
    this->setAutoFillBackground(true);

    // Установка изображения по умолчанию.
    this->ui->scheme_->setPixmap(mImageScheme[0]);
    this->ui->scheme_->setScaledContents(true);

    // Установка шага резьбы метрического крепежа по умолчанию в объект сборщика.
    this->pDataBase->dataCollection(Data::BoltCalculation::step_, this->m_index_thread_pitch_);

    QObject::connect(this->ui->back_, &QToolButton::clicked, [this](){

        this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
        emit previousShowForm(this->x(), this->y());
        this->close();
    });
    QObject::connect(this->ui->close_,    &QToolButton::clicked, [this]() {this->close();});
    QObject::connect(this->ui->collapse_, &QToolButton::clicked, [this]() {this->showMinimized();});
    QObject::connect(this->ui->settings_fasteners_calculation_, &QToolButton::clicked, [this]() {

        this->CalculationParameters->showWidget();
        this->CalculationParameters->move(this->x()+10, this->y()+70);
        this->setDisabled(true);
    });
   QObject::connect(this->ui->thread_type_,&QToolButton::clicked,[this]() {

      this->SettingsThreadPitch->showSettingsThreadPitch(this->x(), this->y(), this->ui->type_size_metric_fasteners_->currentIndex());
      this->setDisabled(true);
   });

    QObject::connect(this->CalculationParameters, SIGNAL(settings(int,int,QString)), SLOT(calculationSetupResult(int,int,QString)));
    QObject::connect(this->ui->rupture_,&QRadioButton::clicked, [this]() {

        this->ui->nut_length_      ->show();
        this->ui->label_nut_length_->show();
        this->ui->scheme_->setPixmap(mImageScheme[1]);
        this->ui->scheme_->setScaledContents(true);
        this->m_type_load_fasteners_ = 2;
        this->m_bolt_load_index_ = 0;
    });

    QObject::connect(this->ui->shear_,&QRadioButton::clicked, [this](){

        this->ui->nut_length_      ->close();
        this->ui->label_nut_length_->close();
        this->ui->scheme_->setPixmap(mImageScheme[0]);
        this->ui->scheme_->setScaledContents(true);
        this->m_type_load_fasteners_ = 0;
        this->m_bolt_load_index_ = 1;
    });

    QObject::connect(this->ui->type_size_metric_fasteners_,&QComboBox::activated, [this](const int index){

        SettingsThreadPitch->standartPitchOfThisMetricFastener(index);
    });

    QObject::connect(this->SettingsThreadPitch, SIGNAL(settingsEmitFastenerPitch(QString, const int)), SLOT(threadPitchSetting(QString, const int)));
    QObject::connect(this->ui->estimation_,     SIGNAL(clicked()), SLOT(fastenerCalculation()));
    QObject::connect(this->ui->clear_fields_,   SIGNAL(clicked()), SLOT(clearFields()));
}

BoltCalculation::~BoltCalculation()
{
    delete ui;
    delete CalculationParameters;
    delete SettingsThreadPitch;
    delete pDataBase;
}

QPoint BoltCalculation::previousPosition() {

    return this->mPreviousPosition;
}

void BoltCalculation::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void BoltCalculation::mousePressEvent(QMouseEvent *event) {
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    } else if(event->button() == Qt::RightButton) setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void BoltCalculation::mouseReleaseEvent(QMouseEvent *event) {

    // При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    } else if(event->button() == Qt::RightButton) return; // При отпускании правой кнопки мыши.

    return QWidget::mouseReleaseEvent(event);
}

void BoltCalculation::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void BoltCalculation::workingWithMetricFastenerWidget(bool flag) {

    if(flag) {

        this->ui->type_size_metric_fasteners_->show();
        this->ui->thread_type_               ->show();
        this->ui->label_thread_type_         ->show();
        this->ui->label_type_size_fasteners_ ->show();
        this->ui->set_thread_pitch_          ->show();

        this->ui->margin_of_safety_      ->close();
        this->ui->label_margin_of_safety_->close();

    } else {

        this->ui->type_size_metric_fasteners_->close();
        this->ui->label_type_size_fasteners_ ->close();
        this->ui->thread_type_               ->close();
        this->ui->label_thread_type_         ->close();
        this->ui->set_thread_pitch_          ->close();

        this->ui->margin_of_safety_      ->show();
        this->ui->label_margin_of_safety_->show();
        }

    this->ui->metric_strength_class_->show();

    this->ui->inch_strength_class_     ->close();
    this->ui->type_size_inch_fasteners_->close();
}

void BoltCalculation::workingWithInchFastenerWidget(bool flag) {

    if(flag) {

        this->ui->type_size_inch_fasteners_  ->show();
        this->ui->label_type_size_fasteners_ ->show();

        this->ui->margin_of_safety_      ->close();
        this->ui->label_margin_of_safety_->close();

    } else {

        this->ui->type_size_inch_fasteners_ ->close();
        this->ui->label_type_size_fasteners_->close();

        this->ui->margin_of_safety_      ->show();
        this->ui->label_margin_of_safety_->show();
    }

    this->ui->type_size_metric_fasteners_->close();
    this->ui->metric_strength_class_     ->close();
    this->ui->thread_type_               ->close();
    this->ui->label_thread_type_         ->close();
    this->ui->set_thread_pitch_          ->close();

    this->ui->inch_strength_class_->show();
}

void BoltCalculation::workingWithWidgetCoordinators(short strength_class_y, short nut_length_y,
                                                                 short group_type_load_y, short margin_of_safety_y) {

    this->ui->label_strength_class_ ->move(this->ui->label_strength_class_->x(), strength_class_y );

    this->ui->label_nut_length_->move(this->ui->label_nut_length_->x(), nut_length_y);
    this->ui->nut_length_      ->move(this->ui->nut_length_->x(), nut_length_y);

    this->ui->group_type_load_->move(this->ui->group_type_load_->x(), group_type_load_y);

    this->ui->label_margin_of_safety_->move(this->ui->label_margin_of_safety_->x(), margin_of_safety_y);
    this->ui->margin_of_safety_      ->move(this->ui->margin_of_safety_->x(), margin_of_safety_y);
}

void BoltCalculation::calculationSetupResult(int type_fasteners, int type_calculation, QString result_settings) {

    short strength_class_y = 0;

    this->ui->result_calculation_->setText("0");

    this->m_type_fasteners_ = type_fasteners;
    this->m_calculation_bolted_connection_ = type_calculation;

    this->ui->module_design_information_->setText(result_settings);
    this->ui->settings_fasteners_calculation_->setDisabled(false);

    if(type_fasteners == 0) { // Метрический крепеж.

        if(type_calculation == 0) { // Расчет метрического крепежа.

           strength_class_y = 180;

           workingWithMetricFastenerWidget();
           workingWithWidgetCoordinators(strength_class_y, 238, 269, 339);
           this->ui->metric_strength_class_->move(this->ui->metric_strength_class_->x(),strength_class_y );
           this->ui->settlement_information_fasteners_->setText("Запас прочности крепежа");

        } else { // Подбор метрического крепежа.

           strength_class_y = 154;

           workingWithMetricFastenerWidget(false);
           workingWithWidgetCoordinators(strength_class_y, 180, 209, 281);
           this->ui->metric_strength_class_->move(this->ui->metric_strength_class_->x(),strength_class_y );
           this->ui->settlement_information_fasteners_->setText("Допустимый крепеж М");
        }

    } else { // Дюймовый крепеж.

        this->ui->inch_strength_class_     ->move(this->ui->metric_strength_class_->pos());
        this->ui->type_size_inch_fasteners_->move(this->ui->type_size_metric_fasteners_->pos());

        if(type_calculation == 0) { // Расчет дюймового крепежа.

           strength_class_y = 180;

           workingWithInchFastenerWidget();
           workingWithWidgetCoordinators(strength_class_y, 209, 240, 310);
           this->ui->inch_strength_class_->move(this->ui->metric_strength_class_->x(),strength_class_y );
           this->ui->settlement_information_fasteners_->setText("Запас прочности крепежа");

        } else { // Подбор дюймового крепежа.

           strength_class_y = 154;

           workingWithInchFastenerWidget(false);
           workingWithWidgetCoordinators(strength_class_y, 180, 209, 281);
           this->ui->inch_strength_class_->move(this->ui->metric_strength_class_->x(),strength_class_y );
           this->ui->settlement_information_fasteners_->setText("Допустимый крепеж");
        }
    }
    this->setDisabled(false); // Включение активности окна.
}

void BoltCalculation::threadPitchSetting(QString thread_pitch, const int index_thread_pitch) {

    this->ui->set_thread_pitch_->setText(thread_pitch);
    this->m_index_thread_pitch_ = index_thread_pitch;
    this->setDisabled(false); // Включение активности окна.
}

void BoltCalculation::fillDatabase() {

    this->pDataBase->dataCollection(Data::BoltCalculation::load_bolt_connectin_, this->ui->load_fasteners_->text().toInt());
    this->pDataBase->dataCollection(Data::BoltCalculation::calculation_bolted_connection_,
                                    this->m_calculation_bolted_connection_ + this->m_type_load_fasteners_);
    this->pDataBase->dataCollection(Data::BoltCalculation::type_fasteners_,  this->m_type_fasteners_);
    this->pDataBase->dataCollection(Data::BoltCalculation::bolt_load_index_, this->m_bolt_load_index_);
    this->pDataBase->dataCollection(Data::BoltCalculation::number_of_bolts_connection_, this->ui->number_bolt_->value());
    this->pDataBase->dataCollection(Data::BoltCalculation::step_, this->m_index_thread_pitch_);

    if(this->m_type_fasteners_ == 0) { // Заполнения объекта сборщика для метрического крепежа.

        this->pDataBase->dataCollection(Data::BoltCalculation::bolt_M_diametr_index_,
                                        this->ui->type_size_metric_fasteners_->currentText().remove(0,1).toInt());
        this->pDataBase->dataCollection(Data::BoltCalculation::strength_class_, this->ui->metric_strength_class_->currentIndex());

    }else { // Заполнения объекта сборщика для дюймового крепежа.

        this->pDataBase->dataCollection(Data::BoltCalculation::bolt_G_diametr_index_,
                               this->tool_to_convert_inches_to_millimeters[this->ui->type_size_inch_fasteners_->currentIndex()]);
        this->pDataBase->dataCollection(Data::BoltCalculation::bolt_G_index, this->ui->type_size_inch_fasteners_->currentIndex());
        this->pDataBase->dataCollection(Data::BoltCalculation::strength_class_,
                               this->indicator_of_the_strength_class_of_inch_fasteners[this->ui->inch_strength_class_->currentIndex()]);
    }

    if(this->m_bolt_load_index_ == 0) this->pDataBase->dataCollection(Data::BoltCalculation::length_female_screw_,
                                                                      this->ui->nut_length_->text().toInt());
    if(m_calculation_bolted_connection_) this->pDataBase->dataCollection(Data::BoltCalculation::margin_of_safary_,
                                                                      this->ui->margin_of_safety_->value());
}

void BoltCalculation::fastenerCalculation() {

    FastenersCalculation FastenersCalculation; // Объект реализации расчета.
    InputValidation      Validator;            // Объект для проверки данных на корректность ввода пользователем.
    ResultCheck          ValidatorCheck;       // Объект отслеживания исключительной ситуации.

    fillDatabase();

    if (Validator(*pDataBase)) { // Проверка данных.

            this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
            auto result = FastenersCalculation.boltConectionCalculation(*pDataBase); // Расчет.

            if (ValidatorCheck(*pDataBase)) { // Проверка на исключительные ситуации при расчете.

                this->ui->settlement_information_fasteners_->show();
                this->ui->result_calculation_              ->show();
                this->ui->message_result_calculation_      ->close();

                if (pDataBase->conclusionData(Data::BoltCalculation::type_fasteners_) == 0) { // При расчете метрического крепежа (Расчет-Подбор).

                    this->ui->result_calculation_->setText(QString::number(result));

                } else {  // При расчете дюймового крепежа.

                    if (pDataBase->conclusionData(Data::BoltCalculation::calculation_bolted_connection_) == 0 ||
                        pDataBase->conclusionData(Data::BoltCalculation::calculation_bolted_connection_) == 2) // Расчет дюймового крепежа.
                        this->ui->result_calculation_->setText(QString::number(result));

                    // Подбор дюймового крепежа.
                    else this->ui->result_calculation_->setText(QString::fromStdString(FastenersCalculation.typesizeFastenersInch()));
                }
            } else { // При возникновении исключительной ситуации при расчете.

                this->ui->settlement_information_fasteners_->close();
                this->ui->result_calculation_->close();
                this->ui->message_result_calculation_->setGeometry(this->ui->settlement_information_fasteners_->x(),
                                                                   this->ui->settlement_information_fasteners_->y(),
                                                                   this->ui->settlement_information_fasteners_->width()+30,
                                                                   this->ui->settlement_information_fasteners_->height());
                this->ui->message_result_calculation_->show();
            }
        }
        else this->ui->botton_hand_->setStyleSheet(SryleWidgets::styleBottonHand()); // При не прохождении проверки данных.
}

void BoltCalculation::clearFields() {

    this->ui->result_calculation_->setText("0");
    this->ui->nut_length_        ->setText("0");
    this->ui->load_fasteners_    ->setText("0");
}
