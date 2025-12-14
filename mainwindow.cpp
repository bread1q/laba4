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

ShapeContainer::ShapeContainer() {}

ShapeContainer::~ShapeContainer() {
    clear();
}

void ShapeContainer::addShape(Shape *shape) {
    if (shape != nullptr) shapes_.push_back(shape);
}

void ShapeContainer::removeShape(int i) {
    if (i >= 0 && i <= shapes_.size()) {
        delete shapes_[i];
        shapes_.erase(shapes_.begin() + i);
    }
}

void ShapeContainer::clear() {
    for (Shape* shape : shapes_) {
        delete shape;
    }
    shapes_.clear();
}

void ShapeContainer::clearSelection() {
    for (Shape* shape : shapes_) {
        shape->setSelected(false);
    }
}

void ShapeContainer::removeSelected() {
    for (int i = shapes_.size() - 1; i >= 0; i--) {
        if (shapes_[i]->getSelected()) {
            delete shapes_[i];
            shapes_.erase(shapes_.begin() + i)
        }
    }
}

void ShapeContainer::selectAll() {
    for (Shape* shape : shapes_) {
        shape->setSelected(true);
    }
}

Shape* ShapeContainer::getShape(int i) const {
    if (i >= 0 && i <= shapes_.size()) {
        return shapes_[i];
    }
}

int ShapeContainer::getCount() const {
    return shapes_.size();
}

void ShapeContainer::moveSelected(int dx, int dy) {
    for (Shape* shape : shapes_) {
        if (shape->getSelected()) {
            shape->move(dx, dy);
        }
    }
}

void ShapeContainer::setSelectedColor(const QColor &color) {
    for (Shape* shape : shapes_) {
        if (shape->getSelected()) {
            shape->setColor(color);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
