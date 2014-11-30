#include "settingstab.h"


SettingsTab::SettingsTab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SettingsTab)
{
    ui->setupUi(this);
}

SettingsTab::~SettingsTab()
{
    delete ui;
}
Ui::SettingsTab *SettingsTab::getUi() const
{
    return ui;
}

void SettingsTab::setUi(Ui::SettingsTab *value)
{
    ui = value;
}

