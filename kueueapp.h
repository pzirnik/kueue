/*
                kueue - keep track of your SR queue
          (C) 2011 - 2012 Stefan Bogner <sbogner@suse.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the

    Free Software Foundation, Inc.
    59 Temple Place - Suite 330
    Boston, MA  02111-1307, USA

    Have a lot of fun :-)

*/

#ifndef KUEUEAPP_H
#define KUEUEAPP_H

#include <QShortcut>

#include "ui/kueuewindow.h"
#include "ui/tabwidget.h"
#include "ui/statusbar.h"
#include "data/datathread.h"
#include "config.h"

class TabWidget;

class KueueApp : public QObject
{
    Q_OBJECT
    
    public:
        KueueApp();
        ~KueueApp();
        
    private:
        QString mSr;
        KueueWindow* mWindow; 
        TabWidget* mTabWidget;
        Systray* mSystray;
        StatusBar* mStatusBar;
        QShortcut* mDbRebuild;
        DataThread* mDataThread;
        bool mFailedMessageDisplayed;
        
    public slots:
        void updateUiData();
        
    private slots:
        void newDB();
        void deleteDirs( QStringList );
        void showSubBrowser( bool );
        void deleteDirsFailed( QStringList );
        void cleanupTemp();
        void notify( QString, QString, QString, QString );
        void createApp();
        void updateJobDone();
        void prodJobDone();
        void connectDataThread();
        void createSystray();
        void createMainWindow();
        void createDatabase();
        void settingsChanged();
        void sendTestNotification();
        void setTabPosition();
        void openConfig();
};

#endif
