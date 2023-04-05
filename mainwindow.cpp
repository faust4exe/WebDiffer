#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QScreen>
#include <QWindow>
#include <QTime>
#include <QDebug>
#include <QDir>

const QString shotsPath = "shots";
const int beepInterval = 5;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDir dir;
    dir.mkdir(shotsPath);

    ui->setupUi(this);

    ui->quickWidget->engine()->rootContext()->setContextProperty("theWindow", this);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));

    m_beepTimer.setInterval(beepInterval * 1000);
    m_compareTimer.setInterval(m_updateInterval * 1000);

    connect(&m_compareTimer, &QTimer::timeout, this, &MainWindow::compareScreenshots);
    connect(&m_beepTimer, &QTimer::timeout, [](){
        QApplication::beep();
    });

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
    on_stopBeeping_clicked();
    compareScreenshots();
    m_compareTimer.start();
    ui->stopCompare->setEnabled(true);
}

void MainWindow::on_stopCompare_clicked()
{
    m_compareTimer.stop();
    ui->stopCompare->setEnabled(false);
}

void MainWindow::compareScreenshots()
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
                QRgb diffPixel = originalPixel - newPixel;
                updateImage.setPixel(x,y, diffPixel);

                if(qRed(diffPixel) || qGreen(diffPixel) || qBlue(diffPixel)){
                    diffCount++;
                }
            }
        }

        if ( diffCount > 15 ) {
            ui->stopBeeping->setEnabled(true);
            QApplication::beep();
            m_beepTimer.start();

            bool ok = updatePixmap.save(fileName, "png");
            qDebug() << "Saved update screen to " << fileName
                     << " : " << ok;

            ok = updateImage.save(fileName.replace(".png","__diff.png"), "png");
            setDiffFilename(QUrl::fromLocalFile(fileName));
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

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "updatePage");
}

void MainWindow::on_stopBeeping_clicked()
{
    m_beepTimer.stop();
    ui->stopBeeping->setEnabled(false);
    setDiffFilename(QUrl());
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
    m_compareTimer.setInterval(m_updateInterval * 1000);

    emit updateIntervalChanged();
}

QUrl MainWindow::diffFilename() const
{
    return m_diffFilename;
}

void MainWindow::setDiffFilename(const QUrl &newDiffFilename)
{
    if (m_diffFilename == newDiffFilename)
        return;

    m_diffFilename = newDiffFilename;
    emit diffFilenameChanged();
}
