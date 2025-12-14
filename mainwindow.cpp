#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

Shape::Shape(int x, int y) : x_(x), y_(y), color_(Qt::red), selected_(false) {}

void Shape::move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}

// потом изменить
bool Shape::checkBorder(int minX, int minY, int maxX, int maxY) const {
    return true;
}

bool Shape::getSelected() const {
    return selected_;
}

void Shape::setSelected(bool selected) {
    selected_ = selected;
}

QColor Shape::getColor() const {
    return color_;
}

void Shape::setColor(const QColor &color) {
    color_ = color;
}

int Shape::getX() const {
    return x_;
}

int Shape::getY() const {
    return y_;
}

void Shape::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

MainWindow::~MainWindow()
{
    delete ui;
}
