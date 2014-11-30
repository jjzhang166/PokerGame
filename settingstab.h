#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H

#include <QTabWidget>
#include "ui_settingstab.h"

namespace Ui {
class SettingsTab;
}

class SettingsTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit SettingsTab(QWidget *parent = 0);
    ~SettingsTab();

    Ui::SettingsTab *getUi() const;
    void setUi(Ui::SettingsTab *value);

private:
    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
