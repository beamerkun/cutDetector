#include "main_window.h"
#include "ui_main_window.h"
#include "opencv_image_widget.h"

main_window::main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_window)
{
    ui->setupUi(this);
}

main_window::~main_window()
{
    delete ui;
}
