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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QByteArray>

class Settings
{
    public:
       
        /* General Group */
        
        static QString dBServer();
        static void setdBServer( const QString& );
        
        static QString engineer();
        static void setEngineer( const QString& );

        static QString unityPassword();
        static void setUnityPassword( const QString& );
       
        static bool showAppWindow();
        static void setShowAppWindow( const bool& );

        static bool showTabsAtTop();
        static void setShowTabsAtTop( const bool& );
        
        static bool enableSsl();
        static void setEnableSsl( const bool& );
        
        static bool autopickRecipientDisabled();
        static void setAutopickRecipientDisabled( const bool& );
        
        static bool unityEnabled();
        static void setUnityEnabled( const bool& );
        
        static bool showDownloadManager();
        static void setShowDownloadManager( const bool& );
        
        static QString unityURL();
        static void setUnityURL( const QString& );
        
        static bool useDefaultFileManager();
        static void setUseDefaultFileManager( const bool& );
        
        static QString otherFileManagerCommand();
        static void setOtherFileManagerCommand( const QString& );
        
        static bool useIdleTimeout();
        static void setUseIdleTimeout( const bool& );
        
        static int idleTimeoutMinutes();
        static void setIdleTimeoutMinutes( const int& );
        
        static int minimumFontSize();
        static void setMinimumFontSize( const int& );
        
        static int defaultFontSize();
        static void setDefaultFontSize( const int& );
        
        static int popupWindowX();
        static void setPopupWindowX( const int& );
        
        static int popupWindowY();
        static void setPopupWindowY( const int& );
        
        static bool externalEditorEnabled();
        static void setExternalEditorEnabled( const bool& );
        
        static QString editorCommand();
        static void setEditorCommand( const QString& );

        static QString editorSaveLocation();
        static void setEditorSaveLocation( const QString& );
        
        static bool replyFormatEnabled();
        static void setReplyFormatEnabled( const bool& );
        
        static int replyFormatLineBreak();
        static void setReplyFormatLineBreak( const int& );
        
        static QString downloadDirectory();
        static void setDownloadDirectory( const QString& );
        
        static bool useSrDirectory();
        static void setUseSrDirectory( const bool& );
                
        static bool autoExtract();
        static void setAutoExtract( const bool& );    
        
        static bool splitSC();
        static void setSplitSC( const bool& );
        
        static bool autoNSA();
        static void setAutoNSA( const bool& );
        
        static bool cleanupDownloadDirectory();
        static void setCleanupDownloadDirectory( const bool& );
        
        static bool showSystemTray();
        static void setShowSystemTray( const bool& );
        
        static bool animateQueue();
        static void setAnimateQueue( const bool& );
        
        static bool animateQmon();
        static void setAnimateQmon( const bool& );
        
        static int leftMouseButton();
        static void setLeftMouseButton( const int& );
        
        static int middleMouseButton();
        static void setMiddleMouseButton( const int& );
        
        static int rightMouseButton();
        static void setRightMouseButton( const int& );
        
        /* View Group */
        
        static bool showSR();
        static void setShowSR( const bool& );
        
        static bool showCR();
        static void setShowCR( const bool& );
        
        static bool showSS();
        static void setShowSS( const bool& );
        
        static bool sortUpdate();
        static void setSortUpdate( const bool& );
        
        static bool sortAge();
        static void setSortAge( const bool& );
        
        static bool sortAsc();
        static void setSortAsc( const bool& );
        
        static bool showAwaitingCustomer();
        static void setShowAwaitingCustomer( const bool& );
        
        static bool showAwaitingSupport();
        static void setShowAwaitingSupport( const bool& );
        
        static bool showStatusOthers();
        static void setShowStatusOthers( const bool& );
                
	
        static bool subShowSR();
        static void setSubShowSR( const bool& );
        
        static bool subShowCR();
        static void setSubShowCR( const bool& );
        
        static bool subShowSS();
        static void setSubShowSS( const bool& );
        
        static bool subSortUpdate();
        static void setSubSortUpdate( const bool& );
        
        static bool subSortAge();
        static void setSubSortAge( const bool& );
        
        static bool subSortAsc();
        static void setSubSortAsc( const bool& );
        
        static bool subShowAwaitingCustomer();
        static void setSubShowAwaitingCustomer( const bool& );
        
        static bool subShowAwaitingSupport();
        static void setSubShowAwaitingSupport( const bool& );
        
        static bool subShowStatusOthers();
        static void setSubShowStatusOthers( const bool& );
        
        /* Monitor Group */
        
        static bool monitorEnabled();
        static void setMonitorEnabled( const bool& );
        
        static QStringList queuesToMonitor();
        static void setQueuesToMonitor( const QStringList& );
        
        static bool showEmptyQueues();
        static void setShowEmptyQueues( const bool& );
        
        static bool monitorPersonalBomgar();
        static void setMonitorPersonalBomgar( const bool& );
        
        static QString bomgarName();
        static void setBomgarName( const QString& );
        
        static QString qmonFilter();
        static void setQmonFilter( const QString& );
        
        /* QBoss Group */
        
        static bool qbossFeatures();
        static void setQbossFeatures( const bool& );
        
        static QStringList engineerList();
        static void setEngineerList( const QStringList& );  
        
        /* Stats Group */
        
        static bool statsEnabled();
        static void setStatsEnabled( const bool& );
        
        /* Notifications Group */
        
        static bool notificationsDisabled();
        static void setNotificationsDisabled( const bool& );

        static bool generalNotificationPopup();
        static void setGeneralNotificationPopup( const bool& );

        static bool generalNotificationSound();
        static void setGeneralNotificationSound( const bool& );

        static QString generalNotificationSoundFile();
        static void setGeneralNotificationSoundFile( const QString& );

        static bool generalNotificationWrite();
        static void setGeneralNotificationWrite( const bool& );

        static QString generalNotificationWriteFile();
        static void setGeneralNotificationWriteFile( const QString& );

        static bool newPersonalNotificationPopup();
        static void setNewPersonalNotificationPopup( const bool& );

        static bool newPersonalNotificationSound();
        static void setNewPersonalNotificationSound( const bool& );

        static QString newPersonalNotificationSoundFile();
        static void setNewPersonalNotificationSoundFile( const QString& );

        static bool newPersonalNotificationWrite();
        static void setNewPersonalNotificationWrite( const bool& );

        static QString newPersonalNotificationWriteFile();
        static void setNewPersonalNotificationWriteFile( const QString& );

        static bool updatePersonalNotificationPopup();
        static void setUpdatePersonalNotificationPopup( const bool& );

        static bool updatePersonalNotificationSound();
        static void setUpdatePersonalNotificationSound( const bool& );

        static QString updatePersonalNotificationSoundFile();
        static void setUpdatePersonalNotificationSoundFile( const QString& );

        static bool updatePersonalNotificationWrite();
        static void setUpdatePersonalNotificationWrite( const bool& );

        static QString updatePersonalNotificationWriteFile();
        static void setUpdatePersonalNotificationWriteFile( const QString& );

        static bool bomgarNotificationPopup();
        static void setBomgarNotificationPopup( const bool& );

        static bool bomgarNotificationSound();
        static void setBomgarNotificationSound( const bool& );

        static QString bomgarNotificationSoundFile();
        static void setBomgarNotificationSoundFile( const QString& );

        static bool bomgarNotificationWrite();
        static void setBomgarNotificationWrite( const bool& );

        static QString bomgarNotificationWriteFile();
        static void setBomgarNotificationWriteFile( const QString& );

        static bool lowNotificationPopup();
        static void setLowNotificationPopup( const bool& );

        static bool lowNotificationSound();
        static void setLowNotificationSound( const bool& );

        static QString lowNotificationSoundFile();
        static void setLowNotificationSoundFile( const QString& );

        static bool lowNotificationWrite();
        static void setLowNotificationWrite( const bool& );

        static QString lowNotificationWriteFile();
        static void setLowNotificationWriteFile( const QString& );

        static bool mediumNotificationPopup();
        static void setMediumNotificationPopup( const bool& );

        static bool mediumNotificationSound();
        static void setMediumNotificationSound( const bool& );

        static QString mediumNotificationSoundFile();
        static void setMediumNotificationSoundFile( const QString& );

        static bool mediumNotificationWrite();
        static void setMediumNotificationWrite( const bool& );

        static QString mediumNotificationWriteFile();
        static void setMediumNotificationWriteFile( const QString& );

        static bool urgentNotificationPopup();
        static void setUrgentNotificationPopup( const bool& );

        static bool urgentNotificationSound();
        static void setUrgentNotificationSound( const bool& );

        static QString urgentNotificationSoundFile();
        static void setUrgentNotificationSoundFile( const QString& );

        static bool urgentNotificationWrite();
        static void setUrgentNotificationWrite( const bool& );

        static QString urgentNotificationWriteFile();
        static void setUrgentNotificationWriteFile( const QString& );

        static bool highNotificationPopup();
        static void setHighNotificationPopup( const bool& );

        static bool highNotificationSound();
        static void setHighNotificationSound( const bool& );

        static QString highNotificationSoundFile();
        static void setHighNotificationSoundFile( const QString& );

        static bool highNotificationWrite();
        static void setHighNotificationWrite( const bool& );

        static QString highNotificationWriteFile();
        static void setHighNotificationWriteFile( const QString& );
        
        /* Misc */
        
        static bool settingsOK();

        static QString appVersion();
        static void setAppVersion( const QString& );
        
        static QPoint mainWinPos();
        static void setMainWinPos( const QPoint& );
        
        static QSize mainWinSize();
        static void setMainWinSize( const QSize& );
        
        static QByteArray mainWinState();
        static void setMainWinState( const QByteArray& );
        
        static QPoint detWinPos();
        static void setDetWinPos( const QPoint& );
        
        static QSize detWinSize();
        static void setDetWinSize( const QSize& );
        
        static QByteArray detWinState();
        static void setDetWinState( const QByteArray& );
        
        static QPoint nsaWinPos();
        static void setNsaWinPos( const QPoint& );
        
        static QSize nsaWinSize();
        static void setNsaWinSize( const QSize& );
        
        static QByteArray nsaWinState();
        static void setNsaWinState( const QByteArray& );
        
        static QString nsaVersion();
        static void setNsaVersion( const QString& );
};    

#endif
