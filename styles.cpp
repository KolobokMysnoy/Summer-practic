#include "styles.h"

QString styles::get_button_style(){
    return "QPushButton {"
    "background-color: skyblue;"
    "border-style: outset;"
    "border-width: 2px;"
    "border-radius: 10px;"
    "border-color: stateblue;"
    "font: bold 14px;"
    "min-width: 10em;"
    "padding: 6px;"
"}";
}

QString styles::get_main_background(){
    return "background-color: lightblue;";
};

QString styles::get_line(){
    return "background-color: white;";
}
