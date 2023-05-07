#include "submodulagriculturalmachiner.h"
#include "ui_submodulagriculturalmachiner.h"
#include "stylewidgets.h"


AgriculturalMachinerCultivators::AgriculturalMachinerCultivators(const int configuration) :
    ui(new Ui::AgriculturalMachinerCultivators)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    if(configuration != 2)this->mConfiguration.configuration(configuration, this->ui);

    this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
    this->setMouseTracking(true);

    pData = new CollectorData<Data::TractionCalculation, int>(Data::TractionCalculation::end_enum_flags_);
    pData->dataCollection(Data::TractionCalculation::calculation_type_index_, configuration);

    // Установка стилей.
    this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
    this->ui->name_program_->setStyleSheet(SryleWidgets::styleLabelName());
    this->ui->collapse_    ->setStyleSheet(SryleWidgets::styleColapsseButton());
    this->ui->close_       ->setStyleSheet(SryleWidgets::styleCloseButton());
    this->ui->upper_band_  ->setStyleSheet(SryleWidgets::styleCheifWidgets());
    this->ui->back_        ->setStyleSheet(SryleWidgets::styleBackDefault());
    this->ui->estimation_  ->setStyleSheet(SryleWidgets::styleButtonCalculation());
    this->ui->clear_fields_->setStyleSheet(SryleWidgets::styleButtonCalculation());

    // Установка цвета, для основного виджета.

    QPalette color;
    color.setColor(this->backgroundRole(),Qt::white);
    this->setPalette(color);
    this->setAutoFillBackground(true);

    mValidator.setRange(0, 10000);

    this->ui->total_weight_->setValidator(&mValidator);
    this->ui->lancet_paw_width_->setValidator(&mValidator);
    this->ui->diametr_rink_->setValidator(&mValidator);

    this->ui->quantity_paw_width_->setRange(0, 1000);
    this->ui->attack_angle_->setRange(0, 90);
    this->ui->attack_angle_->setValue(15);
    this->ui->quantity_disc_->setRange(0, 1000);
    this->ui->speed_->setRange(1, 40);
    this->ui->speed_->setValue(12);
    this->ui->processing_depth_lancet_paw_->setRange(1, 2000);
    this->ui->processing_depth_lancet_paw_->setValue(120);
    this->ui->grip_width_->setRange(1, 100);

    this->ui->availability_of_ice_rinks_->setChecked(true);

    QLabel* scheme_disk = new QLabel(this->ui->groupBox_Disc_);
    scheme_disk->setGeometry(100, 35, 162, 141);

    scheme_disk->setPixmap(this->scheme[0]);
    this->ui->scheme_lancet_paw_->setPixmap(this->scheme[1]);

    scheme_disk->setScaledContents(true);

    this->ui->scheme_lancet_paw_->setScaledContents(true);

    QObject::connect(this->ui->estimation_, SIGNAL(clicked()), SLOT(resistanceCalculation()));
    QObject::connect(this->ui->back_, &QToolButton::clicked, [this](){

        this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
        emit previousShowForm(this->x(), this->y());
        this->close();
    });
    QObject::connect(this->ui->close_,    &QToolButton::clicked, [this]() {this->close();});
    QObject::connect(this->ui->collapse_, &QToolButton::clicked, [this]() {this->showMinimized();});
    QObject::connect(this->ui->clear_fields_, SIGNAL(clicked()), this, SLOT(dataOutput()));
}

AgriculturalMachinerCultivators::~AgriculturalMachinerCultivators()
{
    delete ui;
    delete pData;
}

QPoint AgriculturalMachinerCultivators::previousPosition() {

    return this->mPreviousPosition;
}

void AgriculturalMachinerCultivators::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void AgriculturalMachinerCultivators::mousePressEvent(QMouseEvent *event) {
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    } else if(event->button() == Qt::RightButton) setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void AgriculturalMachinerCultivators::mouseReleaseEvent(QMouseEvent *event) {

    // При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    } else if(event->button() == Qt::RightButton) return; // При отпускании правой кнопки мыши.

    return QWidget::mouseReleaseEvent(event);
}

void AgriculturalMachinerCultivators::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void AgriculturalMachinerCultivators::dataRecording() {

    if(this->ui->availability_of_ice_rinks_->isChecked())
        pData->dataCollection(Data::TractionCalculation::flag_roller_, 1);
    else pData->dataCollection(Data::TractionCalculation::flag_roller_, 0);

    this->pData->dataCollection(Data::TractionCalculation::implement_working_width_, this->ui->grip_width_->value());
    this->pData->dataCollection(Data::TractionCalculation::total_weight_of_the_unit_,
                                this->ui->total_weight_->text().toInt());
    this->pData->dataCollection(Data::TractionCalculation::maximum_working_speed_, this->ui->speed_->value());
    this->pData->dataCollection(Data::TractionCalculation::maximum_working_depth_,
                                this->ui->processing_depth_lancet_paw_->value());

    this->pData->dataCollection(Data::TractionCalculation::soil_type_, this->ui->soil_type_->currentIndex());

    if(pData->conclusionData(Data::TractionCalculation::calculation_type_index_) == 0 ||
            pData->conclusionData(Data::TractionCalculation::calculation_type_index_) == 2){

        this->pData->dataCollection(Data::TractionCalculation::paw_width_, this->ui->lancet_paw_width_->text().toInt());
        this->pData->dataCollection(Data::TractionCalculation::number_of_paws_, this->ui->quantity_paw_width_->value());
    }

    if(pData->conclusionData(Data::TractionCalculation::calculation_type_index_) == 1 ||
            pData->conclusionData(Data::TractionCalculation::calculation_type_index_) == 2){

        this->pData->dataCollection(Data::TractionCalculation::diametr_disc_, this->ui->diametr_rink_->text().toInt());
        this->pData->dataCollection(Data::TractionCalculation::angle_of_attack_of_the_disc_,
                                    this->ui->attack_angle_->text().toInt());
        this->pData->dataCollection(Data::TractionCalculation::number_of_disc_, this->ui->quantity_disc_->value());
    }
}

void AgriculturalMachinerCultivators::resistanceCalculation() {

    InputValidationCalculation Validation; // Проверка данных что ввел пользователь.
    TractionCategory Category;             // Определение тяговой категории трактора.
    TractionForceCalculation Calculation;  // Объект расчета.
    CalculationFaucet Faucet;              // Объект, отвечающий за сборку результатов расчета.

    dataRecording();

    if(Validation.validation(*pData)) {

        Calculation.requiredPowerOfTheTractionUnit(*pData, Faucet);

        this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());

        this->ui->result_traction_resistance_->setText(
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::full_traction_resistance_).minCount())
                    + " - " +
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::full_traction_resistance_).maxCount())
                    );

        this->ui->result_power_tractor_->setText(
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::horsepower_).minCount())
                    + " - " +
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::horsepower_).maxCount())
                    );

        this->ui->result_resistance_lancet_paw_->setText(
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::
                                                             resistance_of_one_boom_share_for_tillage_).minCount())
                    + " - " +
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::
                                                             resistance_of_one_boom_share_for_tillage_).maxCount())
                    );

        this->ui->result_resistance_rink_->setText(
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::resistance_of_one_tillage_disc_).minCount())
                    + " - " +
                    QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::resistance_of_one_tillage_disc_).maxCount())
                    );

        this->ui->result_traction_category_->setText(
                    QString::fromStdString (
                    Category.requiredTractionCategory(Faucet.resultCalculation(CalculationFaucet::TypePair::power_KW_).minCount())
                        .minCount()) + " " +
                    QString::fromStdString(Category.requiredTractionCategory(Faucet.resultCalculation(CalculationFaucet::TypePair::
                                                                                                      power_KW_).maxCount()).maxCount())
                    );

        this->ui->result_power_kW_->setText(
                     QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::power_KW_).minCount()) + " - " +
                     QString::number(Faucet.resultCalculation(CalculationFaucet::TypePair::power_KW_).maxCount())
                    );

    } else this->ui->botton_hand_->setStyleSheet(SryleWidgets::styleBottonHand());
}

void AgriculturalMachinerCultivators::dataOutput() {

    this->ui->lancet_paw_width_->setText("0");
    this->ui->diametr_rink_    ->setText("0");
    this->ui->total_weight_    ->setText("0");

    this->ui->result_traction_resistance_  ->setText("0");
    this->ui->result_power_tractor_        ->setText("0");
    this->ui->result_power_kW_             ->setText("0");
    this->ui->result_traction_category_    ->setText("0");
    this->ui->result_resistance_lancet_paw_->setText("0");
    this->ui->result_resistance_rink_      ->setText("0");
}
