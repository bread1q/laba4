#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
