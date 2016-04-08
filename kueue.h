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

#ifndef KUEUE_NS_H
#define KUEUE_NS_H

#include "ui/kueuewindow.h"
#include "ui/tabwidget.h"
#include "network.h"

#include <QClipboard>
#include <QApplication>
#include <QRegExp>
#include <sys/stat.h>

namespace Kueue 
{
    static void notify( const QString& type, QString title, QString body, const QString& sr )
    {
        Systray::tray().notify( type, title, body, sr );
    }
    
    static void playSound( const QString& file )
    {
        KueueWindow::win().playSound( file );
    }
    
    static void showDetailedDescription( const QString& id = "", bool b = false )
    {
        KueueWindow::win().showDetailedDescription( id, b );
    }
    
    static void showDetailedDescriptionNB( const QString& id = "" )
    {
        KueueWindow::win().showDetailedDescriptionNB( id );
    }

    static void toggleWindow()
    {
        KueueWindow::win().toggleWindow();
    }
    
    static QSize getWindowSize()
    {
        return KueueWindow::win().windowSize();
    }
    
    static QPoint getWindowPos()
    {
        return KueueWindow::win().windowPos();
    }
    
    static void attention( bool a )
    {
        Systray::tray().requiresAttention( a );
    }
    
    static QString getClipboard()
    {
        QString content;
        
        content = QApplication::clipboard()->text( QClipboard::Selection );
        
        if ( content.isEmpty() )
        {
            content = QApplication::clipboard()->text();
        }
        
        return content;
    }
    
    static void setClipboard( const QString& cb )
    {
        QApplication::clipboard()->setText( cb );
        QApplication::clipboard()->setText( cb, QClipboard::Selection );
    }
    
    static bool isSrNr( const QString& data )
    {
        QRegExp srnr( "^[0-9]{11,12}$" );
    
        if ( srnr.exactMatch( data.trimmed() ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }


}

#endif 
