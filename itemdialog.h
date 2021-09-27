#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>

namespace Ui {
class ItemDialog;
}

class ItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemDialog(QWidget *parent = nullptr);
    ~ItemDialog();

    QString getName() const;
    QString getValue() const;
    int getType() const;

private:
    Ui::ItemDialog *ui;
};

#endif // ITEMDIALOG_H
