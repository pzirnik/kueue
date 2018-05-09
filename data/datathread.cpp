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

#include "datathread.h"
#include "config.h"

QPointer<DataThread> DataThread::instance = 0;

DataThread& DataThread::thread()
{
    if ( !instance )
    {
        instance = new DataThread;
    }
 
    return *instance; 
}

DataThread& DataThread::restart()
{
    if ( instance != 0 )
    {
        instance->deleteData();
        instance->quit();
        instance->wait();
        delete instance;
    }
    
    instance = new DataThread;
    return *instance;
}

DataThread::DataThread( QObject *parent ) : QThread( parent )
{
    start();
}

DataThread::~DataThread()
{
    qDebug() << "[DATATHREAD] Destroying";
}

void DataThread::run()
{
    createData();
    
    emit updateQueueBrowserRequested( QString::Null() );
    emit updateSubownerBrowserRequested( QString::Null() );   
 
    if ( Settings::monitorEnabled() )
    {
        emit updateQmonBrowserRequested( QString::Null() );
    }
    
    if ( Settings::statsEnabled() )
    {
        emit updateStatsBrowserRequested();
    }
    
    exec();
}

void DataThread::createData()
{
    mData = new Data();
    
    connect( mData, SIGNAL( queueDataChanged( const QString& ) ), 
             this, SIGNAL( queueDataChanged( const QString& ) ) );
    
    connect( mData, SIGNAL( subownerDataChanged( const QString& ) ),
             this, SIGNAL( subownerDataChanged( const QString& ) ) );
    
    connect( mData, SIGNAL( dirsToDelete( QStringList ) ),
             this, SIGNAL( dirsToDelete( QStringList ) ) );
    
    connect( this, SIGNAL( updateQueueBrowserRequested( QString ) ), 
             mData, SLOT( updateQueueBrowser( QString ) ) );
    
    connect( this, SIGNAL( updateSubownerBrowserRequested( QString ) ),
             mData, SLOT( updateSubownerBrowser( QString ) ) );

    connect( mData, SIGNAL( statsDataChanged( QString ) ), 
             this, SIGNAL( statsDataChanged( QString ) ) );
    
    connect( this, SIGNAL( updateStatsBrowserRequested() ), 
             mData, SLOT( updateStatsBrowser() ) );
    
    connect( mData, SIGNAL(showSubownerBrowser( bool ) ),
             this, SIGNAL(showSubownerBrowser( bool ) ) );
    
    connect( mData, SIGNAL( qmonDataChanged( QString ) ), 
             this, SIGNAL( qmonDataChanged( QString ) ) );
    
    connect( this, SIGNAL( updateQmonBrowserRequested( QString ) ), 
             mData, SLOT( updateQmonBrowser( QString ) ) );
    
    connect( this, SIGNAL( updateQmonRequested() ), 
             mData, SLOT( updateQmon() ) );
    
    connect( mData, SIGNAL( netError() ), 
             this, SIGNAL( netError() ) );
    
    connect( mData, SIGNAL( notify( QString, QString, QString, QString ) ),
             this, SIGNAL( notify( QString, QString, QString, QString ) ) );
}

void DataThread::deleteData()
{
    // on OSX Lion this will crash for whatever reason, thus
    // we will simply not delete mData here. Maybe I'll find a 
    // proper solution at some point, but for now this is good enough.
    
    #ifndef IS_OSX
    #ifndef IS_WIN32
    delete mData;
    #endif
    #endif
}

void DataThread::updateQueueBrowserSlot( const QString& filter )
{
    emit updateQueueBrowserRequested( filter );
}

void DataThread::updateSubownerBrowserSlot( const QString& filter )
{
    emit updateSubownerBrowserRequested( filter );
}

void DataThread::updateQmonBrowserSlot( const QString& filter )
{
    emit updateQmonBrowserRequested( filter );
}

void DataThread::updateQmonSlot()
{
    emit updateQmonRequested();
}

void DataThread::updateStatsBrowserSlot()
{
    emit updateStatsBrowserRequested();
}

#include "datathread.moc"
