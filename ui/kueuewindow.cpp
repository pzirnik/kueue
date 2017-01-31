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

#include "ui/html.h"
#include "settings.h"
#include "data/database.h"
#include "ui/kueuewindow.h"
#include "ui/detailwindow.h"
#include "ui_kueuewindow.h"
#include "ui/statusbar.h"
#include "ui/configdialog.h"
#include "kueue.h"

#include <QShortcut>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProgressBar>
#include <QWidgetAction>
#include <QToolButton>

KueueWindow* KueueWindow::instance = 0;

KueueWindow& KueueWindow::win()
{
    if ( !instance )
    {
        instance = new KueueWindow;
    }
 
    return *instance; 
}

void KueueWindow::destroy()
{
    if ( instance )
    {
        delete instance;
    }
      
    instance = 0;
}

KueueWindow::KueueWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::KueueWindow )
{
    qDebug() << "[KUEUEWINDOW] Constructing";
        
    ui->setupUi( this );
  
    #ifdef USE_PHONON
    
        mMusic = new Phonon::MediaObject( this );
        mMusic = Phonon::createPlayer( Phonon::MusicCategory );
    
    #endif
       
    installEventFilter( this );
}
    
KueueWindow::~KueueWindow()
{
    qDebug() << "[KUEUEWINDOW] Destroying";
    
    Settings::setMainWinPos( pos() );
    Settings::setMainWinSize( size() );
    Settings::setMainWinState( saveState() );
}

void KueueWindow::showWindow()
{
    resize( Settings::mainWinSize() );
    move( Settings::mainWinPos() );
    restoreState( Settings::mainWinState() );
    show();
}

void KueueWindow::hideWindow()
{
    Settings::setMainWinPos( pos() );
    Settings::setMainWinSize( size() );
    Settings::setMainWinState( saveState() );
    hide();
}

void KueueWindow::toggleWindow()
{
    if ( isHidden() )
    {
        showWindow();
    }
    else 
    {
        hideWindow();
    }
}

void KueueWindow::closeEvent( QCloseEvent* e )
{
    if ( e->spontaneous() && Settings::showSystemTray())
    {
        e->ignore();
        hideWindow();
    }
    else
    {
        qApp->quit();
    }
}

bool KueueWindow::eventFilter( QObject* o, QEvent* e )
{
    if ( e->type() == 24 )
    {
        Kueue::attention( false );
        StatusBar::hideDownloadManager();
        return true;
    }
    else if ( e->type() == 13 )
    {
        // move
        StatusBar::hideDownloadManager();
    }
    else if ( e->type() == 99 )
    {
        StatusBar::hideDownloadManager();
    }

    return QObject::event( e );
}

void KueueWindow::playSound( const QString& file )
{
    #ifdef USE_PHONON
    
        mMusic->setCurrentSource( file );
        mMusic->play();
    
    #else
    
        qDebug() << "[KUEUE] Built without phonon, no sound available.";
    
    #endif
}

void KueueWindow::showDetailedDescriptionNB( QString id )
{
    DetailWindow* det = new DetailWindow( id, true );
   
    connect( det, SIGNAL( done( DetailWindow* ) ), 
             this, SLOT( detailWindowClosed( DetailWindow* ) ) ); 
    
    det->setWindowTitle( "Details for SR#" + id );
    det->show();
}

void KueueWindow::showDetailedDescription( QString id, bool b )
{   
    DetailWindow* det = new DetailWindow( id, b );
    
    connect( det, SIGNAL( done( DetailWindow* ) ), 
             this, SLOT( detailWindowClosed( DetailWindow* ) ) ); 
    
    det->setWindowTitle( "Details for SR#" + id );
    det->show();
}

void KueueWindow::detailWindowClosed( DetailWindow* det )
{
    det->close();
    delete det;
}

#include "kueuewindow.moc"
