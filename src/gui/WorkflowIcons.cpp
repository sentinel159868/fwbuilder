/*

                          Firewall Builder

                 Copyright (C) 2010 NetCitadel, LLC

  Author:  Roman Bovsunivskiy     a2k0001@gmail.com

  $Id: WorkflowIcons.cpp® 2786 2010-04-01 14:05:36Z a2k $

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "WorkflowIcons.h"
#include "ClickableLabel.h"
#include "ui_WorkflowIcons.h"
#include "ProjectPanel.h"
#include "FWWindow.h"
#include "ObjectManipulator.h"
#include <fwbuilder/Firewall.h>
#include "global.h"
#include <QDebug>

WorkflowIcons::WorkflowIcons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkflowIcons_q)
{
    ui->setupUi(this);

}

// Find main window object if child is in it's widget tree
QObject *findMainWindow(QObject *child)
{
    if (dynamic_cast<FWWindow*>(child)!=NULL)
        return child;
    else
        return findMainWindow(child->parent());
}

void WorkflowIcons::setUpSignals(QWidget *panel)
{
    ObjectManipulator *om = panel->findChild<ObjectManipulator*>();
    QAction *newFirewall = om->findChild<QAction*>(QString("newObject_") + libfwbuilder::Firewall::TYPENAME);
    connect(ui->newFirewall, SIGNAL(clicked()), newFirewall, SLOT(trigger()));

    // global variable mw is null when this is running
    QObject *mainWindow = findMainWindow(panel);
    QAction *import = mainWindow->findChild<QAction*>("policyImportAction");
    connect(ui->importConfig, SIGNAL(clicked()), import, SLOT(trigger()));

    connect(ui->action_getting_started, SIGNAL(clicked()), mainWindow, SLOT(showTutorial()));
}

WorkflowIcons::~WorkflowIcons()
{
    delete ui;
}

void WorkflowIcons::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}