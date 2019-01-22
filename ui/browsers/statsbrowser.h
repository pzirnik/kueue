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

#ifndef STATSBROWSER_H
#define STATSBROWSER_H

#include <QWebView>
#include "browser.h"

class StatsBrowser : public Browser
{
    Q_OBJECT
     
    public:
        explicit StatsBrowser( QObject* parent = 0L );
        ~StatsBrowser();
        
    private:
        QUrl mUrl;
        int m_currentZoom;
        QVector<int> m_zoomLevels;
        
    public slots:
        void update( const QString& );
        void filter( const QString& );
         
    private slots:
        void zoomIn();
        void zoomOut();
        void openWebInspector();
        void mousePressEvent( QMouseEvent* event );
        void urlHovered( const QString&, const QString&, const QString& );
        void contextMenu( QMouseEvent*, const QString& );
        void contextMenuItemTriggered( QAction* );
        void toggleSrTable( const QString& );
        void contextMenuEvent( QContextMenuEvent* event );
};
 
#endif
