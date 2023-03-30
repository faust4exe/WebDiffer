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

    //    ui->quickWidget->engine()->rootContext()->setContextProperty()
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int counter = 0;
QImage originalImage;
void MainWindow::on_pushButton_clicked()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    WId winId = 0;
    if (const QWindow *window = windowHandle()) {
        screen = window->screen();
        winId = window->winId();
    }
    if (!screen)
        return;

    QPixmap originalPixmap = screen->grabWindow(winId, 30, 150, 720, 530);

    QString fileName = QTime::currentTime().toString();
    fileName.prepend("__");
    fileName.prepend(QString::number(++counter));
    fileName.prepend("shots/");
    fileName.append("__initial.png");
    fileName.replace(":", "-");

    bool ok = originalPixmap.save(fileName, "png");
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
    QScreen *screen = QGuiApplication::primaryScreen();
    WId winId = 0;
    if (const QWindow *window = windowHandle()) {
        screen = window->screen();
        winId = window->winId();
    }
    if (!screen)
        return;

    QPixmap updatePixmap = screen->grabWindow(winId, 30, 150, 720, 530);

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
        }
        else {
            fileName.append(" no changes ");
            ui->label->setText(fileName);
        }
    }
    else {
        qDebug() << "Pixmap is are different in size";
    }

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(),
                              "updatePage");

    int secondToUpdate = 60 * 3;
    QTimer::singleShot(secondToUpdate*1000, this, SLOT(on_pushButton_2_clicked()));
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