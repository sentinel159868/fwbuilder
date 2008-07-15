/* 

                          Firewall Builder

                 Copyright (C) 2004 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@fwbuilder.org

  $Id$

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

#ifndef __FWBSETTINGS_H_
#define __FWBSETTINGS_H_

#include <qsettings.h>
#include <qrect.h>
#include <qprinter.h>
#include <qfont.h>

class QWidget;

/*
 * startup actions. I know, enum would be better, but QComboBox
 * operates with integers and it is much simpler to just store item
 * numbers in preferences.
 */
#define LoadStandardObjects 0
#define LoadLastEditedFile  1
    
#define SETTINGS_PATH_PREFIX  "/3.0"

class FWBSettings : public QSettings {

 public:

    enum LabelColors { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, GRAY };
    enum IconSize{ SIZE25X25, SIZE16X16};

 private:
    QString getLabelColorStr(enum LabelColors c);
    
 public:
    
    FWBSettings();

    void init();
    void save();

    QString getWDir();
    void    setWDir( const QString &wd );

    QString getOpenFileDir();
    void    setOpenFileDir( const QString &d );

    QString getSaveFileDir();
    void    setSaveFileDir( const QString &d );

    int     getInfoStyle();
    void    setInfoStyle(int s);

    QString getGroupViewMode();
    void    setGroupViewMode(const QString &mode);

    QString getGroupViewColumns();
    void    setGroupViewColumns(const QString &mode);

    int     getStartupAction();
    void    setStartupAction(int sa);

    int     getExpandTree();
    void    setExpandTree(int sa);

    int     getMergeLibs();
    void    setMergeLibs(int sa);

    bool    getObjTooltips();
    void    setObjTooltips(bool f);

    int     getTooltipDelay();
    void    setTooltipDelay(int v);
    
    QString getLastEdited();
    void    setLastEdited(const QString &file);

    int     getInfoWindowHeight();
    void    setInfoWindowHeight(int h);

    bool    getRCSLogState();
    void    setRCSLogState(bool f);

    bool    getAutoSave();
    void    setAutoSave(bool f);

    bool    getDontSaveStdLib();
    void    setDontSaveStdLib( bool f);
    
    QString getStr(const QString &attribute);
    void    setStr(const QString &attribute, const QString &val);

    bool    getBool(const QString &attribute);
    void    setBool(const QString &attribute, bool f );

    int     getInt(const QString &attribute);
    void    setInt(const QString &attribute, int v );

    bool    haveGeometry(QWidget *w);
    void    restoreGeometry(QWidget *w);
    void    restoreGeometry(QWidget *w, const QRect &defaultGeometry);
    void    saveGeometry(QWidget *w);

    bool    haveScreenPosition(const QString &wname);
    QPoint  getScreenPosition(const QString &wname);
    void    saveScreenPosition(const QString &wname, const QPoint &p);
    
    QString getLabelColor(enum LabelColors c);
    void    setLabelColor(enum LabelColors c,const QString &s);
    QString getLabelText(enum LabelColors c);
    void    setLabelText(enum LabelColors c, const QString &s);

    QString getSSHPath();
    void    setSSHPath(const QString &path);

    void    getPrinterOptions(QPrinter *printer,int &pageWidth,int &pageHeight);
    void    setPrinterOptions(QPrinter *printer,int pageWidth,int pageHeight);

    enum IconSize getIconsInRulesSize();
    void setIconsInRulesSize(enum IconSize size);

    bool getShowIconsInRules();
    void setShowIconsInRules(bool showIcons);

    bool getShowDirectionText();
    void setShowDirectionText(bool showText);

    QFont getRulesFont();
    void setRulesFont(const QFont &font);

    QFont getTreeFont();
    void setTreeFont(const QFont &font);

    QFont getUiFont();
    void setUiFont(const QFont &font);

    bool getClipComment();
    void setClipComment(bool);
private:
    QFont getFontByType(const char*type);
};

#endif

