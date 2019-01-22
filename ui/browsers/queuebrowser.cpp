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

#include "queuebrowser.h"
#include "settings.h"
#include "kueue.h"
#include "data/database.h"
#include "data/datathread.h"
#include "ui/tabwidget.h"
#include "ui/html.h"

#include <QDebug>
#include <QShortcut>
#include <QGridLayout>
#include <QWebInspector>
#include <QWebFrame>
#include <QMenu>
#include <QWidgetAction>
#include <QWebElementCollection>
#include <QToolButton>
#include <QDesktopServices>

QueueBrowser::QueueBrowser( QObject *parent )
        : Browser( ( QWidget* ) 0 )
{
    qDebug() << "[QUEUEBROWSER] Constructing";
    
    (void)parent;
    
    mOpen = false;
    m_currentZoom = Settings::zoomFactor();
    this->setZoomFactor(static_cast<qreal>(m_currentZoom)/100.0);
    m_zoomLevels << 30 << 50 << 67 << 80 << 90;
    m_zoomLevels << 100;
    m_zoomLevels << 110 << 120 << 133 << 150 << 170 << 200 << 240 << 300;
    
    connect( page(), SIGNAL( linkHovered( const QString&, const QString&, const QString& ) ), this, SLOT( urlHovered( const QString&, const QString&, const QString& ) ) );
    
    QShortcut* shortcut_zi = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_Plus) , this );
    
    connect( shortcut_zi, SIGNAL( activated() ),
             this, SLOT( zoomIn() ) );
    
    QShortcut* shortcut_zo = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_Minus) , this );
    
    connect( shortcut_zo, SIGNAL( activated() ),
             this, SLOT( zoomOut() ) );
    
    QShortcut* shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_I ), this );
    
    connect( shortcut, SIGNAL( activated() ),
             this, SLOT( openWebInspector() ) );
    
    QShortcut* showas = new QShortcut( Qt::Key_F1, this );
    
    connect( showas, SIGNAL( activated() ), 
             this, SLOT( showOnlyAwaitingSupport() ) );
    
    QShortcut* showac = new QShortcut( Qt::Key_F2, this );
    
    connect( showac, SIGNAL( activated() ), 
             this, SLOT( showOnlyAwaitingCustomer() ) );
    
    QShortcut* showot = new QShortcut( Qt::Key_F3, this );
    
    connect( showot, SIGNAL( activated() ), 
             this, SLOT( showOnlyOthers() ) );
    
    QShortcut* showall = new QShortcut( Qt::Key_F4, this );
    
    connect( showall, SIGNAL( activated() ), 
             this, SLOT( showAll() ) );
    
    QShortcut* expcl = new QShortcut( Qt::Key_F5, this );
    
    connect( expcl, SIGNAL( activated() ), 
             this, SLOT( toggleTables() ) );
    
    QShortcut* sorttype = new QShortcut( Qt::Key_F6, this );
   
    connect( sorttype, SIGNAL( activated() ),
             this, SLOT( toggleSortType() ) );
    
    QShortcut* sortorder = new QShortcut( Qt::Key_F7, this );
   
    connect( sortorder, SIGNAL( activated() ),
             this, SLOT( toggleSortOrder() ) );
}

QueueBrowser::~QueueBrowser()
{
    qDebug() << "[QUEUEBROWSER] Destroying";
}

void QueueBrowser::zoomIn()
{
    int i = m_zoomLevels.indexOf(m_currentZoom);
     Q_ASSERT(i >= 0);
     if (i < m_zoomLevels.count() - 1)
          m_currentZoom = m_zoomLevels[i + 1];
   
     this->setZoomFactor(static_cast<qreal>(m_currentZoom)/100.0);
     Settings::setzoomFactor(m_currentZoom);
        
}

void QueueBrowser::zoomOut()
{
    int i = m_zoomLevels.indexOf(m_currentZoom);
     Q_ASSERT(i >= 0);
     if (i > 0)
          m_currentZoom = m_zoomLevels[i - 1];
   
     this->setZoomFactor(static_cast<qreal>(m_currentZoom)/100.0);
     Settings::setzoomFactor(m_currentZoom);
        
}

void QueueBrowser::openWebInspector()
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    
    QWidget* w = new QWidget;
    QGridLayout* l = new QGridLayout;
    w->setLayout( l );
    QWebInspector* i = new QWebInspector( w );
    l->addWidget( i );
    i->setPage( page() );
    w->setWindowTitle( "Webinspector - srbrowser" );
    w->show();
}

void QueueBrowser::showOnlyAwaitingSupport()
{
    Settings::setShowAwaitingCustomer( false );
    Settings::setShowStatusOthers( false );
    Settings::setShowAwaitingSupport( true );
    emit setMenus();
    DataThread::updateQueueBrowser();
}

void QueueBrowser::showOnlyAwaitingCustomer()
{
    Settings::setShowAwaitingCustomer( true );
    Settings::setShowStatusOthers( false );
    Settings::setShowAwaitingSupport( false );
    emit setMenus();
    DataThread::updateQueueBrowser();
}

void QueueBrowser::showOnlyOthers()
{
    Settings::setShowAwaitingCustomer( false );
    Settings::setShowStatusOthers( true );
    Settings::setShowAwaitingSupport( false );
    emit setMenus();
    DataThread::updateQueueBrowser();
}

void QueueBrowser::showAll()
{
    Settings::setShowAwaitingCustomer( true );
    Settings::setShowStatusOthers( true );
    Settings::setShowAwaitingSupport( true );
    emit setMenus();
    DataThread::updateQueueBrowser();
}

void QueueBrowser::urlHovered( const QString& url, const QString& title, const QString& text )
{
    // Copy the last hovered URL to mUrl
    // Necessary to make the mouseButtonEvents work on links
    
    (void)title;
    (void)text;
    
    mUrl = url;
}

void QueueBrowser::update( const QString& html )
{
    //qDebug() << "[QUEUEBROWSER] Updating";

    QPoint pos = page()->currentFrame()->scrollPosition();
    page()->currentFrame()->setHtml( html );
    page()->currentFrame()->setScrollPosition( pos );
    m_currentZoom = Settings::zoomFactor();
    this->setZoomFactor(static_cast<qreal>(m_currentZoom)/100.0);
    
}

void QueueBrowser::filter( const QString& filter )
{
    DataThread::updateQueueBrowser( filter );
}

void QueueBrowser::mousePressEvent( QMouseEvent* event )
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
            toggleInfoHeader( mUrl.toString().remove( "sr://" ) );
        }
    }
        
    if ( ( event->button() == 1 ) && 
         ( mUrl.toString().startsWith(  "arrow://" ) ) )
    {
        toggleSrTable( mUrl.toString().remove( "arrow://" ) );
        toggleInfoHeader( mUrl.toString().remove( "arrow://" ) );
    }
    
    if ( ( event->button() == 1 ) && 
         ( mUrl.toString().startsWith(  "mailto" ) || 
           mUrl.toString().startsWith( "http" ) ) )
    {
        QDesktopServices::openUrl( mUrl );
    }
    
    if ( ( event->button() == 1 ) && 
        ( mUrl.toString().startsWith(  "crsr" ) ) )
    {
        TabWidget::newUnityWithSR( mUrl.toString().remove( "crsr://" ) );
    }
    
    // Right mouse button     
    
    if ( ( event->button() == 2 ) && 
         ( mUrl.toString().startsWith(  "sr://" ) ) )
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
            toggleInfoHeader( mUrl.toString().remove( "sr://" ) );
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
            toggleInfoHeader( mUrl.toString().remove( "sr://" ) );
        }
    }
    
    return QWebView::mousePressEvent( event );
}

void QueueBrowser::contextMenu( QMouseEvent* event, const QString& id )
{
    // Create a custom context menu plus a widget to hold the actual menu
    // This makes it look more like a KMenu (way more beautiful)
    
    QMenu* menu = new QMenu();
            
    QAction* ba = new QAction( menu );
    QAction* oc = new QAction( "Show/Hide SR details", menu );
    QAction* dd = new QAction( "Show detailed description", menu );
    QAction* cb = new QAction( "Copy SR# to clipboard", menu );
    QAction* ou = new QAction( "Open in first Unity browser", menu );
    QAction* on = new QAction( "Open in new Unity browser", menu );
    QWidgetAction* wa = new QWidgetAction( menu );
    
    QFont font = ba->font();
    font.setBold( true );
    
    QToolButton* titleButton = new QToolButton( this );
    
    wa->setDefaultWidget( titleButton );
    wa->setObjectName( "MENUTITLE" );
    
    titleButton->setDefaultAction( ba );
    titleButton->setDown( true );
    titleButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    
    ba->setFont( font );
    ba->setIcon( QIcon( ":/icons/kueue.png" ).pixmap( QSize( 16, 16 ) ) );
    ba->setText( id );
    
    oc->setData( id + "|tog" );
    dd->setData( id + "|dd" );
    cb->setData( id + "|cb" );
    ou->setData( id + "|ou" );
    on->setData( id + "|on" );
    
    oc->setIcon( QIcon( ":/icons/menus/toggle.png" ) );
    dd->setIcon( QIcon( ":/icons/menus/ddesc.png" ) );
    cb->setIcon( QIcon( ":/icons/menus/clipboard.png" ) );
    ou->setIcon( QIcon( ":/icons/menus/siebel.png" ) );
    on->setIcon( QIcon( ":/icons/menus/siebel.png" ) );
    
    menu->addAction( wa );
    menu->addAction( oc );
    menu->addSeparator();
    menu->addAction( dd );
    menu->addSeparator();
    menu->addAction( cb );
    
    if ( Settings::unityEnabled() )
    {
        menu->addSeparator();
        menu->addAction( ou );
        menu->addAction( on );
    }
    
    connect( menu, SIGNAL( triggered( QAction* ) ), 
             this, SLOT( contextMenuItemTriggered( QAction* ) ) );
    
    menu->exec( event->globalPos() );
    
    delete menu;
}

void QueueBrowser::contextMenuEvent(QContextMenuEvent* event)
{
    // Ignore the contextMenuEvent
    // This is necessary to make the right button events work
    
    event->ignore();
}

void QueueBrowser::contextMenuItemTriggered( QAction* a )
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
        toggleInfoHeader( a->data().toString().remove( "|tog" ) );
    }
    else if ( a->data().toString().contains( "|dd" ) )
    {
        Kueue::showDetailedDescription( a->data().toString().remove( "|dd" ), true );
    }
    else if ( a->data().toString().contains( "|ou" ) )
    {
        TabWidget::openInUnity( a->data().toString().remove( "|ou" ) );
    }
    else if ( a->data().toString().contains( "|on" ) )
    {
        TabWidget::newUnityWithSR( a->data().toString().remove( "|on" ) );
    }
}

void QueueBrowser::toggleSrTable( const QString& id )
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
                Database::setQueueDisplay( id + "-block" );
            }
            else if ( list.at(i).attribute( "style" ) == "display:block" ) 
            {
                list.at( i ).setAttribute("style", "display:none" );
                Database::setQueueDisplay( id + "-none" );
            }
        }
    }
}

void QueueBrowser::toggleInfoHeader( const QString& id )
{
    // Toggle the 2nd line from the tableHeaders 
    
    QWebElementCollection divlist = page()->mainFrame()->findAllElements( "div" );
    
    for ( int i = 0; i < divlist.count(); ++i ) 
    {
        if ( divlist.at( i ).attribute( "id" ) == id + "_hinfo" ) 
        { 
            if ( divlist.at( i ).attribute( "style" ) == "display:none" ) 
            {
                divlist.at( i ).setAttribute( "style", "display:block" );
            }
            else if ( divlist.at( i ).attribute( "style" ) == "display:block" ) 
            {
                divlist.at( i ).setAttribute( "style", "display:none" );
            }
        }
    }
}

void QueueBrowser::toggleSortOrder()
{
    if ( Settings::sortAsc() )
    {
        Settings::setSortAsc( false );
    }
    else
    {
        Settings::setSortAsc( true );
    }
    
    DataThread::updateQueueBrowser();
}

void QueueBrowser::toggleSortType()
{
    if ( Settings::sortAge() )
    {
        Settings::setSortAge( false );
    }
    else
    {
        Settings::setSortAge( true );
    }
    
    DataThread::updateQueueBrowser();
}

void QueueBrowser::toggleTables()
{
    if ( mOpen )
    {
        emit closeAll();
        mOpen = false;
    }
    else
    {
        emit expandAll();
        mOpen = true;
    }
}

#include "queuebrowser.moc"
