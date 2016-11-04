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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "ui_configdialog.h"
#include "ui_configdialog-basic.h"

#include <QNetworkReply>
#include <QMessageBox>


class ConfigDialog : public QDialog, private Ui_ConfigDialog
{
    Q_OBJECT

    public:
        ConfigDialog( QWidget *parent = 0 );
            ~ConfigDialog();
    void cfg_splitSC();
            
    private:
        QNetworkReply* mQueueReply;
            
    private slots:
        void writeSettings();
        
        void addEngineer();
        void removeEngineer();
        void moveEngineerUp();
        void moveEngineerDown();
        
        void addQueue();
        void removeQueue();
        void moveQueueUp();
        void moveQueueDown();
        
        void getQueueList();
        void queueJobDone();
        
        void toggleSystemTray( const bool& );
        void toggleMonitor( const bool& );
        void toggleQboss( const bool& );
        void toggleNotifications( const bool& );
        void toggleUnity( const bool& );
        void toggleUnityEditor( const bool& );
        void toggleUnityTimeout( const bool& );
        void getDownloadDirectory();
        
        void getGeneralNotificationSoundFile();
        void playGeneralNotificationSound();
        
        void getNewPersonalNotificationSoundFile();
        void playNewPersonalNotificationSound();
        
        void getUpdatePersonalNotificationSoundFile();
        void playUpdatePersonalNotificationSound();
        
        void getBomgarNotificationSoundFile();
        void playBomgarNotificationSound();
        
        void getLowNotificationSoundFile();
        void playLowNotificationSound();
        
        void getMediumNotificationSoundFile();
        void playMediumNotificationSound();
        
        void getUrgentNotificationSoundFile();
        void playUrgentNotificationSound();
        
        void getHighNotificationSoundFile();
        void playHighNotificationSound();
        
        void getExternalEditorFile();
        void getEditorSaveLocation();
        void getFilemanagerCommand();
        
    signals:
        void settingsChanged();  
};

class BasicConfig : public QDialog, private Ui::BasicConfig
{
    Q_OBJECT

    public:
        BasicConfig();
         ~BasicConfig();
        
    private slots:
        void saveConfig();
        
};

#endif
