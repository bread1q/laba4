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

Rectangle::Rectangle(int x, int y, int width, int height) : Shape(x, y), width_(width), height_(height) {}

bool Rectangle::contains(int x, int y) const {
    return (x >= x_ && x_ <= x + width_ && y >= y_ && y_ <= y + height_);
}

void Rectangle::draw(QPainter &painter) const {
    if (selected_) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(color_.lighter(150));
    }
    else {
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(color_);
    }

    painter.drawRect(x_, y_, width_, height_);
}

QRect Rectangle::getBorderRect() const {
    return QRect(x_, y_, width_, height_);
}

int Rectangle::getWidth() const {
    return width_;
}

int Rectangle::getHeight() const {
    return height_;
}

void Rectangle::setSize(int width, int height) {
    width_ = width;
    height_ = height;
}

Triangle::Triangle(int x, int y, int size) : Shape(x, y), size_(size) {}

bool Triangle::contains(int x, int y) const {
    // Используем ограничивающий прямоугольник с небольшим запасом
    QRect bounds = getBorderRect();

    // Если точка явно вне прямоугольника - сразу false
    if (!bounds.contains(x, y)) {
        return false;
    }

    // Простая проверка: точка выше нижней трети треугольника
    int relativeY = y - (y_ - size_/2); // Относительная Y от верха треугольника
    int triangleHeight = size_;

    // В нижней трети треугольника (где основание) - всегда true
    if (relativeY > triangleHeight * 2/3) {
        return true;
    }

    // В средней и верхней части - проверяем, не слишком ли далеко от центра
    int centerX = x_ + size_ / 2;
    int maxWidth = (triangleHeight - relativeY) * size_ / triangleHeight;

    return std::abs(x - centerX) <= maxWidth / 2;
}

void Triangle::draw(QPainter &painter) const {
    if (selected_) {
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(color_.lighter(150));
    }
    else {
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(color_);
    }

    QPoint points[3] = {
        QPoint(x_, y_ + size_ / 2),
        QPoint(x_ + size_, y_ + size_ / 2),
        QPoint(x_ + size_ / 2, y_ + size_ / 2)
    };

    painter.drawPolygon(points, 3);
}

void Triangle::getBorderRect() const {
    return QRect(x_, y_ - size / 2, size_, size_);
}

int Triangle::getSize() const {
    return size_;
}

void Triangle::setSize(int size) {
    size_ = size;
}

Line::Line(int x1, int y1, int x2, int y2, int thickness) : Shape(x1, y1), x2_(x2), y2_(y2), thickness_(thickness) {}

bool Line::contains(int x, int y) const {
    // Используем ограничивающий прямоугольник с учетом толщины линии
    int left = std::min(x_, x2_) - thickness_ - 3;
    int top = std::min(y_, y2_) - thickness_ - 3;
    int right = std::max(x_, x2_) + thickness_ + 3;
    int bottom = std::max(y_, y2_) + thickness_ + 3;

    QRect bounds(left, top, right - left, bottom - top);
    return bounds.contains(x, y);
}

void Line::draw(QPainter &painter) const {
    if (selected_) {
        painter.setPen(QPen(Qt::blue, thickness_ + 2));
    } else {
        painter.setPen(QPen(color_, thickness_));
    }

    painter.drawLine(x_, y_, x2_, y2_);
}

QRect Line::getBorderRect() const {
    int left = std::min(x_, x2_);
    int top = std::min(y_, y2_);
    int right = std::max(x_, x2_);
    int bottom = std::max(y_, y2_);

    return QRect(left, top, right - left, bottom - top);
}

int Line::getX2() const {
    return x2_;
}

int Line::getY2() const {
    return y2_;
}

int Line::getThickness() const {
    return thickness_;
}

void Line::setEndPoint(int x2, int y2) {
    x2_ = x2;
    y2_ = y2;
}

void Line::setThickness(int thickness) {
    thickness_ = thickness;
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
    return nullptr;
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
