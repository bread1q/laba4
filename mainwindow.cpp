#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <algorithm>
#include <cmath>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Визуальный редактор - Круг (1)");
    resize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
    createMenu();
    createToolBar();
    updateWindowTitle();
}

void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("Файл");

    QAction *clearAction = new QAction("Очистить окно", this);
    clearAction->setShortcut(QKeySequence::New);
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearWindow);
    fileMenu->addAction(clearAction);

    QAction *exitAction = new QAction("Выход", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QMenu *shapesMenu = menuBar()->addMenu("Фигуры");

    QAction *circleAction = new QAction("Круг", this);
    circleAction->setShortcut(Qt::Key_1);
    connect(circleAction, &QAction::triggered, this, &MainWindow::selectCircle);
    shapesMenu->addAction(circleAction);

    QAction *rectAction = new QAction("Прямоугольник", this);
    rectAction->setShortcut(Qt::Key_2);
    connect(rectAction, &QAction::triggered, this, &MainWindow::selectRectangle);
    shapesMenu->addAction(rectAction);

    QAction *squareAction = new QAction("Квадрат", this);
    squareAction->setShortcut(Qt::Key_3);
    connect(squareAction, &QAction::triggered, this, &MainWindow::selectSquare);
    shapesMenu->addAction(squareAction);

    QAction *triangleAction = new QAction("Треугольник", this);
    triangleAction->setShortcut(Qt::Key_4);
    connect(triangleAction, &QAction::triggered, this, &MainWindow::selectTriangle);
    shapesMenu->addAction(triangleAction);

    QAction *lineAction = new QAction("Линия", this);
    lineAction->setShortcut(Qt::Key_5);
    connect(lineAction, &QAction::triggered, this, &MainWindow::selectLine);
    shapesMenu->addAction(lineAction);

    QMenu *editMenu = menuBar()->addMenu("Правка");

    QAction *deleteAction = new QAction("Удалить", this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, [this]() {
        shapes_.removeSelected();
        update();
    });
    editMenu->addAction(deleteAction);

    QAction *selectAllAction = new QAction("Выделить все", this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, [this]() {
        shapes_.selectAll();
        update();
    });
    editMenu->addAction(selectAllAction);

    QAction *increaseSizeAction = new QAction("Увеличить размер", this);
    increaseSizeAction->setShortcut(Qt::CTRL | Qt::Key_Plus);
    connect(increaseSizeAction, &QAction::triggered, this, &MainWindow::increaseSize);
    editMenu->addAction(increaseSizeAction);

    QAction *decreaseSizeAction = new QAction("Уменьшить размер", this);
    decreaseSizeAction->setShortcut(Qt::CTRL | Qt::Key_Minus);
    connect(decreaseSizeAction, &QAction::triggered, this, &MainWindow::decreaseSize);
    editMenu->addAction(decreaseSizeAction);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar("Инструменты");
    toolBar->setMovable(false);

    // Текстовые кнопки вместо иконок
    QAction *circleAction = new QAction("Круг", this);
    circleAction->setToolTip("Круг - клавиша 1");
    connect(circleAction, &QAction::triggered, this, &MainWindow::selectCircle);
    toolBar->addAction(circleAction);

    QAction *rectAction = new QAction("Прямоугольник", this);
    rectAction->setToolTip("Прямоугольник - клавиша 2");
    connect(rectAction, &QAction::triggered, this, &MainWindow::selectRectangle);
    toolBar->addAction(rectAction);

    QAction *squareAction = new QAction("Квадрат", this);
    squareAction->setToolTip("Квадрат - клавиша 3");
    connect(squareAction, &QAction::triggered, this, &MainWindow::selectSquare);
    toolBar->addAction(squareAction);

    QAction *triangleAction = new QAction("Треугольник", this);
    triangleAction->setToolTip("Треугольник - клавиша 4");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::selectTriangle);
    toolBar->addAction(triangleAction);

    QAction *lineAction = new QAction("Линия", this);
    lineAction->setToolTip("Линия - клавиша 5");
    connect(lineAction, &QAction::triggered, this, &MainWindow::selectLine);
    toolBar->addAction(lineAction);

    toolBar->addSeparator();

    QAction *colorAction = new QAction("Цвет", this);
    colorAction->setToolTip("Изменить цвет выделенных фигур (Ctrl+C)");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);
    toolBar->addAction(colorAction);

    QAction *deleteAction = new QAction("Удалить", this);
    deleteAction->setToolTip("Удалить выделенные фигуры (Delete)");
    connect(deleteAction, &QAction::triggered, [this]() {
        shapes_.removeSelected();
        update();
    });
    toolBar->addAction(deleteAction);

    QAction *clearAction = new QAction("Очистить", this);
    clearAction->setToolTip("Очистить весь холст");
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearWindow);
    toolBar->addAction(clearAction);

    toolBar->addSeparator();

    QAction *increaseAction = new QAction("Увеличить", this);
    increaseAction->setToolTip("Увеличить размер фигур (Ctrl++)");
    connect(increaseAction, &QAction::triggered, this, &MainWindow::increaseSize);
    toolBar->addAction(increaseAction);

    QAction *decreaseAction = new QAction("Уменьшить", this);
    decreaseAction->setToolTip("Уменьшить размер фигур (Ctrl+-)");
    connect(decreaseAction, &QAction::triggered, this, &MainWindow::decreaseSize);
    toolBar->addAction(decreaseAction);
}

void MainWindow::selectCircle() {
    currentShapeType_ = CIRCLE;
    updateWindowTitle();
}

void MainWindow::selectRectangle() {
    currentShapeType_ = RECTANGLE;
    updateWindowTitle();
}

void MainWindow::selectSquare() {
    currentShapeType_ = SQUARE;
    updateWindowTitle();
}

void MainWindow::selectTriangle() {
    currentShapeType_ = TRIANGLE;
    updateWindowTitle();
}

void MainWindow::selectLine() {
    currentShapeType_ = LINE;
    updateWindowTitle();
}

void MainWindow::changeColor() {
    // Используем стандартный диалог выбора цвета
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle("Выберите цвет");
    colorDialog.setCurrentColor(Qt::red); // Цвет по умолчанию

    if (colorDialog.exec() == QDialog::Accepted) {
        QColor color = colorDialog.selectedColor();
        shapes_.setSelectedColor(color);
        update();
    }
}

void MainWindow::clearWindow() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Очистка холста",
                                  "Вы уверены, что хотите удалить все фигуры?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        shapes_.clear();
        update();
    }
}

void MainWindow::increaseSize() {
    resizeSelected(5);
}

void MainWindow::decreaseSize() {
    resizeSelected(-5);
}

void MainWindow::updateWindowTitle() {
    QString shapeName;
    switch (currentShapeType_) {
    case CIRCLE: shapeName = "Круг"; break;
    case RECTANGLE: shapeName = "Прямоугольник"; break;
    case SQUARE: shapeName = "Квадрат"; break;
    case TRIANGLE: shapeName = "Треугольник"; break;
    case LINE: shapeName = "Линия"; break;
    }

    setWindowTitle("Визуальный редактор - " + shapeName);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);

    for (int i = 0; i < shapes_.getCount(); i++) {
        Shape* shape = shapes_.getShape(i);
        if (shape) {
            shape->draw(painter);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if (event->button() == Qt::LeftButton) {
        bool ctrlPressed = event->modifiers() & Qt::ControlModifier;

        Shape* clickedShape = nullptr;

        // Находим фигуру под курсором
        for (int i = shapes_.getCount() - 1; i >= 0; i--) {
            Shape* shape = shapes_.getShape(i);
            if (shape && shape->contains(x, y)) {
                clickedShape = shape;
                break;
            }
        }

        if (clickedShape) {
            // Кликнули на существующую фигуру
            if (ctrlPressed) {
                clickedShape->setSelected(!clickedShape->getSelected());
            } else {
                shapes_.clearSelection();
                clickedShape->setSelected(true);
            }
        } else {
            // Кликнули на пустое место - создаем новую фигуру
            if (!ctrlPressed) {
                shapes_.clearSelection();
            }

            Shape* newShape = nullptr;
            int margin = 20; // Минимальный отступ от края

            // Проверяем, чтобы новая фигура помещалась в окно
            switch (currentShapeType_) {
            case CIRCLE: {
                int radius = 20;
                if (x - radius >= margin && x + radius <= width() - margin &&
                    y - radius >= margin && y + radius <= height() - margin) {
                    newShape = new Circle(x, y, radius);
                } else {
                    // Фигура не помещается - создаем у края
                    int safeX = std::max(margin + radius, std::min(x, width() - margin - radius));
                    int safeY = std::max(margin + radius, std::min(y, height() - margin - radius));
                    newShape = new Circle(safeX, safeY, radius);
                }
                break;
            }

            case RECTANGLE: {
                int rectWidth = 50;
                int rectHeight = 30;
                int safeX = std::max(margin, std::min(x - rectWidth/2, width() - margin - rectWidth));
                int safeY = std::max(margin, std::min(y - rectHeight/2, height() - margin - rectHeight));
                newShape = new Rectangle(safeX, safeY, rectWidth, rectHeight);
                break;
            }

            case SQUARE: {
                int size = 40;
                int safeX = std::max(margin, std::min(x - size/2, width() - margin - size));
                int safeY = std::max(margin, std::min(y - size/2, height() - margin - size));
                newShape = new Square(safeX, safeY, size);
                break;
            }

            case TRIANGLE: {
                int size = 40;
                int safeX = std::max(margin, std::min(x - size/2, width() - margin - size));
                int safeY = std::max(margin + size/2, std::min(y, height() - margin - size/2));
                newShape = new Triangle(safeX, safeY, size);
                break;
            }

            case LINE: {
                int length = 40;
                int safeX1 = std::max(margin, std::min(x - length/2, width() - margin - length));
                int safeY1 = std::max(margin, std::min(y, height() - margin));
                int safeX2 = safeX1 + length;
                int safeY2 = safeY1;
                newShape = new Line(safeX1, safeY1, safeX2, safeY2);
                break;
            }
            }

            if (newShape) {
                if (!ctrlPressed) {
                    newShape->setSelected(true);
                }
                shapes_.addShape(newShape);
            }
        }

        update();
    }
    else if (event->button() == Qt::RightButton) {
        shapes_.clearSelection();
        update();
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool needUpdate = false;
    int dx = 0, dy = 0;

    // Определяем направление перемещения и изменение размера
    switch (event->key()) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        shapes_.removeSelected();
        needUpdate = true;
        break;

    case Qt::Key_1:
        currentShapeType_ = CIRCLE;
        updateWindowTitle();
        break;

    case Qt::Key_2:
        currentShapeType_ = RECTANGLE;
        updateWindowTitle();
        break;

    case Qt::Key_3:
        currentShapeType_ = SQUARE;
        updateWindowTitle();
        break;

    case Qt::Key_4:
        currentShapeType_ = TRIANGLE;
        updateWindowTitle();
        break;

    case Qt::Key_5:
        currentShapeType_ = LINE;
        updateWindowTitle();
        break;

    case Qt::Key_Left:
        dx = -5;
        break;

    case Qt::Key_Right:
        dx = 5;
        break;

    case Qt::Key_Up:
        dy = -5;
        break;

    case Qt::Key_Down:
        dy = 5;
        break;

    case Qt::Key_Escape:
        shapes_.clearSelection();
        needUpdate = true;
        break;

    case Qt::Key_A:
        if (event->modifiers() & Qt::ControlModifier) {
            shapes_.selectAll();
            needUpdate = true;
        }
        break;

    // Изменение размера фигур
    case Qt::Key_Plus:
    case Qt::Key_Equal:  // На некоторых клавиатурах + на той же клавише, что и =
        if (event->modifiers() & Qt::ControlModifier) {
            resizeSelected(5);  // Увеличить на 5 пикселей
            needUpdate = true;
        }
        break;

    case Qt::Key_Minus:
        if (event->modifiers() & Qt::ControlModifier) {
            resizeSelected(-5);  // Уменьшить на 5 пикселей
            needUpdate = true;
        }
        break;
    }

    // Если нужно переместить фигуры
    if (dx != 0 || dy != 0) {
        // Получаем границы окна
        int left = 0;
        int top = 0;
        int right = width();
        int bottom = height();

        // Проверяем, можно ли переместить ВСЕ выделенные фигуры
        bool canMoveAll = true;

        // Сначала проверяем все фигуры
        for (int i = 0; i < shapes_.getCount(); i++) {
            Shape* shape = shapes_.getShape(i);
            if (shape && shape->getSelected()) {
                // Временно перемещаем для проверки
                shape->move(dx, dy);
                if (!shape->checkBounds(left, top, right, bottom)) {
                    canMoveAll = false;
                }
                // Возвращаем назад
                shape->move(-dx, -dy);
            }
        }

        // Если ВСЕ фигуры могут переместиться - перемещаем их
        if (canMoveAll) {
            for (int i = 0; i < shapes_.getCount(); i++) {
                Shape* shape = shapes_.getShape(i);
                if (shape && shape->getSelected()) {
                    shape->move(dx, dy);
                }
            }
            needUpdate = true;
        }
    }

    if (needUpdate) {
        update();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeSelected(int delta) {
    // Проверяем, не находятся ли выделенные фигуры слишком близко к границе
    int margin = 3; // Минимальный отступ от края

    bool canResize = true;

    // Сначала проверяем все выделенные фигуры
    for (int i = 0; i < shapes_.getCount(); i++) {
        Shape* shape = shapes_.getShape(i);
        if (shape && shape->getSelected()) {
            QRect bounds = shape->getBorderRect();

            if (delta > 0) {
                // УВЕЛИЧЕНИЕ размера - проверяем, не станет ли фигура слишком большой
                QRect testBounds = bounds.adjusted(-delta, -delta, delta, delta);

                if (testBounds.left() < margin || testBounds.right() > width() - margin ||
                    testBounds.top() < margin || testBounds.bottom() > height() - margin) {
                    canResize = false;
                    break;
                }
            } else {
                // УМЕНЬШЕНИЕ размера - проверяем только текущие границы
                // (если фигура уже у границы, уменьшать можно)
                if (bounds.left() < margin || bounds.right() > width() - margin ||
                    bounds.top() < margin || bounds.bottom() > height() - margin) {
                    // Фигура у границы, но уменьшать можно
                    // Дополнительная проверка: после уменьшения фигура не должна исчезнуть
                    QRect testBounds = bounds.adjusted(delta, delta, -delta, -delta);

                    // Проверяем минимальный размер
                    if (testBounds.width() < 10 || testBounds.height() < 10) {
                        canResize = false;
                        break;
                    }
                }
            }
        }
    }

    // Если можно изменить размер - применяем изменения
    if (canResize) {
        for (int i = 0; i < shapes_.getCount(); i++) {
            Shape* shape = shapes_.getShape(i);
            if (shape && shape->getSelected()) {
                applyResize(shape, delta);
            }
        }
        update();
    }
}

// Применение изменения размера к фигуре
void MainWindow::applyResize(Shape* shape, int delta) {
    if (Circle* circle = dynamic_cast<Circle*>(shape)) {
        int newRadius = circle->getRadius() + delta;
        // Ограничиваем минимальный и максимальный размер
        if (newRadius >= 5 && newRadius <= 100) {
            circle->setRadius(newRadius);
        }
    }
    else if (Rectangle* rect = dynamic_cast<Rectangle*>(shape)) {
        int newWidth = rect->getWidth() + delta;
        int newHeight = rect->getHeight() + delta;
        if (newWidth >= 10 && newWidth <= 200 && newHeight >= 10 && newHeight <= 200) {
            rect->setSize(newWidth, newHeight);
        }
    }
    else if (Square* square = dynamic_cast<Square*>(shape)) {
        int newSize = square->getSide() + delta;
        if (newSize >= 10 && newSize <= 200) {
            square->setSide(newSize);
        }
    }
    else if (Triangle* triangle = dynamic_cast<Triangle*>(shape)) {
        int newSize = triangle->getSize() + delta;
        if (newSize >= 10 && newSize <= 150) {
            triangle->setSize(newSize);
        }
    }
    else if (Line* line = dynamic_cast<Line*>(shape)) {
        // Для линии изменяем длину, сохраняя центр
        int x1 = line->getX();
        int y1 = line->getY();
        int x2 = line->getX2();
        int y2 = line->getY2();

        // Вычисляем центр линии
        int centerX = (x1 + x2) / 2;
        int centerY = (y1 + y2) / 2;

        // Вычисляем текущую длину
        int dx = x2 - x1;
        int dy = y2 - y1;
        double currentLength = sqrt(dx*dx + dy*dy);

        if (currentLength > 0) {
            // Увеличиваем/уменьшаем длину на delta
            double newLength = currentLength + delta;
            if (newLength >= 10 && newLength <= 200) {
                // Масштабируем вектор от центра
                double scale = newLength / currentLength;
                int newX1 = centerX + (int)((x1 - centerX) * scale);
                int newY1 = centerY + (int)((y1 - centerY) * scale);
                int newX2 = centerX + (int)((x2 - centerX) * scale);
                int newY2 = centerY + (int)((y2 - centerY) * scale);

                line->setPosition(newX1, newY1);
                line->setEndPoint(newX2, newY2);
            }
        }
    }
}

bool MainWindow::canResizeSelected(int delta) const {
    for (int i = 0; i < shapes_.getCount(); i++) {
        Shape* shape = shapes_.getShape(i);
        if (shape && shape->getSelected()) {
            // Создаем временную копию для проверки
            if (Circle* circle = dynamic_cast<Circle*>(shape)) {
                int newRadius = circle->getRadius() + delta;
                if (newRadius <= 5 || newRadius >= 100) {
                    return false;
                }
            }
            else if (Rectangle* rect = dynamic_cast<Rectangle*>(shape)) {
                int newWidth = rect->getWidth() + delta;
                int newHeight = rect->getHeight() + delta;
                if (newWidth <= 10 || newWidth >= 200 || newHeight <= 10 || newHeight >= 200) {
                    return false;
                }
            }
            else if (Triangle* triangle = dynamic_cast<Triangle*>(shape)) {
                int newSize = triangle->getSize() + delta;
                if (newSize <= 10 || newSize >= 150) {
                    return false;
                }
            }
        }
    }
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ========== Shape ==========

Shape::Shape(int x, int y) : x_(x), y_(y), color_(Qt::red), selected_(false) {}

void Shape::move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}

bool Shape::checkBounds(int left, int top, int right, int bottom) const {
    QRect bounds = getBorderRect();
    return (bounds.left() >= left &&
            bounds.top() >= top &&
            bounds.right() <= right &&
            bounds.bottom() <= bottom);
}

bool Shape::safeMove(int dx, int dy, int left, int top, int right, int bottom) {
    int oldX = x_;
    int oldY = y_;

    move(dx, dy);

    if (!checkBounds(left, top, right, bottom)) {
        setPosition(oldX, oldY);
        return false;
    }

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

// ========== Circle ==========

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

// ========== Rectangle ==========

Rectangle::Rectangle(int x, int y, int width, int height) : Shape(x, y), width_(width), height_(height) {}

bool Rectangle::contains(int x, int y) const {
    return (x >= x_ && x <= x_ + width_ && y >= y_ && y <= y_ + height_);
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

// ========== Square ==========

Square::Square(int x, int y, int size) : Rectangle(x, y, size, size) {}

void Square::setSize(int size) {
    Rectangle::setSize(size, size);
}

void Square::setSide(int size) {
    setSize(size);
}

int Square::getSide() const {
    return getWidth();
}

// ========== Triangle ==========

Triangle::Triangle(int x, int y, int size) : Shape(x, y), size_(size) {}

bool Triangle::contains(int x, int y) const {
    QRect bounds = getBorderRect().adjusted(-5, -5, 5, 5);
    return bounds.contains(x, y);
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
        QPoint(x_ + size_ / 2, y_ - size_ / 2)
    };

    painter.drawPolygon(points, 3);
}

QRect Triangle::getBorderRect() const {
    return QRect(x_, y_ - size_ / 2, size_, size_);
}

int Triangle::getSize() const {
    return size_;
}

void Triangle::setSize(int size) {
    size_ = size;
}

// ========== Line ==========

Line::Line(int x1, int y1, int x2, int y2, int thickness) : Shape(x1, y1), x2_(x2), y2_(y2), thickness_(thickness) {}

bool Line::contains(int x, int y) const {
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

bool Line::checkBounds(int left, int top, int right, int bottom) const {
    QRect bounds = getBorderRect();

    int extra = thickness_ + 2;

    return (bounds.left() + extra >= left &&
            bounds.right() - extra <= right &&
            bounds.top() + extra >= top &&
            bounds.bottom() - extra <= bottom);
}

QRect Line::getBorderRect() const {
    int left = std::min(x_, x2_);
    int top = std::min(y_, y2_);
    int right = std::max(x_, x2_);
    int bottom = std::max(y_, y2_);

    return QRect(left, top, right - left, bottom - top);
}

void Line::move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
    x2_ += dx;
    y2_ += dy;
}

bool Line::safeMove(int dx, int dy, int left, int top, int right, int bottom) {
    int oldX1 = x_;
    int oldY1 = y_;
    int oldX2 = x2_;
    int oldY2 = y2_;

    move(dx, dy);

    if (!checkBounds(left, top, right, bottom)) {
        x_ = oldX1;
        y_ = oldY1;
        x2_ = oldX2;
        y2_ = oldY2;
        return false;
    }

    return true;
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

// ========== ShapeContainer ==========

ShapeContainer::ShapeContainer() {}

ShapeContainer::~ShapeContainer() {
    clear();
}

void ShapeContainer::addShape(Shape *shape) {
    if (shape != nullptr) shapes_.push_back(shape);
}

void ShapeContainer::removeShape(int i) {
    if (i >= 0 && i < (int)shapes_.size()) {
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
    for (int i = (int)shapes_.size() - 1; i >= 0; i--) {
        if (shapes_[i]->getSelected()) {
            delete shapes_[i];
            shapes_.erase(shapes_.begin() + i);
        }
    }
}

void ShapeContainer::selectAll() {
    for (Shape* shape : shapes_) {
        shape->setSelected(true);
    }
}

Shape* ShapeContainer::getShape(int i) const {
    if (i >= 0 && i < (int)shapes_.size()) {
        return shapes_[i];
    }
    return nullptr;
}

int ShapeContainer::getCount() const {
    return (int)shapes_.size();
}

void ShapeContainer::moveSelected(int dx, int dy, int maxX, int maxY) {
    // Границы окна
    int left = 10;
    int top = 10;
    int right = maxX - 10;
    int bottom = maxY - 10;

    // Сначала проверяем, можно ли переместить ВСЕ выделенные фигуры
    bool canMoveAll = true;

    // Сохраняем текущие позиции всех выделенных фигур
    std::vector<std::pair<int, int>> originalPositions;

    for (auto* shape : shapes_) {
        if (shape->getSelected()) {
            // Сохраняем текущую позицию
            originalPositions.push_back({shape->getX(), shape->getY()});

            // Временно перемещаем для проверки
            shape->move(dx, dy);

            // Проверяем границы
            if (!shape->checkBounds(left, top, right, bottom)) {
                canMoveAll = false;
            }

            // Возвращаем на место
            shape->move(-dx, -dy);
        }
    }

    // Если ВСЕ фигуры могут переместиться - перемещаем их
    if (canMoveAll) {
        int posIndex = 0;
        for (auto* shape : shapes_) {
            if (shape->getSelected()) {
                shape->move(dx, dy);
                posIndex++;
            }
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
