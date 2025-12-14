#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>

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
    virtual bool checkBorder(int minX, int minY, int maxX, int maxY) const;

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

class Triangle : public Shape
{
private:
    int size_;

public:
    Triangle(int x, int y, int size = 40);

    void draw(QPainter &painter) const override;
    void contains(int x, int y) const override;
    QRect getBorderRect() const override;

    int getSize() const;
    int setSize(int size);
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
    void contains(int x, int y) const override;
    QRect getBorderRect() const override;

    int getX2() const;
    int getY2() const;
    int getThickness() const;
    void setEndPoint(int x2, int y2);
    void setThickness(int thickness);
};

class ShapeContainer
{
private:
    vector<Shape*> shapes_;

public:
    ShapeContainer();
    ~ShapeContainer();

    void addShape(Shape *shape);
    void removeShape(Shape *shape);
    void clear();

    void clearSelection();
    void removeSelected();
    void selectAll();

    Shape* getShape() const;
    int getCount() const;

    void moveSelected(int dx, int dy);
    void setSelectedColor(const QColor &color);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ShapeContainer shapes_;
};
#endif // MAINWINDOW_H
