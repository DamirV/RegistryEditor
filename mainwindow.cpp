#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "itemdialog.h"

static const auto REG_PATH_SEPARATOR = "\\";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->treeWidget->setHeaderLabel(tr("registry"));
    translator.load("RegistryEditor_ru_RU");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRegistryItemExpanded( QTreeWidgetItem* item )
{
    QSettings settings(findPathForItem(item), QSettings::NativeFormat );
    addRegistryGroup(item, &settings);
}

void MainWindow::onRegistryItemClicked( QTreeWidgetItem* item, int column ) {
    Q_UNUSED(column)

    if(item && item->parent()) {
        QSettings settings(findPathForItem(item), QSettings::NativeFormat);
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->clearContents();
        for(const auto& key : settings.childKeys()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            QTableWidgetItem* item = new QTableWidgetItem(key);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem( row, 0, item );

            QString value = settings.value(key).toString();
            item = new QTableWidgetItem(value);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(row, 1, item);
        }
    }
}

void MainWindow::onCreate() {
    ItemDialog dlg(this);

    if(dlg.exec() == QDialog::Accepted) {
        QTreeWidgetItem* item = ui->treeWidget->currentItem();
        QSettings settings(findPathForItem(item), QSettings::NativeFormat );
        int type = dlg.getType();

        if(type == 0){
            settings.setValue(dlg.getName(), dlg.getValue());
        }
        else{
            bool ok = false;
            int value = dlg.getValue().toInt(&ok);
            if(ok){
                if(value < 0){
                    QMessageBox::critical(this,tr("Error"),tr("Wrong number, the number must be positive"));
                    return;
                }
                settings.setValue(dlg.getName(), value);
            }
            else{
                QMessageBox::critical(this,tr("Error"),tr("Wrong value"));
                return;
            }
        }

        settings.sync();
        onRegistryItemClicked(item, 0);
    }
}

void MainWindow::onRemove() {

    if(ui->tableWidget->currentRow() != -1){
        QMessageBox::StandardButton msg =
                QMessageBox::question(this,
                                      tr("Delete"),
                                      tr("Are you sure you want to delete this parameter?"),
                                      QMessageBox::Yes|QMessageBox::No);


        if(msg == QMessageBox::Yes){
            auto item = ui->treeWidget->currentItem();
            QSettings settings( findPathForItem(item), QSettings::NativeFormat);
            settings.remove( ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text());
            settings.sync();
            onRegistryItemClicked(item, 0);
        }
    }
}

void MainWindow::addRegistryGroup( QTreeWidgetItem* root, QSettings* settings ) {
    static int depth = 0;
    ++depth;

    if(root->childCount() > 0) {

        for(int i = 0; i < root->childCount(); ++i) {
            QTreeWidgetItem* child = root->child(i);

            if(child->childCount() > 0) {
                break;
            }

            settings->beginGroup(child->text(0));
            addRegistryGroup(child, settings);
            settings->endGroup();
        }

    } else if(depth <= MAX_DEPTH) {
        for(const auto& group : settings->childGroups()) {
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << group);
            root->addChild(item);

            settings->beginGroup(group);
            addRegistryGroup(item, settings);
            settings->endGroup();
        }
    }
    --depth;
}

QString MainWindow::findPathForItem(QTreeWidgetItem* item) const {
    QStringList reversePath;

    while(item != nullptr){
        reversePath << item->text( 0 );
        item = item->parent();
    }

    reversePath.pop_back();
    std::reverse(reversePath.begin(), reversePath.end());

    return reversePath.join(REG_PATH_SEPARATOR);
}

void MainWindow::changeEvent(QEvent *pEvent){
    if(pEvent->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
}

void MainWindow::on_actionChange_language_triggered()
{
    qApp -> installTranslator(&translator);
}


void MainWindow::on_actionEnglish_triggered()
{
    qApp -> removeTranslator(&translator);
}


void MainWindow::on_actionOpen_triggered()
{
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList() << "Computer" );
        ui->treeWidget->addTopLevelItem(root);

        const auto REGISTRY_GROUPS = {
            "HKEY_CLASSES_ROOT",
            "HKEY_CURRENT_USER",
            "HKEY_LOCAL_MACHINE",
            "HKEY_USERS",
        };

        for(const auto& group : REGISTRY_GROUPS) {
            QSettings settings(group, QSettings::NativeFormat);
            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << group);
            root->addChild(item);
            addRegistryGroup(item, &settings);
        }

        ui->treeWidget->expandItem(root);
}


void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    onRegistryItemExpanded(item);
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    onRegistryItemClicked(item, column);
}


void MainWindow::on_createButton_clicked()
{
    onCreate();
}


void MainWindow::on_deleteButton_clicked()
{
    onRemove();
}


void MainWindow::on_actionAbout_program_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Registry editor"));
}

