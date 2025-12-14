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

Circle::Circle(int x, int y, int radius) : Shape(x, y), radius_(radius) {}

bool Circle::contains(int x, int y) const {
    int dx = x - x_;
    int dy = y - y_;

    return (dx * dx + dy * dy) <= (radius_ * radius_);
}

void Circle::draw(QPainter &painter) const {
    if (selected_) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(color_.lighter(150));
    }
    else {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(color_);
    }

    painter.drawEllipse(x_ - radius_, y_ - radius_, 2 * radius_, 2 * radius_);
}

QRect Circle::getBorderRect() const {
    return QRect(x_ - radius_, y_ - radius_, 2 * radius_, 2 * radius_);
}

int Circle::getRadius() const {
    return radius_;
}

void Circle::setRadius(int r) {
    radius_ = r;
}

MainWindow::~MainWindow()
{
    delete ui;
}
