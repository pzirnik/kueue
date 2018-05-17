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

#include "settings.h"
#include "simplecrypt/simplecrypt.h"

#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QColor>

QString Settings::dBServer()
{
    QSettings settings;
    return settings.value( "DBServer" ).toString();
}

void Settings::setdBServer( const QString& s )
{
    QSettings settings;
    settings.setValue( "DBServer", s );
    settings.sync();
}
        
QString Settings::engineer()
{
    QSettings settings;
    return settings.value( "engineer" ).toString();
}

void Settings::setEngineer( const QString& s )
{
    QSettings settings;
    settings.setValue( "engineer", s );
    settings.sync();
}

QString Settings::unityPassword()
{
    // I know this is not really secure, but how would one hide the 
    // encryption key in OSS software? :P
    // At least it's not in the config file in cleartext...
    
    QSettings settings;
    SimpleCrypt crypto( Q_UINT64_C( 0424632454124622 ) );
    QString pw;
    
    if ( !settings.value( "unityPassword" ).toString().isEmpty() )
    {
        pw = crypto.decryptToString( settings.value( "unityPassword" ).toString() );
    }

    return pw;
}

void Settings::setUnityPassword( const QString& s )
{
    SimpleCrypt crypto( Q_UINT64_C( 0424632454124622 ) );
    QString pw = crypto.encryptToString( s );
    QSettings settings;

    settings.setValue( "unityPassword", pw );
    settings.sync();
}
        
bool Settings::showAppWindow()
{
    QSettings settings;
    return settings.value( "showAppWindow", true ).toBool();
}

void Settings::setShowAppWindow( const bool& b )
{
    QSettings settings;
    settings.setValue( "showAppWindow", b );
    settings.sync();
}

bool Settings::showTabsAtTop()
{
    QSettings settings;
    return settings.value( "showTabsAtTop", false ).toBool();
}

void Settings::setShowTabsAtTop( const bool& b )
{        
    QSettings settings;
    settings.setValue( "showTabsAtTop", b );
    settings.sync();
}

bool Settings::enableSsl()
{
    QSettings settings;
    return settings.value( "enableSsl", false ).toBool();
}

void Settings::setEnableSsl( const bool& b )
{
    QSettings settings;
    settings.setValue( "enableSsl", b );
    settings.sync();
}

bool Settings::autopickRecipientDisabled()
{
    QSettings settings;
    return settings.value( "autopickRecipientDisabled", false ).toBool();
}

void Settings::setAutopickRecipientDisabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "autopickRecipientDisabled", b);
    settings.sync();
}

bool Settings::unityEnabled()
{
    QSettings settings;
    return settings.value( "unityEnabled", true ).toBool();
}

void Settings::setUnityEnabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "unityEnabled", b );
    settings.sync();
}

bool Settings::showDownloadManager()
{
    QSettings settings;
    return settings.value( "showDownloadManager", false ).toBool();
}

void Settings::setShowDownloadManager( const bool& b )
{
    QSettings settings;
    settings.setValue( "showDownloadManager", b );
    settings.sync();
}

QString Settings::unityURL()
{
    QSettings settings;
    return settings.value( "unityURL" ).toString();
}

void Settings::setUnityURL( const QString& s )
{
    QSettings settings;
    settings.setValue( "unityURL", s );
    settings.sync();
}

bool Settings::useDefaultFileManager()
{
    QSettings settings;
    return settings.value( "useDefaultFileManager", true ).toBool();
}

void Settings::setUseDefaultFileManager( const bool& b )
{
    QSettings settings;
    settings.setValue( "useDefaultFileManager", b );
    settings.sync();
}

QString Settings::otherFileManagerCommand()
{
    QSettings settings;
    return settings.value( "otherFileManagerCommand", "default" ).toString();
}

void Settings::setOtherFileManagerCommand( const QString& s )
{
    QSettings settings;
    settings.setValue( "otherFileManagerCommand", s );
    settings.sync();
}

bool Settings::useIdleTimeout()
{
    QSettings settings;
    return settings.value( "useIdleTimeout", true ).toBool();
}

void Settings::setUseIdleTimeout( const bool& b )
{
    QSettings settings;
    settings.setValue( "useIdleTimeout", b );
    settings.sync();
}

int Settings::idleTimeoutMinutes()
{
    QSettings settings;
    return settings.value( "idleTimeoutMinutes", 60 ).toInt();
}

void Settings::setIdleTimeoutMinutes( const int& i )
{
    QSettings settings;
    settings.setValue( "idleTimeoutMinutes", i );
    settings.sync();
}

int Settings::minimumFontSize()
{
    QSettings settings;
    return settings.value( "minimumFontSize", 14 ).toInt();
}

void Settings::setMinimumFontSize( const int& i )
{
    QSettings settings;
    settings.setValue( "minimumFontSize", i );
    settings.sync();
}

int Settings::defaultFontSize()
{
    QSettings settings;
    return settings.value( "defaultFontSize", 14 ).toInt();
}

void Settings::setDefaultFontSize( const int& i )
{
    QSettings settings;
    settings.setValue( "defaultFontSize", i );
    settings.sync();
}

int Settings::popupWindowX()
{
    QSettings settings;
    return settings.value( "popupWindowX", 0 ).toInt();
}

void Settings::setPopupWindowX( const int& i )
{
    QSettings settings;
    settings.setValue( "popupWindowX", i );
    settings.sync();
}

int Settings::popupWindowY()
{
    QSettings settings;
    return settings.value( "popupWindowY", 0 ).toInt();
}

void Settings::setPopupWindowY( const int& i )
{
    QSettings settings;
    settings.setValue( "popupWindowY", i );
    settings.sync();
}

bool Settings::externalEditorEnabled()
{
    QSettings settings;
    return settings.value( "externalEditorEnabled", false ).toBool();
}

void Settings::setExternalEditorEnabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "externalEditorEnabled", b );
    settings.sync();
}

QString Settings::editorCommand()
{
    QSettings settings;
    return settings.value( "editorCommand" ).toString();
}

void Settings::setEditorCommand( const QString& s )
{
    QSettings settings;
    settings.setValue( "editorCommand", s );
    settings.sync();
}

QString Settings::editorSaveLocation()
{
    QSettings settings;
    return settings.value( "editorSaveLocation" ).toString();
}

void Settings::setEditorSaveLocation( const QString& s )
{
    QSettings settings;
    settings.setValue( "editorSaveLocation", s );
    settings.sync();
}

bool Settings::replyFormatEnabled()
{
    QSettings settings;
    return settings.value( "replyFormatEnabled", false ).toBool();
}

void Settings::setReplyFormatEnabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "replyFormatEnabled", b );
    settings.sync();
}

int Settings::replyFormatLineBreak()
{
    QSettings settings;
    return settings.value( "replyFormatLineBreak", 75 ).toInt();
}

void Settings::setReplyFormatLineBreak( const int& i )
{
    QSettings settings;
    settings.setValue( "replyFormatLineBreak", i );
    settings.sync();
}

QString Settings::downloadDirectory()
{
    QSettings settings;
    return settings.value( "downloadDirectory", QDir::homePath() ).toString();
}

void Settings::setDownloadDirectory( const QString& s )
{
    QSettings settings;
    settings.setValue( "downloadDirectory", s );
    settings.sync();
}

bool Settings::useSrDirectory()
{
    QSettings settings;
    return settings.value( "useSrDirectory", false ).toBool();
}

void Settings::setUseSrDirectory( const bool& b )
{
    QSettings settings;
    settings.setValue( "useSrDirectory", b );
    settings.sync();
}

bool Settings::autoExtract()
{
    QSettings settings;
    return settings.value( "autoExtract", false ).toBool();
}

void Settings::setAutoExtract( const bool& b )
{
    QSettings settings;
    settings.setValue( "autoExtract", b );
    settings.sync();
}

bool Settings::splitSC()
{
    QSettings settings;
    return settings.value( "splitSC", false ).toBool();
}

void Settings::setSplitSC( const bool& b )
{
    QSettings settings;
    settings.setValue( "splitSC", b );
    settings.sync();
}

bool Settings::autoNSA()
{
    QSettings settings;
    return settings.value( "autoNSA", false ).toBool();
}

void Settings::setAutoNSA( const bool& b )
{
    QSettings settings;
    settings.setValue( "autoNSA", b );
    settings.sync();
}

bool Settings::cleanupDownloadDirectory()
{
    QSettings settings;
    return settings.value( "cleanupDownloadDirectory", false ).toBool();
}

void Settings::setCleanupDownloadDirectory( const bool& b )
{
    QSettings settings;
    settings.setValue( "cleanupDownloadDirectory", b );
    settings.sync();
}

bool Settings::showSystemTray()
{
    QSettings settings;
    return settings.value( "showSystemTray", true ).toBool();
}

void Settings::setShowSystemTray( const bool& b )
{
    QSettings settings;
    settings.setValue( "showSystemTray", b );
    settings.sync();
}

bool Settings::animateQueue()
{
    QSettings settings;
    return settings.value( "animateQueue", true ).toBool();
}

void Settings::setAnimateQueue( const bool& b )
{
    QSettings settings;
    settings.setValue( "animateQueue", b );
    settings.sync();
}
        
bool Settings::animateQmon()
{
    QSettings settings;
    return settings.value( "animateQmon", true ).toBool();
}

void Settings::setAnimateQmon( const bool& b )
{
    QSettings settings;
    settings.setValue( "animateQmon", b );
    settings.sync();
}

int Settings::leftMouseButton()
{
    QSettings settings;
    return settings.value( "leftMouseButton", 0 ).toInt();
}

void Settings::setLeftMouseButton( const int& i )
{
    QSettings settings;
    settings.setValue( "leftMouseButton", i );
    settings.sync();
}

int Settings::middleMouseButton()
{
    QSettings settings;
    return settings.value( "middleMouseButton", 2 ).toInt();
}

void Settings::setMiddleMouseButton( const int& i )
{
    QSettings settings;
    settings.setValue( "middleMouseButton", i );
    settings.sync();
}

int Settings::rightMouseButton()
{
    QSettings settings;
    return settings.value( "rightMouseButton", 1 ).toInt();
}

void Settings::setRightMouseButton( const int& i )
{
    QSettings settings;
    settings.setValue( "rightMouseButton", i );
    settings.sync();
}
        
bool Settings::showSR()
{
    QSettings settings;
    return settings.value( "showSR", true ).toBool();
}

void Settings::setShowSR( const bool& b )
{
    QSettings settings;
    settings.setValue( "showSR", b );
    settings.sync();
}
        
bool Settings::showCR()
{
    QSettings settings;
    return settings.value( "showCR", true ).toBool();
}

void Settings::setShowCR( const bool& b )
{
    QSettings settings;
    settings.setValue( "showCR", b );
    settings.sync();
}
      
bool Settings::showSS()
{
    QSettings settings;
    return settings.value( "showSS", false ).toBool();
}

void Settings::setShowSS( const bool& b )
{
    QSettings settings;
    settings.setValue( "showSS", b );
    settings.sync();
}
        
bool Settings::sortUpdate()
{
    QSettings settings;
    return settings.value( "sortUpdate", true ).toBool();
}

void Settings::setSortUpdate( const bool& b )
{
    QSettings settings;
    settings.setValue( "sortUpdate", b );
    settings.sync();
}
        
bool Settings::sortAge()
{
    QSettings settings;
    return settings.value( "sortAge", false ).toBool();
}

void Settings::setSortAge( const bool& b )
{
    QSettings settings;
    settings.setValue( "sortAge", b );
    settings.sync();
}
        
bool Settings::sortAsc()
{
    QSettings settings;
    return settings.value( "sortAsc", true ).toBool();
}

void Settings::setSortAsc( const bool& b )
{
    QSettings settings;
    settings.setValue( "sortAsc", b );
    settings.sync();
}

bool Settings::showAwaitingCustomer()
{
    QSettings settings;
    return settings.value( "showAwaitingCustomer", true ).toBool();
}

void Settings::setShowAwaitingCustomer( const bool& b )
{
    QSettings settings;
    settings.setValue( "showAwaitingCustomer", b );
    settings.sync();
}

bool Settings::showAwaitingSupport()
{
    QSettings settings;
    return settings.value( "showAwaitingSupport", true ).toBool();
}

void Settings::setShowAwaitingSupport( const bool& b )
{
    QSettings settings;
    settings.setValue( "showAwaitingSupport", b );
    settings.sync();
}

bool Settings::showStatusOthers()
{
    QSettings settings;
    return settings.value( "showStatusOthers", true ).toBool();
}

void Settings::setShowStatusOthers( const bool& b )
{
    QSettings settings;
    settings.setValue( "showStatusOthers", b );
    settings.sync();
}

bool Settings::subShowSR()
{
    QSettings settings;
    return settings.value( "subShowSR", true ).toBool();
}

void Settings::setSubShowSR( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowSR", b );
    settings.sync();
}

bool Settings::subShowCR()
{
    QSettings settings;
    return settings.value( "subShowCR", true ).toBool();
}

void Settings::setSubShowCR( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowCR", b );
    settings.sync();
}

bool Settings::subShowSS()
{
    QSettings settings;
    return settings.value( "subShowSS", false ).toBool();
}

void Settings::setSubShowSS( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowSS", b );
    settings.sync();
}

bool Settings::subSortUpdate()
{
    QSettings settings;
    return settings.value( "subSortUpdate", true ).toBool();
}

void Settings::setSubSortUpdate( const bool& b )
{
    QSettings settings;
    settings.setValue( "subSortUpdate", b );
    settings.sync();
}

bool Settings::subSortAge()
{
    QSettings settings;
    return settings.value( "subSortAge", false ).toBool();
}

void Settings::setSubSortAge( const bool& b )
{
    QSettings settings;
    settings.setValue( "subSortAge", b );
    settings.sync();
}

bool Settings::subSortAsc()
{
    QSettings settings;
    return settings.value( "subSortAsc", true ).toBool();
}

void Settings::setSubSortAsc( const bool& b )
{
    QSettings settings;
    settings.setValue( "subSortAsc", b );
    settings.sync();
}

bool Settings::subShowAwaitingCustomer()
{
    QSettings settings;
    return settings.value( "subShowAwaitingCustomer", true ).toBool();
}

void Settings::setSubShowAwaitingCustomer( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowAwaitingCustomer", b );
    settings.sync();
}

bool Settings::subShowAwaitingSupport()
{
    QSettings settings;
    return settings.value( "subShowAwaitingSupport", true ).toBool();
}

void Settings::setSubShowAwaitingSupport( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowAwaitingSupport", b );
    settings.sync();
}

bool Settings::subShowStatusOthers()
{
    QSettings settings;
    return settings.value( "subShowStatusOthers", true ).toBool();
}

void Settings::setSubShowStatusOthers( const bool& b )
{
    QSettings settings;
    settings.setValue( "subShowStatusOthers", b );
    settings.sync();
}

bool Settings::monitorEnabled()
{
    QSettings settings;
    return settings.value( "monitorEnabled", false ).toBool();
}

void Settings::setMonitorEnabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "monitorEnabled", b );
    settings.sync();
}
        
QStringList Settings::queuesToMonitor()
{
    QSettings settings;
    return settings.value( "queuesToMonitor" ).toStringList();
}

void Settings::setQueuesToMonitor( const QStringList& l )
{
    QSettings settings;
    settings.setValue( "queuesToMonitor", l );
    settings.sync();
}

QString Settings::qmonFilter()
{
    QSettings settings;
    return settings.value( "qmonFilter", "ALL" ).toString();
}

void Settings::setQmonFilter( const QString& s )
{
    QSettings settings;
    settings.setValue( "qmonFilter", s );
    settings.sync();
}

bool Settings::showEmptyQueues()
{
    QSettings settings;
    return settings.value( "showEmptyQueues", true ).toBool();
}

void Settings::setShowEmptyQueues( const bool& b )
{
    QSettings settings;
    settings.setValue( "showEmptyQueues", b );
    settings.sync();
}

bool Settings::monitorPersonalBomgar()
{
    QSettings settings;
    return settings.value( "monitorPersonalBomgar", false ).toBool();
}

void Settings::setMonitorPersonalBomgar( const bool& b )
{
    QSettings settings;
    settings.setValue( "monitorPersonalBomgar", b );
    settings.sync();
}
        
QString Settings::bomgarName()
{
    QSettings settings;
    return settings.value( "bomgarName" ).toString();
}

void Settings::setBomgarName( const QString& s )
{
    QSettings settings;
    settings.setValue( "bomgarName", s );
    settings.sync();
}
        
bool Settings::qbossFeatures()
{
    QSettings settings;
    return settings.value( "qbossFeatures", false ).toBool();
}

void Settings::setQbossFeatures( const bool& b )
{
    QSettings settings;
    settings.setValue( "qbossFeatures", b );
    settings.sync();
}
        
QStringList Settings::engineerList()
{
    QSettings settings;
    return settings.value( "engineerList" ).toStringList();
}

void Settings::setEngineerList( const QStringList& l )  
{
    QSettings settings;
    settings.setValue( "engineerList", l );
    settings.sync();
}
        
bool Settings::statsEnabled()
{
    QSettings settings;
    return settings.value( "statsEnabled", false ).toBool();
}

void Settings::setStatsEnabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "statsEnabled", b );
    settings.sync();
}

bool Settings::notificationsDisabled()
{
    QSettings settings;
    return settings.value( "notificationsDisabled", false ).toBool();
}

void Settings::setNotificationsDisabled( const bool& b )
{
    QSettings settings;
    settings.setValue( "notificationsDisabled", b );
    settings.sync();   
}

bool Settings::generalNotificationPopup()
{
    QSettings settings;
    return settings.value( "generalNotificationPopup", true ).toBool();
}

void Settings::setGeneralNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "generalNotificationPopup", b );
    settings.sync();   
}

bool Settings::generalNotificationSound()
{
    QSettings settings;
    return settings.value( "generalNotificationSound", false ).toBool();
}

void Settings::setGeneralNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "generalNotificationSound", b );
    settings.sync();   
}

QString Settings::generalNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "generalNotificationSoundFile" ).toString();
}

void Settings::setGeneralNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "generalNotificationSoundFile", s );
    settings.sync();
}

bool Settings::generalNotificationWrite()
{
    QSettings settings;
    return settings.value( "generalNotificationWrite", false ).toBool();
}

void Settings::setGeneralNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "generalNotificationWrite", b );
    settings.sync();   
}

QString Settings::generalNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "generalNotificationWriteFile" ).toString();
}

void Settings::setGeneralNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "generalNotificationWriteFile", s );
    settings.sync();
}

bool Settings::newPersonalNotificationPopup()
{
    QSettings settings;
    return settings.value( "newPersonalNotificationPopup", true ).toBool();
}

void Settings::setNewPersonalNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "newPersonalNotificationPopup", b );
    settings.sync();   
}

bool Settings::newPersonalNotificationSound()
{
    QSettings settings;
    return settings.value( "newPersonalNotificationSound", false ).toBool();
}

void Settings::setNewPersonalNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "newPersonalNotificationSound", b );
    settings.sync();   
}

QString Settings::newPersonalNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "newPersonalNotificationSoundFile" ).toString();
}

void Settings::setNewPersonalNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "newPersonalNotificationSoundFile", s );
    settings.sync();
}

bool Settings::newPersonalNotificationWrite()
{
    QSettings settings;
    return settings.value( "newPersonalNotificationWrite", false ).toBool();
}

void Settings::setNewPersonalNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "newPersonalNotificationWrite", b );
    settings.sync();   
}

QString Settings::newPersonalNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "newPersonalNotificationWriteFile" ).toString();
}

void Settings::setNewPersonalNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "newPersonalNotificationWriteFile", s );
    settings.sync();
}

bool Settings::updatePersonalNotificationPopup()
{
    QSettings settings;
    return settings.value( "updatePersonalNotificationPopup", true ).toBool();
}

void Settings::setUpdatePersonalNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "updatePersonalNotificationPopup", b );
    settings.sync();   
}

bool Settings::updatePersonalNotificationSound()
{
    QSettings settings;
    return settings.value( "updatePersonalNotificationSound", false ).toBool();
}

void Settings::setUpdatePersonalNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "updatePersonalNotificationSound", b );
    settings.sync();   
}

QString Settings::updatePersonalNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "updatePersonalNotificationSoundFile" ).toString();
}

void Settings::setUpdatePersonalNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "updatePersonalNotificationSoundFile", s );
    settings.sync();
}

bool Settings::updatePersonalNotificationWrite()
{
    QSettings settings;
    return settings.value( "updatePersonalNotificationWrite", false ).toBool();
}

void Settings::setUpdatePersonalNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "updatePersonalNotificationWrite", b );
    settings.sync();   
}

QString Settings::updatePersonalNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "updatePersonalNotificationWriteFile" ).toString();
}

void Settings::setUpdatePersonalNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "updatePersonalNotificationWriteFile", s );
    settings.sync();
}

bool Settings::bomgarNotificationPopup()
{
    QSettings settings;
    return settings.value( "bomgarNotificationPopup", true ).toBool();
}

void Settings::setBomgarNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "bomgarNotificationPopup", b );
    settings.sync();   
}

bool Settings::bomgarNotificationSound()
{
    QSettings settings;
    return settings.value( "bomgarNotificationSound", false ).toBool();
}

void Settings::setBomgarNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "bomgarNotificationSound", b );
    settings.sync();   
}

QString Settings::bomgarNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "bomgarNotificationSoundFile" ).toString();
}

void Settings::setBomgarNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "bomgarNotificationSoundFile", s );
    settings.sync();
}

bool Settings::bomgarNotificationWrite()
{
    QSettings settings;
    return settings.value( "bomgarNotificationWrite", false ).toBool();
}

void Settings::setBomgarNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "bomgarNotificationWrite", b );
    settings.sync();   
}

QString Settings::bomgarNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "bomgarNotificationWriteFile" ).toString();
}

void Settings::setBomgarNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "bomgarNotificationWriteFile", s );
    settings.sync();
}

bool Settings::lowNotificationPopup()
{
    QSettings settings;
    return settings.value( "lowNotificationPopup", true ).toBool();
}

void Settings::setLowNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "lowNotificationPopup", b );
    settings.sync();   
}

bool Settings::lowNotificationSound()
{
    QSettings settings;
    return settings.value( "lowNotificationSound", false ).toBool();
}

void Settings::setLowNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "lowNotificationSound", b );
    settings.sync();   
}

QString Settings::lowNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "lowNotificationSoundFile" ).toString();
}

void Settings::setLowNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "lowNotificationSoundFile", s );
    settings.sync();
}

bool Settings::lowNotificationWrite()
{
    QSettings settings;
    return settings.value( "lowNotificationWrite", false ).toBool();
}

void Settings::setLowNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "lowNotificationWrite", b );
    settings.sync();   
}

QString Settings::lowNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "lowNotificationWriteFile" ).toString();
}

void Settings::setLowNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "lowNotificationWriteFile", s );
    settings.sync();
}

bool Settings::mediumNotificationPopup()
{
    QSettings settings;
    return settings.value( "mediumNotificationPopup", true ).toBool();
}

void Settings::setMediumNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "mediumNotificationPopup", b );
    settings.sync();   
}

bool Settings::mediumNotificationSound()
{
    QSettings settings;
    return settings.value( "mediumNotificationSound", false ).toBool();
}

void Settings::setMediumNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "mediumNotificationSound", b );
    settings.sync();   
}

QString Settings::mediumNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "mediumNotificationSoundFile" ).toString();
}

void Settings::setMediumNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "mediumNotificationSoundFile", s );
    settings.sync();
}

bool Settings::mediumNotificationWrite()
{
    QSettings settings;
    return settings.value( "mediumNotificationWrite", false ).toBool();
}

void Settings::setMediumNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "mediumNotificationWrite", b );
    settings.sync();   
}

QString Settings::mediumNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "mediumNotificationWriteFile" ).toString();
}

void Settings::setMediumNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "mediumNotificationWriteFile", s );
    settings.sync();
}

bool Settings::urgentNotificationPopup()
{
    QSettings settings;
    return settings.value( "urgentNotificationPopup", true ).toBool();
}

void Settings::setUrgentNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "urgentNotificationPopup", b );
    settings.sync();   
}

bool Settings::urgentNotificationSound()
{
    QSettings settings;
    return settings.value( "urgentNotificationSound", false ).toBool();
}

void Settings::setUrgentNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "urgentNotificationSound", b );
    settings.sync();   
}

QString Settings::urgentNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "urgentNotificationSoundFile" ).toString();
}

void Settings::setUrgentNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "urgentNotificationSoundFile", s );
    settings.sync();
}

bool Settings::urgentNotificationWrite()
{
    QSettings settings;
    return settings.value( "urgentNotificationWrite", false ).toBool();
}

void Settings::setUrgentNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "urgentNotificationWrite", b );
    settings.sync();   
}

QString Settings::urgentNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "urgentNotificationWriteFile" ).toString();
}

void Settings::setUrgentNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "urgentNotificationWriteFile", s );
    settings.sync();
}

bool Settings::highNotificationPopup()
{
    QSettings settings;
    return settings.value( "highNotificationPopup", true ).toBool();
}

void Settings::setHighNotificationPopup( const bool& b )
{
    QSettings settings;
    settings.setValue( "highNotificationPopup", b );
    settings.sync();   
}

bool Settings::highNotificationSound()
{
    QSettings settings;
    return settings.value( "highNotificationSound", false ).toBool();
}

void Settings::setHighNotificationSound( const bool& b )
{
    QSettings settings;
    settings.setValue( "highNotificationSound", b );
    settings.sync();   
}

QString Settings::highNotificationSoundFile()
{
    QSettings settings;
    return settings.value( "highNotificationSoundFile" ).toString();
}

void Settings::setHighNotificationSoundFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "highNotificationSoundFile", s );
    settings.sync();
}

bool Settings::highNotificationWrite()
{
    QSettings settings;
    return settings.value( "highNotificationWrite", false ).toBool();
}

void Settings::setHighNotificationWrite( const bool& b )
{
    QSettings settings;
    settings.setValue( "highNotificationWrite", b );
    settings.sync();   
}

QString Settings::highNotificationWriteFile()
{
    QSettings settings;
    return settings.value( "highNotificationWriteFile" ).toString();
}

void Settings::setHighNotificationWriteFile( const QString& s )
{
    QSettings settings;
    settings.setValue( "highNotificationWriteFile", s );
    settings.sync();
}

bool Settings::settingsOK()
{
    QSettings settings;
    
    if ( dBServer().isEmpty() || engineer().isEmpty() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

QString Settings::appVersion()
{
    QSettings settings;
    return settings.value( "appVersion", "NEW" ).toString();
}

void Settings::setAppVersion( const QString& s )
{
    QSettings settings;
    settings.setValue( "appVersion", s );
    settings.sync();
}

QPoint Settings::mainWinPos()
{
    QSettings settings;
    return settings.value( "mainWinPos" ).toPoint();
}

void Settings::setMainWinPos( const QPoint& p )
{
    QSettings settings;
    settings.setValue( "mainWinPos", p );
    settings.sync();
}

QSize Settings::mainWinSize()
{
    QSettings settings;
    return settings.value( "mainWinSize", QSize( 800, 800 ) ).toSize();
}

void Settings::setMainWinSize( const QSize& s )
{
    QSettings settings;
    settings.setValue( "mainWinSize", s );
    settings.sync();
}

QByteArray Settings::mainWinState()
{
    QSettings settings;
    return settings.value( "mainWinState" ).toByteArray();
}
 
void Settings::setMainWinState( const QByteArray& s )
{
    QSettings settings;
    settings.setValue( "mainWinState", s );
    settings.sync();
}

QPoint Settings::detWinPos()
{
    QSettings settings;
    return settings.value( "detWinPos" ).toPoint();
}

void Settings::setDetWinPos( const QPoint& p )
{
    QSettings settings;
    settings.setValue( "detWinPos", p );
    settings.sync();
}

QSize Settings::detWinSize()
{
    QSettings settings;
    return settings.value( "detWinSize", QSize( 600, 800 ) ).toSize();
}

void Settings::setDetWinSize( const QSize& s )
{
    QSettings settings;
    settings.setValue( "detWinSize", s );
    settings.sync();
}

QByteArray Settings::detWinState()
{
    QSettings settings;
    return settings.value( "detWinState" ).toByteArray();
}
 
void Settings::setDetWinState( const QByteArray& s )
{
    QSettings settings;
    settings.setValue( "detWinState", s );
    settings.sync();
}

QPoint Settings::nsaWinPos()
{
    QSettings settings;
    return settings.value( "nsaWinPos" ).toPoint();
}

void Settings::setNsaWinPos( const QPoint& p )
{
    QSettings settings;
    settings.setValue( "nsaWinPos", p );
    settings.sync();
}

QSize Settings::nsaWinSize()
{
    QSettings settings;
    return settings.value( "nsaWinSize", QSize( 600, 800 ) ).toSize();
}

void Settings::setNsaWinSize( const QSize& s )
{
    QSettings settings;
    settings.setValue( "nsaWinSize", s );
    settings.sync();
}

QByteArray Settings::nsaWinState()
{
    QSettings settings;
    return settings.value( "nsaWinState" ).toByteArray();
}
 
void Settings::setNsaWinState( const QByteArray& s )
{
    QSettings settings;
    settings.setValue( "nsaWinState", s );
    settings.sync();
}

QString Settings::nsaVersion()
{
    QSettings settings;
    return settings.value( "nsaVersion" ).toString();
}

void Settings::setNsaVersion( const QString& s )
{
    QSettings settings;
    settings.setValue( "nsaVersion", s );
    settings.sync();
}

QString Settings::ratingAcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingAcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#70ff70");
    }
    return color;
}

void Settings::setratingAcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingAcol", color.name() );
    settings.sync();
}

QString Settings::ratingBcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingBcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#90ff90");
    }
    return color;
}

void Settings::setratingBcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingBcol", color.name() );
    settings.sync();
}

QString Settings::ratingCcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingCcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#b0ffb0");
    }
    return color;
}

void Settings::setratingCcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingCcol", color.name() );
    settings.sync();
}

QString Settings::ratingDcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingDcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#d0ffd0");
    }
    return color;
}

void Settings::setratingDcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingDcol", color.name() );
    settings.sync();
}

QString Settings::ratingEcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingEcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#f0fff0");
    }
    return color;
}

void Settings::setratingEcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingEcol", color.name() );
    settings.sync();
}

QString Settings::ratingFcol()
{
    QSettings settings;
    QString color;
    color = settings.value( "ratingFcol" ).toString();
    if ( color.isEmpty() ) {
        return ( "#E8E8E8");
    }
    return color;
}

void Settings::setratingFcol( const QColor& color )
{
    QSettings settings;
    settings.setValue( "ratingFcol", color.name() );
    settings.sync();
}
