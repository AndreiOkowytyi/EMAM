#include "submodulbeamcalculation.h"
#include "stylewidgets.h"

BeamCalculation::BeamCalculation(QWidget *parent) :
    QWidget(parent), ui(new Ui::BeamCalculation) {

    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    this->pDataBase        = new CollectorData<Data::BeamCalculation, int>(Data::BeamCalculation::end_enum_flags_); // Создание "сборщика".
    this->pBeamCalculation = new BeamStressCalculation(*pDataBase);                                                 // Создание объекта расчета.

    QIntValidator *InputLimit_1 = new QIntValidator(0,100000,this->ui->load_beam_);
    QIntValidator *InputLimit_2 = new QIntValidator(0,10000, this->ui->field_L1_);
    QIntValidator *InputLimit_3 = new QIntValidator(0,10000, this->ui->field_L2_);
    QIntValidator *InputLimit_4 = new QIntValidator(0,1000,  this->ui->section_size_B_);
    QIntValidator *InputLimit_5 = new QIntValidator(0,1000,  this->ui->section_size_H_);
    QIntValidator *InputLimit_6 = new QIntValidator(0,1000,  this->ui->section_size_H_);

    // Устанавка органичений ввода для QLineEdit.
    this->ui->load_beam_->setValidator(InputLimit_1);
    this->ui->field_L1_ ->setValidator(InputLimit_2);
    this->ui->field_L2_ ->setValidator(InputLimit_3);
    this->ui->section_size_B_->setValidator(InputLimit_4);
    this->ui->section_size_H_->setValidator(InputLimit_5);
    this->ui->section_size_S_->setValidator(InputLimit_6);

    this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
    this->setMouseTracking(true);

    this->ui->load_x_->setChecked(true);

    // Установка стилей.
    this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
    this->ui->name_program_->setStyleSheet(SryleWidgets::styleLabelName());
    this->ui->collapse_    ->setStyleSheet(SryleWidgets::styleColapsseButton());
    this->ui->close_       ->setStyleSheet(SryleWidgets::styleCloseButton());
    this->ui->upper_band_  ->setStyleSheet(SryleWidgets::styleCheifWidgets());
    this->ui->back_        ->setStyleSheet(SryleWidgets::styleBackDefault());
    this->ui->estimation_  ->setStyleSheet(SryleWidgets::styleButtonCalculation());
    this->ui->clear_fields_->setStyleSheet(SryleWidgets::styleButtonCalculation());

    this->ui->standart_profile_Ibeam_  ->close();
    this->ui->standart_profile_channel_->close();
    this->ui->standart_profile_corner_ ->close();

    // Установка цвета, для основного виджета.
    QPalette color;
    color.setColor(this->backgroundRole(),Qt::white);
    this->setPalette(color);
    this->setAutoFillBackground(true);

    // Установка изображений для схемы балки, а также сопротивления сечения.
    this->ui->scheme_beam_image_->setPixmap(mPixBeam[0]);
    this->ui->section_image_    ->setPixmap(mProfileMaterials[0]);
    this->ui->scheme_beam_image_->setScaledContents(true);
    this->ui->section_image_    ->setScaledContents(true);

    this->ui->section_beam_            ->setToolTip("Профиль сечения балки");
    this->ui->standart_profile_Ibeam_  ->setToolTip("Номер двутавра по ГОСТ 57837-2017");
    this->ui->standart_profile_channel_->setToolTip("Номер швеллера по ГОСТ 8240-89");
    this->ui->standart_profile_corner_ ->setToolTip("Номер уголка по ГОСТ 8509-93");
    this->ui->back_                    ->setToolTip("Вернуться в главное меню");

    QObject::connect(this->ui->scheme_beam_, SIGNAL(activated(int)),SLOT(schemeBeamImages(int)));
    QObject::connect(this->ui->section_beam_,SIGNAL(activated(int)),SLOT(sectionBeamImages(int)));

    QObject::connect(this->ui->back_, &QToolButton::clicked, [this](){

        this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
        emit previousShowForm(this->x(), this->y());
        //this->hide();
        this->close();
    });
    QObject::connect(this->ui->close_,    &QToolButton::clicked, [this]() {this->close();});
    QObject::connect(this->ui->collapse_, &QToolButton::clicked, [this]() {this->showMinimized();});

    QObject::connect(this->ui->scheme_beam_, &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::scheme_option_index_, index);});

    QObject::connect(this->ui->material_,    &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::material_index_, index);});

    QObject::connect(this->ui->scheme_load_, &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::beam_load_index_, index);});

    QObject::connect(this->ui->section_beam_,&QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::section_beam_index_,index);});

    QObject::connect(this->ui->standart_profile_Ibeam_, &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::standart_profile_Ibeam_index_,index);});

    QObject::connect(this->ui->standart_profile_channel_, &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::standart_profile_channel_index_,index);});

    QObject::connect(this->ui->standart_profile_corner_, &QComboBox::activated, [this](const int index)
       {this->pDataBase->dataCollection(Data::BeamCalculation::standart_profile_corner_index_,index);});

    QObject::connect(this->ui->clear_fields_, SIGNAL(clicked()), SLOT(clearFields()));
    QObject::connect(this->ui->estimation_,   SIGNAL(clicked()), SLOT(beamLoadCalculation()));
}

BeamCalculation::~BeamCalculation() {

    delete pDataBase;
    delete pBeamCalculation;
    delete ui;
}

QPoint BeamCalculation::previousPosition() {

    return this->mPreviousPosition;
}

void BeamCalculation::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void BeamCalculation::mousePressEvent(QMouseEvent *event) {
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    }else if(event->button() == Qt::RightButton)  setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void BeamCalculation::mouseReleaseEvent(QMouseEvent *event) {

    // При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    }else if(event->button() == Qt::RightButton) return; // При отпускании правой кнопки мыши.

    return QWidget::mouseReleaseEvent(event);
}

void BeamCalculation::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void BeamCalculation::schemeBeamImages(const int index) {

    this->ui->scheme_beam_image_->clear();
    this->ui->scheme_beam_image_->setPixmap(mPixBeam[index]);
    this->ui->scheme_beam_image_->setScaledContents(true);

    this->mConfigScheme.configImage(index, this->ui);
}

void BeamCalculation::sectionBeamImages(const int index) {

    this->ui->section_image_->clear();
    this->ui->section_image_->setPixmap(mProfileMaterials[index]);
    this->ui->section_image_->setScaledContents(true);

    this->mConfigSection.configImage(index,this->ui);
}

void BeamCalculation::clearFields() {

    this->ui->load_beam_       ->setText("0");
    this->ui->field_L1_        ->setText("0");
    this->ui->field_L2_        ->setText("0");
    this->ui->section_size_B_  ->setText("0");
    this->ui->section_size_H_  ->setText("0");
    this->ui->section_size_S_  ->setText("0");
    this->ui->prop_bean_A_     ->setText("0");
    this->ui->prop_bean_B_     ->setText("0");
    this->ui->result_beam_     ->setText("0");
    this->ui->margin_of_safety_->setText("0");
}

void BeamCalculation::beamLoadCalculation() {

     // Помещение данных в сборщик.

    if(this->ui->load_x_->isChecked()) this->pDataBase->dataCollection(Data::BeamCalculation::load_X_index_, 1);
    else this->pDataBase->dataCollection(Data::BeamCalculation::load_X_index_, 0);

    if(this->ui->load_y_->isChecked()) this->pDataBase->dataCollection(Data::BeamCalculation::load_Y_index_, 1);
    else this->pDataBase->dataCollection(Data::BeamCalculation::load_Y_index_, 0);

    this->pDataBase->dataCollection(Data::BeamCalculation::load_F_,   this->ui->load_beam_->text().toInt());
    this->pDataBase->dataCollection(Data::BeamCalculation::scheme_L1_,this->ui->field_L1_ ->text().toInt());
    this->pDataBase->dataCollection(Data::BeamCalculation::scheme_L2_,this->ui->field_L2_ ->text().toInt());
    this->pDataBase->dataCollection(Data::BeamCalculation::size_B_,   this->ui->section_size_B_->text().toInt());
    this->pDataBase->dataCollection(Data::BeamCalculation::size_H_,   this->ui->section_size_H_->text().toInt());
    this->pDataBase->dataCollection(Data::BeamCalculation::size_S_,   this->ui->section_size_S_->text().toInt());

    DataValidation Validation; // Объект для проверки данных что ввел пользователь, на корректность ввода.

    if(Validation(*pDataBase)) {

        this->ui->result_beam_     ->setText(QString::number(this->pBeamCalculation->maxSressBeam()));      // Максимальная нагрузка в балке.
        this->ui->margin_of_safety_->setText(QString::number(this->pBeamCalculation->marginOfSafery()));    // Запас прочности.
        this->ui->prop_bean_A_     ->setText(QString::number(this->pBeamCalculation->reactionSupportsA())); // Реакция опоры А.
        this->ui->prop_bean_B_     ->setText(QString::number(this->pBeamCalculation->reactionSupportsB())); // Реакция опоры Б.

        this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());

    }else this->ui->botton_hand_->setStyleSheet(SryleWidgets::styleBottonHand()); // Вывод сообщения об ошибке через установку стиля.
}
