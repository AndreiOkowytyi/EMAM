#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylewidgets.h"


MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedHeight(350);
    this->setFixedWidth(475);
    this->setWindowFlags(Qt::FramelessWindowHint); // Убираем основную рамку.
    this->setMouseTracking(true);

    // Создание кнопок второго модуля. Наименование. Позиция в виджете.

    this->pagricultural_machiner_cultivators_       = new QPushButton(this->ui->widget);
    this->pagricultural_machiner_discharrow_        = new QPushButton(this->ui->widget);
    this->pagricultural_machiner_group_calculation_ = new QPushButton(this->ui->widget);

    this->pagricultural_machiner_cultivators_->setText      ("Тяговый расчет культиватора");
    this->pagricultural_machiner_discharrow_->setText       ("Тяговый расчет лущильника");
    this->pagricultural_machiner_group_calculation_->setText("Тяговый комплексный расчет");

    pagricultural_machiner_cultivators_->setGeometry(this->ui->button_beam_calculation_->x(),
                                                     this->ui->button_beam_calculation_->y(),
                                                     this->ui->button_beam_calculation_->width(),
                                                     this->ui->button_beam_calculation_->height());

    pagricultural_machiner_discharrow_->setGeometry(this->ui->button_bolt_calculation_->x(),
                                                    this->ui->button_bolt_calculation_->y(),
                                                    this->ui->button_bolt_calculation_->width(),
                                                    this->ui->button_bolt_calculation_->height());

    pagricultural_machiner_group_calculation_->setGeometry(this->ui->button_beam_calculation_->x(),
                                                           this->ui->button_bolt_calculation_->y()+
                                                           this->ui->button_bolt_calculation_->height(),
                                                           this->ui->button_bolt_calculation_->width(),
                                                           this->ui->button_bolt_calculation_->height());
    // Выставляем стили.
    this->ui->widget       ->setStyleSheet(SryleWidgets::styleCheifWidgets());
    this->ui->botton_hand_ ->setStyleSheet(SryleWidgets::styleLabel());
    this->ui->name_program_->setStyleSheet(SryleWidgets::styleLabelName());
    this->ui->collapse_    ->setStyleSheet(SryleWidgets::styleColapsseButton());
    this->ui->close_       ->setStyleSheet(SryleWidgets::styleCloseButton());

    this->ui->button_confirmation_       ->setStyleSheet(SryleWidgets::styleButton());
    this->ui->button_agrotechnical_modul_->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->ui->button_engineering_modul_  ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->ui->button_beam_calculation_   ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->ui->button_bolt_calculation_   ->setStyleSheet(SryleWidgets::styleButtonDefault());

    this->pagricultural_machiner_cultivators_      ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->pagricultural_machiner_discharrow_       ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->pagricultural_machiner_group_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());

    // Скрываем все кнопки.
    this->configurationVisibilityEnginModul(false);
    this->configurationVisibilityAgroModul (false);

    QObject::connect(this->ui->button_engineering_modul_,   SIGNAL(clicked()), SLOT(activeConfigurationVisibilityEnginModul()));
    QObject::connect(this->ui->button_agrotechnical_modul_, SIGNAL(clicked()), SLOT(activeConfigurationVisibilityAgroModul()));
    QObject::connect(this->ui->close_, &QToolButton::clicked,    [this](){this->close();});
    QObject::connect(this->ui->collapse_, &QToolButton::clicked, [this](){this->showMinimized();});
    QObject::connect(this->ui->button_beam_calculation_, &QPushButton::clicked,[this](){

        this->m_index_calculation_ = 0;
        this->ui->button_beam_calculation_->setStyleSheet(SryleWidgets::styleButtonClicked());
        this->ui->button_bolt_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
    });
    QObject::connect(this->ui->button_bolt_calculation_, &QPushButton::clicked,[this](){

        this->m_index_calculation_ = 1;
        this->ui->button_bolt_calculation_->setStyleSheet(SryleWidgets::styleButtonClicked());
        this->ui->button_beam_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
    });
    QObject::connect(this->pagricultural_machiner_cultivators_,&QPushButton::clicked,[this](){

        this->m_index_calculation_ = 0;
        this->pagricultural_machiner_cultivators_->setStyleSheet(SryleWidgets::styleButtonClicked());
        this->pagricultural_machiner_discharrow_->setStyleSheet(SryleWidgets::styleButtonDefault());
        this->pagricultural_machiner_group_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
    });
    QObject::connect(this->pagricultural_machiner_discharrow_, &QPushButton::clicked,[this](){

        this->m_index_calculation_ = 1;
        this->pagricultural_machiner_discharrow_->setStyleSheet(SryleWidgets::styleButtonClicked());
        this->pagricultural_machiner_cultivators_->setStyleSheet(SryleWidgets::styleButtonDefault());
        this->pagricultural_machiner_group_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
    });
    QObject::connect(this->pagricultural_machiner_group_calculation_, &QPushButton::clicked,[this](){

        this->m_index_calculation_ = 2;
        this->pagricultural_machiner_group_calculation_->setStyleSheet(SryleWidgets::styleButtonClicked());
        this->pagricultural_machiner_discharrow_->setStyleSheet(SryleWidgets::styleButtonDefault());
        this->pagricultural_machiner_cultivators_->setStyleSheet(SryleWidgets::styleButtonDefault());
    });
    QObject::connect(this->ui->button_confirmation_, SIGNAL(clicked()), SLOT(callTheCalculationFunction()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPoint MainWindow::previousPosition() {

    return this->mPreviousPosition;
}

void MainWindow::setPreviousPosition(QPoint previousPosition) {

    if(this->mPreviousPosition == previousPosition)  return;
    this->mPreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
     // При клике левой кнопкой мыши.
    if (event->button() == Qt::LeftButton ) {

        setPreviousPosition(event->pos()); // Определяем, в какой области произошёл клик и устанавливаем позицию клика.

    }else if(event->button() == Qt::RightButton)  setPreviousPosition(event->pos()); // При клике правой кнопки мыши.

    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // При отпускании левой кнопки мыши сбрасываем состояние клика.
    if (event->button() == Qt::LeftButton) {

        setPreviousPosition(event->pos());

    }else if(event->button() == Qt::RightButton) return; // При отпуске правой кнопкой мыши.

    return QWidget::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    auto dx = event->x() - mPreviousPosition.x();
    auto dy = event->y() - mPreviousPosition.y();

    setGeometry(x() + dx, y() + dy, width(), height());
}

void MainWindow::configurationVisibilityEnginModul(bool visibility) {

    if(visibility) {
        this->ui->button_beam_calculation_->show();
        this->ui->button_bolt_calculation_->show();
    }else {
        this->ui->button_beam_calculation_->close();
        this->ui->button_bolt_calculation_->close();
    }
}

void MainWindow::configurationVisibilityAgroModul (bool visibility) {

    if(visibility) {
        this->pagricultural_machiner_cultivators_      ->show();
        this->pagricultural_machiner_discharrow_       ->show();
        this->pagricultural_machiner_group_calculation_->show();
    }else {
        this->pagricultural_machiner_cultivators_      ->close();
        this->pagricultural_machiner_discharrow_       ->close();
        this->pagricultural_machiner_group_calculation_->close();
    }
}

void MainWindow::activeConfigurationVisibilityEnginModul(){

    this->m_index_modul_ = 0;                   // Выставляем необходимый индекс.
    this->m_index_calculation_ = NOT_ACTIVATED; // Деактивируем индекс вызова окон.
    defaultButtonAgroModul();                   // Убираем активность кнопок агро модуля.

    this->ui->button_engineering_modul_  ->setStyleSheet(SryleWidgets::styleButtonClicked()); // Подсвечиваем кнопку.
    this->ui->button_agrotechnical_modul_->setStyleSheet(SryleWidgets::styleButtonDefault()); // Гарантируем что кнопка будет погаашена.
    this->configurationVisibilityEnginModul(true);                                            // Высвечуем кнопки машиностроительного раздела.
    this->configurationVisibilityAgroModul(false);                                            // Убирааем кнопки агро раздела.
}

void MainWindow::activeConfigurationVisibilityAgroModul(){

    this->m_index_modul_ = 1;                   // Выставляем необходимый индекс.
    this->m_index_calculation_ = NOT_ACTIVATED; // Деактивируем индекс вызова окон.
    defaultButtonEnginModul();                  // Убираем активность кнопок инженерного модуля.

    this->ui->button_agrotechnical_modul_->setStyleSheet(SryleWidgets::styleButtonClicked()); // Подсвечиваем кнопку.
    this->ui->button_engineering_modul_  ->setStyleSheet(SryleWidgets::styleButtonDefault()); // Гарантируем что кнопка будет погаашена.
    this->configurationVisibilityEnginModul(false);                                           // Высвечуем кнопки агро раздела.
    this->configurationVisibilityAgroModul(true);                                             // Убирааем кнопки машиностроительного раздела.
}

void MainWindow::defaultButtonEnginModul() {

    this->ui->button_beam_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->ui->button_bolt_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
}

void MainWindow::defaultButtonAgroModul() {

    this->pagricultural_machiner_cultivators_      ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->pagricultural_machiner_discharrow_       ->setStyleSheet(SryleWidgets::styleButtonDefault());
    this->pagricultural_machiner_group_calculation_->setStyleSheet(SryleWidgets::styleButtonDefault());
}

 void MainWindow::callTheCalculationFunction() {

    if(this->m_index_modul_ != NOT_ACTIVATED && this->m_index_calculation_ != NOT_ACTIVATED) {

      callModules.callModul(m_index_modul_, m_index_calculation_,this->x(),this->y());

      QObject::connect(&callModules, SIGNAL(position(int,int)), this, SLOT(showThisForm(int,int)));

      this->hide();
    }
    this->m_index_calculation_ = NOT_ACTIVATED;
 }

 void MainWindow::showThisForm(int x, int y) {

     defaultButtonEnginModul();
     defaultButtonAgroModul();
     this->move(x, y);
     this->show();
 }
