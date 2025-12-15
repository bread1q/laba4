#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Shape
{
protected:
    int x_;
    int y_;
    QColor color_;
    bool selected_;

public:
    Shape(int x, int y);
    virtual ~Shape() = default;

    virtual void draw(QPainter &painter) const = 0;
    virtual bool contains(int x, int y) const = 0;
    virtual QRect getBorderRect() const = 0;

    virtual void move(int dx, int dy);
    virtual bool checkBounds(int left, int top, int right, int bottom) const;
    virtual bool safeMove(int dx, int dy, int left, int top, int right, int bottom);

    bool getSelected() const;
    void setSelected(bool selected);

    QColor getColor() const;
    void setColor(const QColor& color);

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
};

class Circle : public Shape
{
private:
    int radius_;

public:
    Circle(int x, int y, int radius);

    void draw(QPainter &painter) const override;
    bool contains(int x, int y) const override;
    QRect getBorderRect() const override;

    int getRadius() const;
    void setRadius(int r);
};

class Rectangle : public Shape
{
private:
    int width_;
    int height_;

public:
    Rectangle(int x, int y, int width = 50, int height = 30);

    void draw(QPainter &painter) const override;
    bool contains(int x, int y) const override;
    QRect getBorderRect() const override;

    int getWidth() const;
    int getHeight() const;
    void setSize(int width, int height);
};

class Square : public Rectangle
{
public:
    Square(int x, int y, int size = 40);

    void setSize(int size);

    void setSide(int size);
    int getSide() const;
};

class Triangle : public Shape
{
private:
    int size_;

public:
    Triangle(int x, int y, int size = 40);

    void draw(QPainter &painter) const override;
    bool contains(int x, int y) const override;
    QRect getBorderRect() const override;

    int getSize() const;
    void setSize(int size);
};

class Line : public Shape
{
private:
    int x2_;
    int y2_;
    int thickness_;

public:
    Line(int x1, int y1, int x2, int y2, int thickness = 3);

    void draw(QPainter &painter) const override;
    bool contains(int x, int y) const override;
    QRect getBorderRect() const override;

    bool checkBounds(int left, int top, int right, int bottom) const override;
    bool safeMove(int dx, int dy, int left, int top, int right, int bottom) override;
    void move(int dx, int dy) override;

    int getX2() const;
    int getY2() const;
    int getThickness() const;
    void setEndPoint(int x2, int y2);
    void setThickness(int thickness);
};

class ShapeContainer
{
private:
    std::vector<Shape*> shapes_;

public:
    ShapeContainer();
    ~ShapeContainer();

    void addShape(Shape *shape);
    void removeShape(int i);
    void clear();

    void clearSelection();
    void removeSelected();
    void selectAll();

    Shape* getShape(int i) const;
    int getCount() const;

    void moveSelected(int dx, int dy, int maxX, int maxY);  // Изменена сигнатура!
    void setSelectedColor(const QColor &color);
};

enum ShapeType {
    CIRCLE,
    RECTANGLE,
    SQUARE,
    TRIANGLE,
    LINE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void selectCircle();
    void selectRectangle();
    void selectSquare();
    void selectTriangle();
    void selectLine();
    void changeColor();
    void clearWindow();
    void increaseSize();
    void decreaseSize();

private:
    Ui::MainWindow *ui;
    ShapeContainer shapes_;
    ShapeType currentShapeType_ = CIRCLE;

    void createMenu();
    void createToolBar();
    void updateWindowTitle();
    void resizeSelected(int delta);
    bool canResizeSelected(int delta) const;
    void applyResize(Shape* shape, int delta);
};
#endif // MAINWINDOW_H
