/*
              kueued - create xml data for kueue's qmon 
              (C) 2012 Stefan Bogner <sbogner@suse.com>

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

#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>

#include "data.h"

class DataThread : public QThread
{
    Q_OBJECT
    
    public:
        static DataThread& thread();
        static DataThread& restart();

        void run();
        
        static void updateQueueBrowser( const QString& filter = QString::Null() )
        {
            DataThread::thread().updateQueueBrowserSlot( filter );
        }

        static void updateSubownerBrowser( const QString& filter = QString::Null() )
        {
            DataThread::thread().updateSubownerBrowserSlot( filter );
        }
        
        static void updateQmonBrowser( const QString& filter = QString::Null() )
        {
            DataThread::thread().updateQmonBrowserSlot( filter );
        }
        
        static void updateQmon()
        {
            DataThread::thread().updateQmonSlot();
        }
        
        static void updateStatsBrowser()
        {
            DataThread::thread().updateStatsBrowserSlot();
        }
        
    private:
        static QPointer<DataThread> instance;
        DataThread( QObject *parent = 0L );
        ~DataThread();
        QPointer<Data> mData;
        
    private slots:
        void createData();
        void deleteData();
        
    public slots:
        void updateQueueBrowserSlot( const QString& filter );
        void updateSubownerBrowserSlot( const QString& filter );
        void updateQmonBrowserSlot( const QString& filter );
        void updateQmonSlot();
        void updateStatsBrowserSlot();
        
    signals:
        void notify( const QString&, QString, QString, const QString& );
        void dirsToDelete( QStringList );
        void updateQueueBrowserRequested( QString );
        void updateSubownerBrowserRequested( QString );
        void updateQmonBrowserRequested( QString );
        void updateQmonRequested();
        void showSubownerBrowser( bool );
        void updateStatsBrowserRequested();
        void queueDataChanged( QString );
        void subownerDataChanged( QString );
        void qmonDataChanged( QString );
        void statsDataChanged( QString );
        void qmonUpdateDone( const QString& );
        void statsUpdateDone( const QString& );
        void netError();
        void killed();
};

#endif
