#include "itemdialog.h"
#include "ui_itemdialog.h"

ItemDialog::ItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);
    ui->szButton->click();
    setWindowTitle( tr( "New value" ) );
}

ItemDialog::~ItemDialog()
{
    delete ui;
}

QString ItemDialog::getName() const {
    return ui->nameEdit->text();
}

QString ItemDialog::getValue() const {
    return ui->valueEdit->text();
}

int ItemDialog::getType() const {
    if(ui->szButton->isChecked()){
        return 0;
    }

    return 1;
}
