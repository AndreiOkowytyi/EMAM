#include "stylewidgets.h"

QString SryleWidgets::styleCheifWidgets() {

    return "QWidget{"
           "background-color:#464646;"
           "}";
}

QString SryleWidgets::styleLabel() {

    return "QLabel{"
           "background-color:#F18722;"
           "color:#F18722;"
           "}";
}

QString SryleWidgets::styleLabelName() {

    return "QLabel{"
           "background-color:#464646;"
           "color:white;"
           "}";
}

QString SryleWidgets::styleButton() {

    return "QPushButton{"
           "background-color:#F18722;"
           "color:white;"
           "border: none;"
           "}"
           "QPushButton:hover{"
           "color:black;"
           "}";
}

QString SryleWidgets::styleCloseButton() {

    return "QToolButton{"
           "image: url(:/new/prefix1/window/close_passive.jpg);"
           "background-color: #464646 ;"
           "padding: 3px;"
           "margin: -1px;"
           "}"
           "QToolButton:hover{"
           "image: url(:/new/prefix1/window/close_active.jpg);"
           "margin: -1px;"
           "};";
}

QString SryleWidgets::styleColapsseButton() {

    return "QToolButton{"
           "image: url(:/new/prefix1/window/collapse_button_passive.jpg);"
           "background-color: #464646 ;"
           "padding: 3px;"
           "margin: -1px;"
           "}"
           "QToolButton:hover{"
           "image: url(:/new/prefix1/window/collapse_button_active.jpg);"
           "margin: -1px;"
           "};";
}

QString SryleWidgets::styleButtonDefault() {

    return "QPushButton{"
           "background-color:#464646;"
           "color:white;"
           "border: none;"
           "}"
           "QPushButton:hover{"
           "background-color:#373434;"
           "};";
}

QString SryleWidgets::styleButtonClicked() {

    return "QPushButton{"
           "color:white;"
           "background-color:#373434;"
           "border: none;"
           "};";
}

QString SryleWidgets::styleBackDefault() {

    return "QToolButton{"
           "border: none;"
           "color:white;"
           "image: url(:/new/prefix1/image_module/back_passive.jpg);"
           "}"
           "QToolButton:hover{"
           "image: url(:/new/prefix1/image_module/back_active.jpg);"
           "}";
}

QString SryleWidgets::styleBottonHand() {

    return "QLabel{"
           "color:white;"
           "background-color:#61AFAF;"
           "};";
}

QString SryleWidgets::styleButtonCalculation() {

    return "QPushButton{"
           "border-radius: 10px;"
           "background-color:white;"
           "color:black;"
           "border: none;"
           "}"
           "QPushButton:hover{"
           "background-color:#464646;"
           "color:white;"
           "};";
}


QString StyleWidjetsSettingsFasteners::styleButtonConfirmation() {

    return "QPushButton{"
           "background-color:#F4F5F5;"
           "color:black;"
           "border: none;"
           "}"
           "QPushButton:hover{"
           "color:#008080;"
           "}";
}

QString StyleWidjetsSettingsFasteners::styleButtonDefault() {

    return "QPushButton{"
           "background-color:#F4F5F5;"
           "color:black;"
           "border:none;"
           "}"
           "QPushButton:hover{"
           "background-color:#E9EAEA;"
           "color:black;"
           "border:none;"
           "}";
}

QString StyleWidjetsSettingsFasteners::styleButtonClicked() {

    return "QPushButton{"
           "color:black;"
           "background-color:#E9EAEA;"
           "border:none;"
           "}";
}

QString StyleWidjetsSettingsFasteners::styleBackground() {

    return "QWidget{"
           "background-color:#F4F5F5;"
           "}";
}

QString StyleWidjetsSettingsFasteners::styleSettings() {

    return "QToolButton{"
           "image: url(:/new/prefix1/image_module/setting_passive.jpg);"
           "background-color: white ;"
           "padding: 3px;"
           "margin: -1px;"
           "}"
           "QToolButton:hover{"
           "image: url(:/new/prefix1/image_module/setting_active.jpg);"
           "margin: -1px;"
           "}";
}

QString StyleWidjetsSettingsFasteners::styleImageScheme() {

    return "QLabel{image: url(:/new/prefix1/image_module/bolted_connection_01.jpg);}";
}
