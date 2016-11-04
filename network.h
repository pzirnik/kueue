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

#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>

class Network : public QObject
{
    Q_OBJECT
    
    private:
        static Network* instance;
        Network();
        ~Network();
        
        QNetworkAccessManager* mNAM;
        QNetworkReply* getImpl( const QString&, bool auth=false );
        QNetworkReply* getExtImpl( const QUrl& );
        QStringList mIPs;
        
    public:
        static Network& net();
        static void destroy();
        static QNetworkReply* get( const QString& u, bool auth=false )
        {
            return Network::net().getImpl( u,auth );
        }
        static QNetworkReply* getExt( const QUrl& url )
        {
            return Network::net().getExtImpl( url );
        }

    private slots:
        void error( QNetworkReply::NetworkError );
        
    signals:
        void netError();
};

#endif
