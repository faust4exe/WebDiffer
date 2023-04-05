#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QRect scanRect READ scanRect WRITE setScanRect NOTIFY scanRectChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
    Q_PROPERTY(QUrl diffFilename READ diffFilename WRITE setDiffFilename NOTIFY diffFilenameChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QRect scanRect() const;
    void setScanRect(const QRect &newScanRect);

    Q_INVOKABLE QUrl fromUserInput(const QString &text);

    int updateInterval() const;
    void setUpdateInterval(int newUpdateInterval);

    QUrl diffFilename() const;
    void setDiffFilename(const QUrl &newDiffFilename);

signals:
    void scanRectChanged();
    void updateIntervalChanged();
    void diffFilenameChanged();

private slots:
    void on_takeScreenshot_clicked();
    void on_compareScreenshot_clicked();
    void on_stopCompare_clicked();
    void on_stopBeeping_clicked();
    void on_secsToUpdate_valueChanged();

    void compareScreenshots();

private:
    Ui::MainWindow *ui;
    QRect m_scanRect;
    int counter = 0;
    QImage originalImage;
    QString originalFilename;
    int m_updateInterval = 300;
    QUrl m_diffFilename;
    QTimer m_beepTimer;
    QTimer m_compareTimer;
};
#endif // MAINWINDOW_H
