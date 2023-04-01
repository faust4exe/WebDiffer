#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QRect scanRect READ scanRect WRITE setScanRect NOTIFY scanRectChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QRect scanRect() const;
    void setScanRect(const QRect &newScanRect);

    Q_INVOKABLE QUrl fromUserInput(const QString &text);

    int updateInterval() const;
    void setUpdateInterval(int newUpdateInterval);

signals:
    void scanRectChanged();

    void updateIntervalChanged();

private slots:
    void on_takeScreenshot_clicked();
    void on_compareScreenshot_clicked();
    void on_stopBeeping_clicked();
    void on_secsToUpdate_valueChanged();

    void loopBeep();

private:
    Ui::MainWindow *ui;
    QRect m_scanRect;
    bool m_loop_beep = false;
    int counter = 0;
    QImage originalImage;
    QString originalFilename;
    int m_updateInterval = 300;
};
#endif // MAINWINDOW_H
