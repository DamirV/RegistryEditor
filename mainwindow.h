#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onRegistryItemExpanded( QTreeWidgetItem* item );
    void onRegistryItemClicked( QTreeWidgetItem* item, int column );

    void onCreate();
    void onRemove();

    void on_actionChange_language_triggered();

    void on_actionEnglish_triggered();

    void on_actionOpen_triggered();

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_createButton_clicked();

    void on_deleteButton_clicked();

    void on_actionAbout_program_triggered();

private:
    Ui::MainWindow *ui;
    void changeEvent(QEvent *pEvent);
    QTranslator translator;
    void addRegistryGroup( QTreeWidgetItem* root, QSettings* settings );
    QString findPathForItem( QTreeWidgetItem* item ) const;

    const int MAX_DEPTH = 2;
};
#endif // MAINWINDOW_H
