#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QScreen>
#include <QWindow>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QDir>

const QString shotsPath = "shots";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDir dir;
    dir.mkdir(shotsPath);

    ui->setupUi(this);

    ui->quickWidget->engine()->rootContext()->setContextProperty("theWindow", this);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
}

MainWindow::~MainWindow()
{
    ui->quickWidget->setSource(QUrl());
    delete ui;
}

void MainWindow::on_takeScreenshot_clicked()
{
    QPixmap originalPixmap = ui->quickWidget->grab(scanRect());

    QString fileName =
            QString("%1/%2__%3__initial.png")
            .arg(shotsPath)
            .arg(QString::number(++counter))
            .arg(QTime::currentTime().toString("hh-mm-ss"));

    bool ok = originalPixmap.save(fileName, "png");
    originalFilename = fileName;
    qDebug() << "Saved initial screen to " << fileName
             << " : " << ok;

    if(ok) fileName.append("  - ok");
    else fileName.append("  - fail");

    ui->label->setText("Saved to " + fileName);
    ui->compareScreenshot->setEnabled(true);

    originalImage = originalPixmap.toImage();
}

void MainWindow::on_compareScreenshot_clicked()
{
    QPixmap updatePixmap = ui->quickWidget->grab(scanRect());

    QString fileName =
            QString("%1/%2__%3.png")
            .arg(shotsPath)
            .arg(QString::number(++counter))
            .arg(QTime::currentTime().toString("hh-mm-ss"));

    // compare with original
    int diffCount = 0;
    if ( updatePixmap.width() == originalImage.width()
         && updatePixmap.height() == originalImage.height() ) {

        QImage updateImage = updatePixmap.toImage();

        for(int x=0; x<updateImage.width(); x++) {
            for(int y=0; y<updateImage.height(); y++) {

                QRgb originalPixel = originalImage.pixel(x,y);
                QRgb newPixel = updateImage.pixel(x,y);
                QRgb diffPixel = newPixel - originalPixel;
                updateImage.setPixel(x,y, diffPixel);

                if(qRed(diffPixel) || qGreen(diffPixel) || qBlue(diffPixel)){
                    diffCount++;
                }
            }
        }

        if ( diffCount > 15 ) {
            m_loop_beep = true;
            ui->stopBeeping->setEnabled(true);
            loopBeep();

            bool ok = updatePixmap.save(fileName, "png");
            qDebug() << "Saved update screen to " << fileName
                     << " : " << ok;

            ok = updateImage.save(fileName.replace(".png","__diff.png"), "png");
            qDebug() << "Diff saved " << ok << "  with differentPixels " << diffCount;

            if(ok) fileName.append("  - ok");
            else fileName.append("  - fail");

            fileName.append(" diffCount: " + QString::number(diffCount) );
            ui->label->setText("Saved to " + fileName);
        }
        else {
            ui->label->setText(fileName + " no changes from " + originalFilename);
        }
    }
    else {
        ui->label->setText("Pixmap is are different in size");
    }

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(),
                              "updatePage");

    QTimer::singleShot(ui->secsToUpdate->value() * 1000,
                       this, SLOT(on_compareScreenshot_clicked()));
}

void MainWindow::loopBeep()
{
    if ( m_loop_beep == false ) {
        return;
    }

    QApplication::beep();

    const int secondToUpdate = 5;
    QTimer::singleShot(secondToUpdate * 1000, this, SLOT(loopBeep()));
}

void MainWindow::on_stopBeeping_clicked()
{
    m_loop_beep = false;
    ui->stopBeeping->setEnabled(false);
}

void MainWindow::on_secsToUpdate_valueChanged()
{
    setUpdateInterval(ui->secsToUpdate->value());
}

QRect MainWindow::scanRect() const
{
    return m_scanRect;
}

void MainWindow::setScanRect(const QRect &newScanRect)
{
    if (m_scanRect == newScanRect)
        return;

    m_scanRect = newScanRect;
    emit scanRectChanged();
}

QUrl MainWindow::fromUserInput(const QString &text)
{
    return QUrl::fromUserInput(text);
}

int MainWindow::updateInterval() const
{
    return m_updateInterval;
}

void MainWindow::setUpdateInterval(int newUpdateInterval)
{
    if (m_updateInterval == newUpdateInterval)
        return;

    m_updateInterval = newUpdateInterval;
    emit updateIntervalChanged();
}
