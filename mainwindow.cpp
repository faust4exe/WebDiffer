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

bool loop_beep = false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->quickWidget->engine()->rootContext()->setContextProperty("theWindow", this);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
}

MainWindow::~MainWindow()
{
    ui->quickWidget->setSource(QUrl());
    delete ui;
}

int counter = 0;
QImage originalImage;
QString originalFilename;
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Push button clicked";
    QPixmap originalPixmap = ui->quickWidget->grab(scanRect());

    QString fileName = QTime::currentTime().toString();
    fileName.prepend("__");
    fileName.prepend(QString::number(++counter));
    fileName.prepend("shots/");
    fileName.append("__initial.png");
    fileName.replace(":", "-");

    bool ok = originalPixmap.save(fileName, "png");
    originalFilename = fileName;
    qDebug() << "Saved initial screen to " << fileName
             << " : " << ok;

    if(ok)fileName.append("  - ok");
    else fileName.append("  - fail");

    fileName.prepend("Saved to ");
    ui->label->setText(fileName);

    originalImage = originalPixmap.toImage();
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "Push button 2 clicked";
    QPixmap updatePixmap = ui->quickWidget->grab(scanRect());

    QString fileName = QTime::currentTime().toString();
    fileName.prepend("__");
    fileName.prepend(QString::number(++counter));
    fileName.prepend("shots/");
    fileName.append(".png");
    fileName.replace(":", "-");

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
            loop_beep = true;
            loopBeep();

            bool ok = updatePixmap.save(fileName, "png");
            qDebug() << "Saved update screen to " << fileName
                     << " : " << ok;

            ok = updateImage.save(fileName.replace(".png","__diff.png"), "png");
            qDebug() << "Diff saved " << ok << "  with differentPixels " << diffCount;

            if(ok)fileName.append("  - ok");
            else fileName.append("  - fail");

            fileName.prepend("Saved to ");
            fileName.append(" diffCount: " + QString::number(diffCount) );
            ui->label->setText(fileName);
            qDebug() << "Push button 2 clicked - has changes";
        }
        else {
            fileName.append(" no changes from " + originalFilename);
            ui->label->setText(fileName);
            qDebug() << "Push button 2 clicked - no changes";
        }
    }
    else {
        qDebug() << "Pixmap is are different in size";
    }

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(),
                              "updatePage");

    int secondToUpdate = 60 * 3;
    QTimer::singleShot(secondToUpdate * 1000, this, SLOT(on_pushButton_2_clicked()));
}

void MainWindow::loopBeep()
{
    QApplication::beep();

    if ( loop_beep ) {
        int secondToUpdate = 5;
        QTimer::singleShot(secondToUpdate*1000, this, SLOT(loopBeep()));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    loop_beep = false;
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
