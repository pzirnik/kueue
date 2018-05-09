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

#ifndef _HTML_H_
#define _HTML_H_

#include "nsa/nsajob.h"
#include "data/dataclasses.h"

class SR;
class NSATableItem;

class HTML
{
    public:
        static QString styleSheet();
        static QString pageHeader( const QString&, int );
        static QString subPageHeader( int );
        static QString SRTable( QueueSR );
        static QString pageFooter();
        static QString qmonPageHeader();
        static QString qmonTableHeader( const QString&, int );
        static QString qmonSrInQueue( QmonSR );
        static QString qmonTableFooter();
        static QString csatTable( Survey );
        static QString csatTableHeader( int, int, int, int );
        static QString closedTableHeader( int, int, int );
        static QString closedTable( ClosedItem );
        static QString statsPageHeader( Statz );
        
        static QString nsaPageHeader( NSASummaryItem );
        static QString nsaTableStart( const QString& );
        static QString nsaTableItem( NSATableItem );
        static QString nsaTableEnd();
        static QString nsaPageEnd();
        
    private:
        static QString timeString( int );
        static int secDays( int );
        static QString BgColor(const QString color);
    
};


#endif
