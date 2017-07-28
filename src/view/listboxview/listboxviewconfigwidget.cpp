/*
 * ListBoxViewConfigWidget.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: sk
 */

#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>

#include "dbobjectmanager.h"
#include "dbovariableorderedsetwidget.h"
#include "listboxview.h"
#include "listboxviewconfigwidget.h"
#include "listboxviewdatasource.h"
#include "logger.h"
#include "stringconv.h"

using namespace Utils;

ListBoxViewConfigWidget::ListBoxViewConfigWidget( ListBoxView* view, QWidget* parent )
    :   QWidget( parent ), view_( view ), variable_set_widget_ (nullptr), presentation_check_ (nullptr), overwrite_check_(nullptr)
{
    QVBoxLayout *vlayout = new QVBoxLayout;

    assert (view_);

    variable_set_widget_ = view_->getDataSource()->getSet()->widget();
    vlayout->addWidget (variable_set_widget_);

    presentation_check_ = new QCheckBox("Use Presentation");
    presentation_check_->setChecked(view_->getDataSource()->usePresentation());
    connect(presentation_check_, SIGNAL( clicked() ), this, SLOT( toggleUsePresentation() ));
    vlayout->addWidget(presentation_check_);

    vlayout->addStretch();

    overwrite_check_ = new QCheckBox("Overwrite Exported File");
    overwrite_check_->setChecked(view_->getDataSource()->overwriteCSV());
    connect(overwrite_check_, SIGNAL( clicked() ), this, SLOT( toggleUseOverwrite() ));
    vlayout->addWidget(overwrite_check_);

    QPushButton *export_button = new QPushButton ("Export");
    connect(export_button, SIGNAL(clicked(bool)), this, SLOT(exportSlot()));
    vlayout->addWidget(export_button);

    setLayout( vlayout );
}

ListBoxViewConfigWidget::~ListBoxViewConfigWidget()
{
}

void ListBoxViewConfigWidget::toggleUsePresentation()
{
  assert (presentation_check_);
  bool checked = presentation_check_->checkState() == Qt::Checked;
  logdbg  << "ListBoxViewConfigWidget: toggleUsePresentation: setting use presentation to " << checked;
  view_->getDataSource()->usePresentation(checked);
}

void ListBoxViewConfigWidget::toggleUseOverwrite()
{
    assert (overwrite_check_);
    bool checked = overwrite_check_->checkState() == Qt::Checked;
    logdbg  << "ListBoxViewConfigWidget: toggleUseOverwrite: setting overwrite to " << checked;
    view_->getDataSource()->overwriteCSV (checked);

}

void ListBoxViewConfigWidget::exportSlot()
{
    logdbg << "ListBoxViewConfigWidget: exportSlot";
    assert (overwrite_check_);
    emit exportSignal(overwrite_check_->checkState() == Qt::Checked);
}
