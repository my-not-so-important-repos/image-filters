// created by Heitor Adao Junior
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void newOpenedImage();
private:
    Ui::MainWindow *ui;
    QString lastOpenedPath;
    QPixmap imageOriginal;
    QPixmap imageModified;

    void transformGrayScale();
    void transformLowPass();
    void transformHighPass();
    void transformPaletted(int numColors = 4);
    //QColor getPixel(int x, int y, QPixmap *a, QPixmap *b);
    double colorDistance(const QColor &color1, const QColor &color2);

private slots:
    void openImage();
    void on_toolButton_clicked();
    void on_pushButtonSaveAs_clicked();
};

#endif // MAINWINDOW_H
