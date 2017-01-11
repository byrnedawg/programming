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
           void run_the_code(void);

    private slots:
           void on_start_button_clicked();

private:
    int i;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
