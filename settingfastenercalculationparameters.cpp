#include "settingfastenercalculationparameters.h"
#include "stylewidgets.h"

FastenerCalculationParameters::FastenerCalculationParameters() {

    this->ui = new  Ui::Form;
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
    this->setMouseTracking(true);

    this->resize(320,150);

    this->ui->name_widget_->setStyleSheet(SryleWidgets::styleLabelName());
    this->ui->upper_band_ ->setStyleSheet(SryleWidgets::styleCheifWidgets());

    // Установка стилей.

    this->ui->background_           ->setStyleSheet(StyleWidjetsSettingsFasteners::styleBackground());
    this->ui->type_metric_fasteners_->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonClicked());
    this->ui->calculatoin_fasteners_->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonClicked());
    this->ui->type_inch_fasteners_  ->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonDefault());
    this->ui->selection_fasteners_  ->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonDefault());


    QObject::connect(this->ui->close_,     SIGNAL(clicked()), SLOT(closeSettings()));
    QObject::connect(this->ui->button_ok_, SIGNAL(clicked()), SLOT(closeSettings()));
    QObject::connect(this->ui->type_metric_fasteners_,&QPushButton::clicked, [this]() {

        this->m_type_fasteners_ = 0;
        this->m_result_setting_[0] = "Метрический крепеж";
        changedButton(this->ui->type_metric_fasteners_, this->ui->type_inch_fasteners_);
    });

    QObject::connect(this->ui->type_inch_fasteners_,&QPushButton::clicked, [this]() {

        this->m_type_fasteners_ = 1;
        this->m_result_setting_[0] = "Дюймовый крепеж";
        changedButton(this->ui->type_inch_fasteners_, this->ui->type_metric_fasteners_);
    });

    QObject::connect(this->ui->calculatoin_fasteners_,&QPushButton::clicked, [this]() {

        this->m_type_calculation_ = 0;
        this->m_result_setting_[1] = "Расчет";
        changedButton(this->ui->calculatoin_fasteners_, this->ui->selection_fasteners_);
    });

    QObject::connect(this->ui->selection_fasteners_,&QPushButton::clicked, [this]() {

        this->m_type_calculation_ = 1;
        this->m_result_setting_[1] = "Подбор";
        changedButton(this->ui->selection_fasteners_, this->ui->calculatoin_fasteners_);
    });
}

FastenerCalculationParameters::~FastenerCalculationParameters() {

    delete ui;
}

QPoint FastenerCalculationParameters::previousPosition() {

    return this->mPreviousPosition;
}

void FastenerCalculationParameters::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void FastenerCalculationParameters::mousePressEvent(QMouseEvent *event) {
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    }else if(event->button() == Qt::RightButton)  setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void FastenerCalculationParameters::mouseReleaseEvent(QMouseEvent *event) {

    //При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    }else if(event->button() == Qt::RightButton) return; // При отпускании правой кнопки мыши.

    return QWidget::mouseReleaseEvent(event);
}

void FastenerCalculationParameters::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void FastenerCalculationParameters::closeSettings() {

    emit settings(this->m_type_fasteners_, this->m_type_calculation_, this->m_result_setting_[0] + "-" + this->m_result_setting_[1]);
    this->hide();
}

void FastenerCalculationParameters::changedButton(QPushButton* active_button, QPushButton* passively_button) {

    active_button   ->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonClicked());
    passively_button->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonDefault());
}

void FastenerCalculationParameters::showWidget() {

    this->ui->close_    ->setStyleSheet(SryleWidgets::styleCloseButton());
    this->ui->button_ok_->setStyleSheet(StyleWidjetsSettingsFasteners::styleButtonConfirmation());
    this->show();
}
