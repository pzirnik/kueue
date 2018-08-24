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

#include "qmonbrowser.h"
#include "settings.h"
#include "kueue.h"
#include "data/database.h"
#include "ui/html.h"
#include "ui/tabwidget.h"
#include "data/datathread.h"

#include <QMessageBox>
#include <QMenu>
#include <QWebFrame>
#include <QWidgetAction>
#include <QWebInspector>
#include <QShortcut>
#include <QGridLayout>
#include <QToolButton>
#include <QWebElementCollection>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QDebug>


QMonBrowser::QMonBrowser( QObject *parent )
            : Browser( ( QWidget* ) 0 )
{
    qDebug() << "[QMONBROWSER] Constructing";
    
    (void)parent;
    
    mFilter = QString::Null();
    
    connect( page(), SIGNAL( linkHovered( const QString&, const QString&, const QString& ) ), 
             this, SLOT( urlHovered( const QString&, const QString&, const QString& ) ) );
        
    QShortcut* shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_I ), this );
   
    connect( shortcut, SIGNAL( activated() ), 
             this, SLOT( openWebInspector() ) );
    
    QShortcut* emea = new QShortcut( QKeySequence( Qt::Key_F1 ), this );
    emea->setObjectName( "emea" );
    
    connect( emea, SIGNAL( activated() ), 
             this, SLOT( setFilter() ) );
    
    QShortcut* us = new QShortcut( QKeySequence( Qt::Key_F2 ), this );
    us->setObjectName( "us" );
    
    connect( us, SIGNAL( activated() ), 
             this, SLOT( setFilter() ) );
    
    QShortcut* apac = new QShortcut( QKeySequence( Qt::Key_F3 ), this );
    apac->setObjectName( "apac" );
    
    connect( apac, SIGNAL( activated() ), 
             this, SLOT( setFilter() ) );
    
    QShortcut* all = new QShortcut( QKeySequence( Qt::Key_F4 ), this );
    all->setObjectName( "all" );
    
    connect( all, SIGNAL( activated() ), 
             this, SLOT( setFilter() ) );
}

QMonBrowser::~QMonBrowser()
{
    qDebug() << "[QMONBROWSER] Destroying";
}

void QMonBrowser::openWebInspector()
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    
    QWidget* w = new QWidget;
    QGridLayout* l = new QGridLayout;
    w->setLayout( l );
    QWebInspector* i = new QWebInspector( w );
    l->addWidget( i );
    i->setPage( page() );
    w->setWindowTitle( "Webinspector - qmonbrowser" );
    w->show();
}

void QMonBrowser::urlHovered( const QString& url, const QString& title, const QString& text )
{
    // Copy the last hovered URL to mSr
    // Necessary to make the mouseButtonEvents work on links
    
    (void)title;
    (void)text;
    
    mUrl = url;
}

void QMonBrowser::update( const QString& html )
{
    //qDebug() << "[QMONBROWSER] Updating";
    
    QPoint pos = page()->currentFrame()->scrollPosition();
    page()->currentFrame()->setHtml( html );
    page()->currentFrame()->setScrollPosition( pos );
}

void QMonBrowser::filter( const QString& filter )
{
    DataThread::updateQmonBrowser( filter );
}

void QMonBrowser::mousePressEvent( QMouseEvent* event )
{
    StatusBar::hideDownloadManager();
    
    // Left mouse button 
    // Different actions for different "protocols"
    
    if ( ( event->button() == 1 ) && 
         ( mUrl.toString().startsWith(  "sr://" ) ) )
    {
        if ( Settings::leftMouseButton() == 1 )
        {
            contextMenu( event, mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::leftMouseButton() == 2 )
        {
            Kueue::setClipboard( mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::leftMouseButton() == 0 )
        {
            toggleSrTable( mUrl.toString().remove( "sr://" ) );
        }
    }
        
    if ( ( event->button() == 1 ) &&
         ( mUrl.toString().startsWith(  "arrow://" ) ) )
    {
        toggleSrTable( mUrl.toString().remove( "arrow://" ) );
    }
        
    if ( ( event->button() == 1 ) && 
         ( mUrl.toString().startsWith(  "mailto" ) ) )
    {
        QDesktopServices::openUrl( mUrl );
    }
    
    // Right mouse button     
    
    if ( ( event->button() == 2 ) && ( mUrl.toString().startsWith(  "sr://" ) ) )
    {
        if ( Settings::rightMouseButton() == 1 )
        {
            contextMenu( event, mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::rightMouseButton() == 2 )
        {
            Kueue::setClipboard( mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::rightMouseButton() == 0 )
        {
            toggleSrTable( mUrl.toString().remove( "sr://" ) );
        }
    }
    
    // Middle mouse button 
  
    if ( ( event->button() == 4 ) && 
         ( mUrl.toString().startsWith(  "sr://" ) ) )
    {
        if ( Settings::middleMouseButton() == 1 )
        {
            contextMenu( event, mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::middleMouseButton() == 2 )
        {
            Kueue::setClipboard( mUrl.toString().remove( "sr://" ) );
        }
        else if ( Settings::middleMouseButton() == 0 )
        {
            toggleSrTable( mUrl.toString().remove( "sr://" ) );
        }
    }
    
    return QWebView::mousePressEvent(event);
}

void QMonBrowser::contextMenu( QMouseEvent* event, const QString& id )
{
    // Create a custom context menu plus a widget to hold the actual menu
    // This makes it look more like a KMenu (way more beautiful)
    
    
    
    QMenu* menu = new QMenu();
            
    QAction* ba = new QAction( menu );
    QAction* oc = new QAction( "Show/Hide SR details", menu );
    QAction* dd = new QAction( "Show detailed description", menu );
    QAction* cb = new QAction( "Copy SR# to clipboard", menu );
    QAction* ts = new QAction( "Take SR and open in new Unity browser", menu );
    QAction* ou = new QAction( "Open in first Unity browser", menu );
    QAction* on = new QAction( "Open in new Unity browser", menu );
    QWidgetAction* wa = new QWidgetAction( menu );
    
    QMenu* sm = new QMenu( menu );
    
    sm->setTitle("move to ...");
    sm->setIcon( QIcon( ":/icons/conf/qboss.png" ) );
    
    QStringList que = Settings::queuesToMonitor();

    for ( int i = 0; i < que.count(); ++i ) 
    {
                QAction* ac;
                
                if ( que.at(i).section( "|", 0, 0 ).isEmpty() )
                {
                    ac = sm->addAction( que.at(i).section( "|", 1, 1 ) );
                }
                else
                {
                    ac = sm->addAction( que.at(i).section( "|", 0, 0 ) );
                }
                ac->setData( id + "#" + que.at(i).section( "|", 1, 1 ) + "|so" );
    }
      
    QFont font = ba->font();
    font.setBold( true );
    
    QToolButton* titleButton = new QToolButton( this );
    
    wa->setDefaultWidget( titleButton );
    wa->setObjectName( "MENUTITLE" );
    
    titleButton->setDefaultAction( ba );
    titleButton->setDown( true );
    titleButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    
    ba->setFont( font );
    ba->setIcon( QIcon( QIcon( ":/icons/kueue.png" ).pixmap( QSize( 16, 16 ) ) ) );
    ba->setText( id );
    
    oc->setData( id + "|tog" );
    dd->setData( id + "|dd" );
    cb->setData( id + "|cb" );
    ts->setData( id + "|ts" );
    ou->setData( id + "|ou" );
    on->setData( id + "|on" );
    
    oc->setIcon( QIcon( ":/icons/menus/toggle.png" ) );
    dd->setIcon( QIcon( ":/icons/menus/ddesc.png" ) );
    cb->setIcon( QIcon( ":/icons/menus/clipboard.png" ) );
    ts->setIcon( QIcon( ":/icons/menus/take.png" ) );
    ou->setIcon( QIcon( ":/icons/menus/siebel.png" ) );
    on->setIcon( QIcon( ":/icons/menus/siebel.png" ) );
    
    menu->addAction( wa );
    menu->addAction( oc );
    menu->addSeparator();
    menu->addAction( dd );
    menu->addSeparator();
    menu->addAction( cb );
    menu->addSeparator();
    menu->addAction( ts );
    
    if ( Settings::unityEnabled() )
    {
        menu->addSeparator();
        if (!sm->isEmpty() )
        {
            menu->addMenu( sm );
        }
        menu->addAction( ou );
        menu->addAction( on );
    }
    
    connect( menu, SIGNAL( triggered( QAction* ) ), 
             this, SLOT( contextMenuItemTriggered( QAction* ) ) );
    
    menu->exec( event->globalPos() );
    
    delete menu;
}

void QMonBrowser::contextMenuEvent(QContextMenuEvent* event)
{
    // Ignore the contextMenuEvent
    // This is necessary to make the right button events work
    
    event->ignore();
}

void QMonBrowser::contextMenuItemTriggered( QAction* a )
{   
    // This is called when a QAction from within the contextMenu is clicked
    // The data.contains thing is an ugly hack... But it works
    
    if ( a->data().toString().contains( "|cb" ) )
    {
        Kueue::setClipboard( a->data().toString().remove( "|cb" ) );
    }
    else if ( a->data().toString().contains( "|tog" ) )
    {
        toggleSrTable( a->data().toString().remove( "|tog" ) );
    }
    else if ( a->data().toString().contains( "|dd" ) )
    {
        Kueue::showDetailedDescription( a->data().toString().remove( "|dd" ), false );
    }
    else if ( a->data().toString().contains( "|ts" ) )
    {
        takeSR( a->data().toString().remove( "|ts" ) );
    }
    else if ( a->data().toString().contains( "|ou" ) )
    {
        TabWidget::openInUnity( a->data().toString().remove( "|ou" ) );
    }
    else if ( a->data().toString().contains( "|on" ) )
    {
        TabWidget::newUnityWithSR( a->data().toString().remove( "|on" ) );
    }
    else if (a->data().toString().contains( "|so" ) )
    {
        assignSR( a->data().toString().remove( "|so" ));
    }

}

void QMonBrowser::toggleSrTable( const QString& id )
{
    // Toggle the detailed SR infos table 
    
    QWebElementCollection list = page()->mainFrame()->findAllElements( "table" );
    
    for ( int i = 0; i < list.count(); ++i ) 
    {
        if ( list.at(i).attribute( "id" ) == id + "_body" ) 
        { 
            if ( list.at(i).attribute( "style" ) == "display:none" ) 
            {
                list.at( i ).setAttribute( "style", "display:block" );
                Database::setQmonDisplay( id + "-block" );
            }
            else if ( list.at(i).attribute( "style" ) == "display:block" ) 
            {
                list.at( i ).setAttribute("style", "display:none" );
                Database::setQmonDisplay( id + "-none" );
            }
        }
    }
}

void QMonBrowser::assignSR( const QString& data)
{
    QString sr;
    QString queue;
    
    sr = data.section( "#", 0, 0 );
    queue = data.section( "#", 1, 1 );
    
    QMessageBox* box = new QMessageBox( this );
    
    box->setText( "Moving <b>" + sr + "</b> to " + queue + " ?" );
    box->setWindowTitle( "Moving" );
    box->setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    box->setDefaultButton( QMessageBox::Yes );
    box->setIcon( QMessageBox::Question );
    
    int reply = box->exec();
    
    if ( reply == QMessageBox::Yes ) 
    {
        showProgress();
        mSR = "NoTab";
        mAssign = Network::get( "assign/" + sr + "|" + queue.toUpper(), true);
        
        connect( mAssign, SIGNAL( finished() ), 
                 this, SLOT( assignFinished() ) );
    }
    
    delete box;
}

void QMonBrowser::takeSR( const QString& sr )
{
    QMessageBox* box = new QMessageBox( this );
    
    box->setText( "Assign <b>" + sr + "</b> to " + Settings::engineer() + "?" );
    box->setWindowTitle( "Assign SR" );
    box->setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    box->setDefaultButton( QMessageBox::Yes );
    box->setIcon( QMessageBox::Question );
    
    int reply = box->exec();
    
    if ( reply == QMessageBox::Yes ) 
    {
        showProgress();
        mSR = sr;
        mAssign = Network::get( "assign/" + sr + "|" + Settings::engineer().toUpper(), true);
        
        connect( mAssign, SIGNAL( finished() ), 
                 this, SLOT( assignFinished() ) );
    }
    
    delete box;
}

void QMonBrowser::showProgress()
{
    mProgress = new QProgressDialog( this );
    mProgress->setWindowTitle( "Assigning/Moving SR/CR" );
    mProgress->setLabelText( "Assigning/Moving SR/CR" );
    mProgress->setMinimum( 0 );
    mProgress->setMaximum( 0 );
    mProgress->setWindowModality( Qt::WindowModal );
    mProgress->show();
}

void QMonBrowser::assignFinished()
{
    QString data;
    
    mProgress->close();
    
    if( mAssign->error() )
    {
            QMessageBox::critical( this, "Error", "Error sending data to Server" );
            qDebug() << "[QMONBROWSER] Assign/move error: " << mAssign->errorString();
    }
    else
    {
        qDebug() << "[QMONBROWSER] Assign/move success: " << mAssign->errorString();
        data = mAssign->readAll();
    }

    if ( data.startsWith( "ASSIGNED" ) )
        QMessageBox::critical( this, "Error", "SR already assigned/moved to " + data.split( "|" ).at( 2 ) );

    else if ( data.startsWith( "SR_NOT_FOUND" ) )
        QMessageBox::critical( this, "Error", "SR not found");

    else if ( data.startsWith( "NO_SR" ) )
        QMessageBox::critical( this, "Error", "No SR number specified" );

    else if ( data.startsWith( "NO_OWNER" ) )
        QMessageBox::critical( this, "Error", "No assignee/target specified" );

    else if ( data.startsWith( "FAILURE" ) )
        QMessageBox::critical( this, "Error", "General failure" );

    else if ( data.startsWith( "SUCCESS" ) )
    {
        QMessageBox::information( this, "Done", "SR successfully assigned/moved" );
        if (!mSR.contains( "NoTab" ) ) 
        {
            TabWidget::newUnityWithSR( mSR );
        }
        mSR.clear();
    }
    
    else QMessageBox::critical( this, "Error", "Unknown reply: " + data );
    
    return;
}

void QMonBrowser::setFilter()
{
    QShortcut* sc = qobject_cast<QShortcut*>( sender() );
    
    if ( sc->objectName() == "emea" )
    {
        Settings::setQmonFilter( "EMEA" );
    }
    else if ( sc->objectName() == "us" )
    {
        Settings::setQmonFilter( "USA" );
    }
    else if ( sc->objectName() == "apac" )
    {
        Settings::setQmonFilter( "ASIAPAC" );
    }
    else
    {
        Settings::setQmonFilter( "ALL" );
    }
    
    DataThread::updateQmonBrowser();
}


#include "qmonbrowser.moc"
