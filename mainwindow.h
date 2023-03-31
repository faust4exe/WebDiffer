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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QRect scanRect() const;
    void setScanRect(const QRect &newScanRect);

    Q_INVOKABLE QUrl fromUserInput(const QString &text);

signals:
    void scanRectChanged();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void loopBeep();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QRect m_scanRect;
};
#endif // MAINWINDOW_H
