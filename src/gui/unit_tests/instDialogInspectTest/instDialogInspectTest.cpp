/*

                          Firewall Builder

                 Copyright (C) 2010 NetCitadel, LLC

  Author:  Roman Bovsunivskiy     a2k0001@gmail.com

  $Id: instDialogInspectTest.cpp 2786 2010-04-01 14:05:36Z a2k $

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

#include "instDialogInspectTest.h"

#include "unistd.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMenuBar>
#include <QTextBrowser>

#include "FWObjectClipboard.h"
#include "FWBApplication.h"

using namespace std;
using namespace QTest;
using namespace libfwbuilder;


bool checkProgress(QTreeWidget *list)
{
    for(int i=0; i<list->topLevelItemCount(); i++)
    {
        if ( (list->topLevelItem(i)->text(1).toStdString() == "Compiling ...") ||
             (list->topLevelItem(i)->text(1).toStdString() == "Installing ...") )
            return false;
    }
    return true;
}

void instDialogInspectTest::initTestCase()
{
    mw = new FWWindow();
    mw->show();
    mw->loadFile("test_work.fwb", false);
    FWObjectClipboard *clip = new FWObjectClipboard();
    dialogClosed = false;
    om = mw->findChild<ObjectManipulator*>("om");
    tree = mw->activeProject()->getCurrentObjectTree();
    tree->expandAll();
}

void instDialogInspectTest::cleanupTestCase()
{
    if (QFileInfo("test1.fw").exists())
        QVERIFY(QFile::remove("test1.fw"));
    if (QFileInfo("test2.fw").exists())
        QVERIFY(QFile::remove("test2.fw"));
    if (QFileInfo("test3.fw").exists())
        QVERIFY(QFile::remove("test3.fw"));
    if (QFileInfo("test4.fw").exists())
        QVERIFY(QFile::remove("test4.fw"));
}

void instDialogInspectTest::closeInstallOptions()
{
    QVERIFY(dynamic_cast<instOptionsDialog*>(app->activeModalWidget()) != NULL);
    app->activeModalWidget()->close();
    dialogClosed = true;
}

QPoint findItemPos(ObjectTreeViewItem *item, ObjectTreeView *tree)
{
    for (int h=10; h<tree->height(); h+=1)
    {
        for (int w=75; w<tree->width(); w+=1)
        {
            if(tree->itemAt(w,h) == item)
                return QPoint(w, h);
        }
    }
    return QPoint(-1,-1);
}

void instDialogInspectTest::closeContextMenu()
{
    QMenu *menu;
    foreach(QWidget *w, QApplication::allWidgets())
    {
        if (w->objectName() == "objectTreeContextMenu")
        {
            menu = dynamic_cast<QMenu*>(w);
            break;
        }
    }
    menu->hide();
}

/*
 * This function finds and activates an item with given name in the
 * context menu. If item is absent in the menu or is disabled, it
 * fails the test.
 */
void instDialogInspectTest::openContextMenu(ObjectManipulator *om,
                                               ObjectTreeViewItem *item, ObjectTreeView *tree,
                                               const QString &actionText)
{
    QTimer::singleShot(100, this, SLOT(closeContextMenu()));
    om->contextMenuRequested(findItemPos(item, tree));
    bool found_menu_item = false;
    QMenu *menu = NULL;
    foreach(QWidget *w, QApplication::allWidgets())
    {
        if (w->objectName() == "objectTreeContextMenu")
        {
            menu = dynamic_cast<QMenu*>(w);
            break;
        }
    }
    QVERIFY(menu != NULL);
    foreach (QObject *act, menu->children())
    {
        QAction *action = dynamic_cast<QAction*>(act);
        if (action == NULL) continue;
        if (action->text() == actionText)
        {
            QVERIFY(action->isEnabled() == true);
            action->activate(QAction::Trigger);
            found_menu_item = true;
            break;
        }
    }
    QVERIFY2(found_menu_item == true,
             QString("Item %1 not found in the context menu").arg(actionText).toAscii().constData());
}

void instDialogInspectTest::testInspect_firewall()
{
    ObjectTreeViewItem *treeitem = dynamic_cast<ObjectTreeViewItem*>(tree->findItems("test1", Qt::MatchExactly | Qt::MatchRecursive).first());
    this->openContextMenu(om, treeitem, tree, "Install");

    instDialog *dlg = mw->findChild<instDialog*>();

    QTest::mouseClick(dlg->findChild<QPushButton*>("pushButton16"), Qt::LeftButton);

    QPushButton *back = dlg->findChild<QPushButton*>("backButton");
    QPushButton *next = dlg->findChild<QPushButton*>("nextButton");

    QPushButton *inspect = dlg->findChild<QPushButton*>("inspectGeneratedFiles");

    QTest::mouseClick(next, Qt::LeftButton);

    QTreeWidget *list= dlg->findChild<QTreeWidget*>("fwWorkList");
    QTextBrowser *processLogDisplay = dlg->findChild<QTextBrowser*>("procLogDisplay");

    while (!checkProgress(list))
    {
        QVERIFY(!inspect->isEnabled());
        QTest::qWait(50);
    }
    QTest::qWait(50);
    QVERIFY(inspect->isEnabled());

    QString oldtext = processLogDisplay->toPlainText();
    QStackedWidget *stack = dlg->findChild<QStackedWidget*>();

    QVERIFY(stack->currentIndex() == 1);
    QTest::mouseClick(inspect, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 3);
    QVERIFY(back->isEnabled());
    QVERIFY(next->isEnabled());

    QTest::mouseClick(back, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 1);
    QVERIFY(oldtext == processLogDisplay->toPlainText());

    QTest::mouseClick(inspect, Qt::LeftButton);
    QTimer::singleShot(100, this, SLOT(closeInstallOptions()));
    QTest::mouseClick(next, Qt::LeftButton);
    QVERIFY(dialogClosed);
    QVERIFY(stack->currentIndex() == 1);

    dlg->reject();
    QTest::qWait(500);
}

void instDialogInspectTest::testInspect_cluster()
{
    ObjectTreeViewItem *treeitem = dynamic_cast<ObjectTreeViewItem*>(tree->findItems("cluster1", Qt::MatchExactly | Qt::MatchRecursive).first());
    this->openContextMenu(om, treeitem, tree, "Install");

    instDialog *dlg = mw->findChild<instDialog*>();

    QTest::mouseClick(dlg->findChild<QPushButton*>("pushButton16"), Qt::LeftButton);

    QPushButton *back = dlg->findChild<QPushButton*>("backButton");
    QPushButton *next = dlg->findChild<QPushButton*>("nextButton");

    QPushButton *inspect = dlg->findChild<QPushButton*>("inspectGeneratedFiles");

    QTest::mouseClick(next, Qt::LeftButton);

    QTreeWidget *list= dlg->findChild<QTreeWidget*>("fwWorkList");
    QTextBrowser *processLogDisplay = dlg->findChild<QTextBrowser*>("procLogDisplay");

    while (!checkProgress(list))
    {
        QVERIFY(!inspect->isEnabled());
        QTest::qWait(50);
    }
    QTest::qWait(50);
    QVERIFY(inspect->isEnabled());

    QString oldtext = processLogDisplay->toPlainText();
    QStackedWidget *stack = dlg->findChild<QStackedWidget*>();

    QVERIFY(stack->currentIndex() == 1);
    QTest::mouseClick(inspect, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 2);
    QVERIFY(back->isEnabled());
    QVERIFY(next->isEnabled());

    QTest::mouseClick(back, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 1);
    QVERIFY(oldtext == processLogDisplay->toPlainText());

    dlg->reject();
    QTest::qWait(500);
}

void instDialogInspectTest::testInspect_space()
{
    ObjectTreeViewItem *treeitem = dynamic_cast<ObjectTreeViewItem*>(tree->findItems("firewall name", Qt::MatchExactly | Qt::MatchRecursive).first());
    this->openContextMenu(om, treeitem, tree, "Install");

    instDialog *dlg = mw->findChild<instDialog*>();

    QTest::mouseClick(dlg->findChild<QPushButton*>("pushButton16"), Qt::LeftButton);

    QPushButton *back = dlg->findChild<QPushButton*>("backButton");
    QPushButton *next = dlg->findChild<QPushButton*>("nextButton");

    QPushButton *inspect = dlg->findChild<QPushButton*>("inspectGeneratedFiles");

    QTest::mouseClick(next, Qt::LeftButton);

    QTreeWidget *list= dlg->findChild<QTreeWidget*>("fwWorkList");
    QTextBrowser *processLogDisplay = dlg->findChild<QTextBrowser*>("procLogDisplay");

    while (!checkProgress(list))
    {
        QVERIFY(!inspect->isEnabled());
        QTest::qWait(50);
    }
    QTest::qWait(50);
    QVERIFY(inspect->isEnabled());

    QString oldtext = processLogDisplay->toPlainText();
    QStackedWidget *stack = dlg->findChild<QStackedWidget*>();

    QVERIFY(stack->currentIndex() == 1);
    QTest::mouseClick(inspect, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 3);
    QVERIFY(back->isEnabled());
    QVERIFY(next->isEnabled());

    QTest::mouseClick(back, Qt::LeftButton);
    QVERIFY(stack->currentIndex() == 1);
    QVERIFY(oldtext == processLogDisplay->toPlainText());

    QTest::mouseClick(inspect, Qt::LeftButton);
    QTimer::singleShot(100, this, SLOT(closeInstallOptions()));
    QTest::mouseClick(next, Qt::LeftButton);
    QVERIFY(dialogClosed);
    QVERIFY(stack->currentIndex() == 1);

    dlg->reject();
}
