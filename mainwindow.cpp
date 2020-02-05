// created by Heitor Adao Junior
#include "mainwindow.h"
#include "configdialog.h"
#include "colorfrequency.h"
#include "ui_mainwindow.h"
#include "colormatrix.h"
#include <cmath>
#include <QFileDialog>
#include <QPainter>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->toolButtonOpenImage, SIGNAL(clicked()), this, SLOT(openImage()));
    lastOpenedPath = QDir::homePath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    QString pathSearch;
    pathSearch = (ui->lineEditPath->text().isEmpty()) ? lastOpenedPath : ui->lineEditPath->text();
    QString filename(QFileDialog::getOpenFileName(this, "", pathSearch, "Images (*.png *.bmp *.jpg *.gif);;All files(*.*)"));
    ui->lineEditPath->setText(filename);
    QDir dir(filename);
    lastOpenedPath = dir.absolutePath();
    imageOriginal.load(filename);
    ui->labelOriginal->setPixmap(imageOriginal);
    emit newOpenedImage();
    switch (ui->comboBoxTransformType->currentIndex()) {
    case 0:
        transformGrayScale();
        break;
    case 1:
        transformHighPass();
        break;
    case 2:
        transformLowPass();
        break;
    case 3:
        transformPaletted();
        break;
    }
}

void MainWindow::transformGrayScale()
{
    // cria o novo pixmap
    // seta o tamanho igual ao tamanho da iagem de origem
    QSize size(imageOriginal.size());
    imageModified = QPixmap(size);
    QPainter painter(&imageModified);
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            QColor oldColor(imageOriginal.toImage().pixel(x,y));
            int newColorComponents = oldColor.red() * 0.5 + oldColor.green() * 0.3 + oldColor.blue() * 0.2;
            QColor newColor(newColorComponents, newColorComponents, newColorComponents);
            painter.setPen(newColor);
            painter.drawPoint(x, y);
        }
    }
    ui->labelModified->setPixmap(imageModified);
}

void MainWindow::transformLowPass()
{
    ColorMatrix colorMatrix;
    QSize size(imageOriginal.size());
    imageModified = QPixmap(size);
    QPainter painter(&imageModified);
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            QColor newColor(colorMatrix.getPixel(imageOriginal.toImage(), QPoint(x, y)));
            painter.setPen(newColor);
            painter.drawPoint(x, y);
        }
    }
    ui->labelModified->setPixmap(imageModified);
}

void MainWindow::transformHighPass()
{
    int weightHighPass[9] = {0, -1,  0,
                            -1,  5, -1,
                             0, -1,  0};
    ColorMatrix colorMatrix(3, weightHighPass);

    QSize size(imageOriginal.size());
    imageModified = QPixmap(size);
    QPainter painter(&imageModified);
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            QColor newColor(colorMatrix.getPixel(imageOriginal.toImage(), QPoint(x, y)));
            painter.setPen(newColor);
            painter.drawPoint(x, y);
        }
    }
    ui->labelModified->setPixmap(imageModified);
}

void MainWindow::transformPaletted(int numColors)
{
    // value is the frequency of color
    QMap<QColor, int> histogram;

    QSize size(imageOriginal.size());
    for (int y = 0; y < size.height(); ++y) { // for each pixel of image
        for (int x = 0; x < size.width(); ++x) {
            QColor oldColor(imageOriginal.toImage().pixel(x,y));
            if (histogram.contains(oldColor)) {            // if pixel already on histogram...
                int frequency = histogram.value(oldColor); //     increment
                histogram[oldColor] = ++frequency;
            } else {                                       // else
                histogram.insert(oldColor, 1);             //    insert, with default value = 1
            }
        }
    }


    // cria um vetor de tamanho = max colors
    QVector<QColor> newPalette;
    newPalette.resize(numColors);


    int higherFrequency = 0;
    QColor colorWithHigherFrequency;
    for (int c = 0; c < numColors; ++c) {
        //percorre o map
        QMap<QColor, int>::iterator it = histogram.begin();
        for (; it != histogram.end(); ++it) {
            if (it.value() > higherFrequency)
                colorWithHigherFrequency = it.key();
        }
        histogram.remove(colorWithHigherFrequency);
        newPalette[c] = colorWithHigherFrequency;
    }



    imageModified = QPixmap(size);
    QPainter painter(&imageModified);
    // raster the image again, and for each pixel...
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            QColor oldColor(imageOriginal.toImage().pixel(x,y));


            QColor closerColor;
            double distanceOfCloserColor = 9999999999999999.0;

            // para cara item do vetor


            QVector<QColor>::iterator it = newPalette.begin();
            for(; it != newPalette.end(); ++it) {
                // calc the distance from actual pixel to item vector
                double distance = colorDistance(oldColor, *it);

                // comparae a distancia com a menor distancia, se for menor ou igual...
                if (distance < distanceOfCloserColor) {
                    // store the distance and color
                    closerColor = *it;
                    distanceOfCloserColor = distance;
                }
            }

            // draw the color in the second image
            painter.setPen(closerColor);
            painter.drawPoint(x, y);
        }
    }

#if 1
    for (int i = 0; i < numColors; ++i) {
        painter.setPen(newPalette.at(i));
        painter.drawPoint(i, 0);
    }
#endif
    ui->labelModified->setPixmap(imageModified);
}

void MainWindow::on_toolButton_clicked()
{
    ConfigDialog config;
    config.exec();
}

void MainWindow::on_pushButtonSaveAs_clicked()
{
    QString fileName(QFileDialog::getSaveFileName(this, "save modified image as...", lastOpenedPath,
                                                  "Bitmap (*.bmp);;"
                                                  "Portable Network Graphics (*.png)"));
    QString extension(fileName.right(3));
    extension.toUpper();
    const int quality = 100; // no compression
    imageModified.save(fileName, extension.toLatin1(), quality);
}

double MainWindow::colorDistance(const QColor &color1, const QColor &color2)
{
    double red = std::pow((double)color1.red() - color2.red(), 2);
    double green = std::pow((double)color1.green() - color2.green(), 2);
    double blue = std::pow((double)color1.blue() - color2.blue(), 2);
    return std::sqrt(red + green + blue);
}
