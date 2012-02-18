/*
                kueue - keep track of your SR queue
             (C) 2011 Stefan Bogner <sbogner@suse.com>

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

#include "network.h"
#include "settings.h"
#include "kueue.h"

#include <QtGui>
#include <QDebug>

Network* Network::instance = 0;

Network& Network::net()
{
    if ( !instance )
    {
        instance = new Network;
    }
 
    return *instance; 
}

void Network::destroy()
{
    if ( instance )
    {
        delete instance;
    }
      
    instance = 0;
}

Network::Network()
{
    qDebug() << "[NETWORK] Constructing";   
    
    mNAM = new QNetworkAccessManager( this );
}

Network::~Network()
{
    qDebug() << "[NETWORK] Destroying";
}

QNetworkReply* Network::getImpl( const QUrl& url )
{
    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", QString( "kueue " + QApplication::applicationVersion() + " - " + Settings::engineer() ).toUtf8() );
    
    QNetworkReply* reply = mNAM->get( request );
    
    connect( reply, SIGNAL( error( QNetworkReply::NetworkError ) ),
             this, SLOT( error( QNetworkReply::NetworkError ) ) );

    return reply;
}

void Network::error( QNetworkReply::NetworkError error )
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>( QObject::sender() );
    
    qDebug() << "[NETWORK] Error getting" << reply->url();
}

#include "network.moc"
