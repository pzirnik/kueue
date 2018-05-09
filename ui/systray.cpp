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

#include "systray.h"
#include "kueue.h"
#include "settings.h"
#include "tabwidget.h"

#include <QMenu>
#include <QShortcut>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProgressBar>
#include <QWidgetAction>
#include <QToolButton>

#ifdef QT_HAS_DBUS
    #include <QtDBus/QDBusConnection>
    #include <QtDBus/QDBusConnectionInterface>
#endif

Systray* Systray::instance = 0;

Systray& Systray::tray()
{
    if ( !instance )
    {
        instance = new Systray;
    }
 
    return *instance; 
}

void Systray::destroy()
{
    if ( instance )
    {
        delete instance;
    }
      
    instance = 0;
}

Systray::Systray()
{
    qDebug() << "[SYSTEMTRAY] Constructing";
    
    QMenu* menu = new QMenu();

    QAction* ba = new QAction( menu );
    QAction* det = new QAction( "Details for SR# in clipboard", menu );
    QAction* un = new QAction( "Open SR# in clipboard in Unity", menu );
    QAction* hr = new QAction( "Hide/Raise window", menu );
    QAction* quit = new QAction( "Quit", menu );

    QWidgetAction* wa = new QWidgetAction( menu );

    QFont font = ba->font();
    font.setBold( true );

    QToolButton* titleButton = new QToolButton( menu );

    wa->setDefaultWidget( titleButton );

    titleButton->setDefaultAction( ba );
    titleButton->setDown( true );
    titleButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    ba->setFont( font );
    ba->setIcon( QIcon( ":/icons/kueue.png" ) );
    ba->setText( "kueue" );

    det->setData( "det" );
    un->setData( "un" );
    hr->setData( "hr" );
    quit->setData( "quit" );
    
    det->setIcon( QIcon( ":/icons/menus/ddesc.png" ) );
    un->setIcon( QIcon( ":/icons/menus/siebel.png" ) );
    hr->setIcon( QIcon( ":/icons/menus/toggle.png" ) );
    quit->setIcon( QIcon( ":/icons/menus/quit.png" ) );
    
    menu->addAction( wa );
    menu->addAction( det );
    menu->addSeparator();
    menu->addAction( un );
    menu->addSeparator();
    menu->addAction( hr );
    menu->addSeparator();
    menu->addAction( quit );
    
    setContextMenu( menu );
    
    connect( menu, SIGNAL( triggered( QAction* ) ), 
             this, SLOT( trayMenuItemTriggered(QAction*) ) );
    
    connect( this, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
             this, SLOT( trayIconActivated( QSystemTrayIcon::ActivationReason ) ) );

    #ifdef QT_HAS_DBUS
    
        dbusServiceName = "org.freedesktop.Notifications";
        dbusInterfaceName = "org.freedesktop.Notifications";
        dbusPath = "/org/freedesktop/Notifications";

        QDBusConnectionInterface* interface = QDBusConnection::sessionBus().interface();
        
        if ( !interface || !interface->isServiceRegistered( dbusServiceName ) ) 
        {
            qDebug() << "[SYSTRAY] Service " << dbusServiceName << "not registered or no interface";
        }
        
        QDBusConnection::sessionBus().connect( QString(), dbusPath, dbusInterfaceName, "ActionInvoked", 
                                            this, SLOT( notificationActionInvoked( uint, QString ) ) );
        
        QDBusMessage m = QDBusMessage::createMethodCall( dbusServiceName, dbusPath, dbusInterfaceName, "GetServerInformation" );
        QDBusMessage replyMsg = QDBusConnection::sessionBus().call( m );
        
        QList<QVariant> replylist = replyMsg.arguments();
        QStringList replies;
        
        for ( int i = 0; i < replylist.size(); ++i ) 
        {
            replies.append( replylist.at( i ).toString() );
        }
        
        if ( replies.contains( "GNOME" ) ) 
        {
            mIsGnome = true;
        }
        else
        {
            mIsGnome = false;
        }
    
    #else
    
        connect( this, SIGNAL( messageClicked() ), this, SLOT( winMessageClicked() ) );
    
    #endif

    mMovie = new QMovie( ":/icons/app/kueue-animated.mng" );
    
    connect( mMovie, SIGNAL( frameChanged(int) ), this, SLOT( changeIcon() ) );
        
    changeIcon();

    if ( Settings::showSystemTray() )
    {
        show();
    }
}

Systray::~Systray()
{
    qDebug() << "[SYSTEMTRAY] Destroying";

}

void Systray::trayMenuItemTriggered( QAction* action )
{
    if ( ( action->data() == "det" ) &&
         ( Kueue::isSrNr( Kueue::getClipboard() ) ) )
    {
        Kueue::showDetailedDescriptionNB( Kueue::getClipboard() );
    }
    else if ( ( action->data() == "un" ) &&
              ( Kueue::isSrNr( Kueue::getClipboard() ) ) )
    {
        TabWidget::openInUnity( Kueue::getClipboard() );
    }
    else if ( action->data() == "hr" )
    {
        Kueue::toggleWindow();
    }
    else if ( action->data() == "quit" )
    {
        qApp->quit();
    }
}

void Systray::trayIconActivated( QSystemTrayIcon::ActivationReason reason )
{
    if ( reason == 3 )
    {   
        #ifndef IS_OSX 
        
            Kueue::attention( false );
            Kueue::toggleWindow();
        
        #endif
    }
    else if ( ( reason == 4 ) &&
              ( Kueue::isSrNr( Kueue::getClipboard() ) ) )
    {
        Kueue::showDetailedDescriptionNB( Kueue::getClipboard() );
    }
}

void Systray::notify( const QString& type, QString title, QString body, const QString& sr )
{
    if ( !Settings::notificationsDisabled() )
    {
        #ifdef QT_HAS_DBUS    

            QStringList actions;
            QList<QVariant> args;
            
            if ( type.startsWith( "kueue-sr" ) || type.startsWith( "kueue-monitor" ) )
            {
                actions = QStringList() << "det|" + sr << "Details" << "un|" + sr << "Unity" << "cl|" + sr << "Close";
            }

            if ( mIsGnome )
            {
                body.replace( "<br>", " -- " );
            }
            
            args.append( "kueue" );
            args.append( 0U );
            args.append( "kueue" );
            args.append( title );
            args.append( body );
            args.append( actions );
            args.append( QVariantMap() );
            args.append( 5 * 1000 );    

            QDBusMessage m = QDBusMessage::createMethodCall( dbusServiceName, dbusPath, dbusInterfaceName, "Notify" );
            m.setArguments( args );

        #else
        
            body.remove( "<b>");
            body.remove( "</b>" );
            body.replace( "<br>", " -- " );
            mCurrentMsg = sr;
        
        #endif
        
        if ( type == "kueue-general" )
        {
            if ( Settings::generalNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                
                #endif
            }
            
            if ( Settings::generalNotificationSound() )
            {
                Kueue::playSound( Settings::generalNotificationSoundFile() );
            }
        }
        else if ( type == "kueue-alert" )
        {   
            #ifdef QT_HAS_DBUS
            
                QStringList actions;
                QList<QVariant> args;
                
                actions = QStringList() << "cl|" << "Close";
                args.append( "kueue" );
                args.append( 0U );
                args.append( "kueue" );
                args.append( title );
                args.append( body );
                args.append( actions );
                args.append( QVariantMap() );
                args.append( 0 );
                QDBusMessage m = QDBusMessage::createMethodCall( dbusServiceName, dbusPath, dbusInterfaceName, "Notify" );
                m.setArguments( args );
                bool queued = QDBusConnection::sessionBus().send( m );
                
            #else
                
                showMessage( title, body, QSystemTrayIcon::Information, 0 );
                
            #endif
                
        }
        else if ( type == "kueue-sr-new" )
        {
            if ( Settings::newPersonalNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                            
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                        
                #endif
            }
            
            if ( Settings::newPersonalNotificationSound() )
            {
                Kueue::playSound( Settings::newPersonalNotificationSoundFile() );
            }
            
            if ( Settings::animateQueue() )
            {
                requiresAttention( true );
            }
        }
        else if ( type == "kueue-sr-update" ) 
        {
            if ( Settings::updatePersonalNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                        
                #else
                            
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                            
                #endif
            }
            
            if ( Settings::updatePersonalNotificationSound() )
            {
                Kueue::playSound( Settings::updatePersonalNotificationSoundFile() );
            }
            
            if ( Settings::animateQueue() )
            {
                requiresAttention( true );
            }
        }    
        else if ( type == "kueue-monitor-low" )
        {
            if ( Settings::lowNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                            
                #else
                
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                
                #endif
            }
            
            if ( Settings::lowNotificationSound() )
            {
                Kueue::playSound( Settings::lowNotificationSoundFile() );
            }
            
            if ( Settings::animateQmon() )
            {
                requiresAttention( true );
            }
        }
        else if ( type == "kueue-monitor-medium" )
        {
            if ( Settings::mediumNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                            
                #endif
            }
            
            if ( Settings::mediumNotificationSound() )
            {
                Kueue::playSound( Settings::mediumNotificationSoundFile() );
            }
            
            if ( Settings::animateQmon() )
            {
                requiresAttention( true );
            }
        }
        else if ( type == "kueue-monitor-urgent" )
        {
            if ( Settings::urgentNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                            
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                
                #endif
            }
            
            if ( Settings::urgentNotificationSound() )
            {
                Kueue::playSound( Settings::urgentNotificationSoundFile() );
            }
            
            if ( Settings::animateQmon() )
            {
                requiresAttention( true );
            }
        }
        else if ( type == "kueue-monitor-high" )
        {
            if ( Settings::highNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                
                #endif
            }
            
            if ( Settings::highNotificationSound() )
            {
                Kueue::playSound( Settings::highNotificationSoundFile() );
            }
            
            if ( Settings::animateQmon() )
            {
                requiresAttention( true );
            }
        }
        else if ( type == "kueue-personal-bomgar" )
        {
            if ( Settings::bomgarNotificationPopup() )
            {
                #ifdef QT_HAS_DBUS
                
                    bool queued = QDBusConnection::sessionBus().send( m );
                
                #else
                
                    showMessage( title, body, QSystemTrayIcon::Information, 100000 );
                
                #endif
            }
            
            if ( Settings::bomgarNotificationSound() )
            {
                Kueue::playSound( Settings::bomgarNotificationSoundFile() );
            }
            
            if ( Settings::animateQmon() )
            {
                requiresAttention( true );
            }
        }
    }
}

void Systray::notificationActionInvoked( uint id, QString type )
{
    qDebug() << "[SYSTRAY] notificationActionInvoked: id" << id << "With string" << type;
    
    if ( type.startsWith( "det|" ) )
    {
        Kueue::showDetailedDescription( type.remove( "det|" ) );
    }
    else if ( type.startsWith( "un|" ) )
    {
        TabWidget::newUnityWithSR( type.remove( "un|" ) );
    }
    else if ( type.startsWith( "cl|" ) )
    {
        closeNotification( id );
    }
    requiresAttention( false );
    
}

void Systray::closeNotification( uint id )
{
    #ifdef QT_HAS_DBUS    
    
        QList<QVariant> args;
        args.append( id );

        QDBusMessage m = QDBusMessage::createMethodCall( dbusServiceName, dbusPath, dbusInterfaceName, "CloseNotification" );
        m.setArguments( args );
        
        QDBusConnection::sessionBus().send( m );
    
    #endif
}

void Systray::requiresAttention( const bool& b )
{
#ifndef IS_WIN32
#ifndef IS_OSX

    if ( b )
    {
        if ( mMovie->state() != QMovie::Running )
        {
            mMovie->start();
        }
    }
    else
    {
        if ( mMovie->state() == QMovie::Running )
        {
            mMovie->stop();
        }
        
        changeIcon();
    }
    
#endif
#endif
}

void Systray::changeIcon()
{
    if ( mMovie->state() == QMovie::Running )
    {
        setIcon( mMovie->currentPixmap() );
    }
    else
    {
        setIcon( QIcon( ":/icons/kueue.png" ) );
    }
}

void Systray::winMessageClicked()
{
    if ( Kueue::isSrNr( mCurrentMsg ) )
    {
        Kueue::showDetailedDescription( mCurrentMsg );
    }
}

#include "systray.moc"
