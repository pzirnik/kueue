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

#include "kueueapp.h"

#include <QtSingleApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE( kueue );

    QtSingleApplication app( argc, argv );

    
    if ( app.isRunning() && !app.arguments().contains( "restart" ) )
    {
        qDebug() << "Kueue is already running.";
        QMessageBox::critical( 0, "Error", "kueue is already running" );
        return 0;
    }

    QApplication::setOrganizationName( "nts" );
    QApplication::setOrganizationDomain( "suse.com" );
    QApplication::setApplicationName( "kueue" );
    QApplication::setApplicationVersion( "5.6.4" );
    QApplication::setQuitOnLastWindowClosed( false );

    KueueApp kueueapp;
    
    return app.exec();
}
