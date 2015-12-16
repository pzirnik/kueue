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

#include "data.h"
#include "data/database.h"
#include "settings.h"
#include "ui/html.h"
#include "kueue.h"

#include <QtXml>
#include <QDesktopServices>
#include <QHostInfo>

Data::Data()
{
    qDebug() << "[DATA] Constructing, threadID" << thread()->currentThreadId();
    
    mQueueUpdateRunning = false;
    mNAM = new QNetworkAccessManager( this );
 
    /*QHostInfo info = QHostInfo::fromName( Settings::dBServer() );
    QList<QHostAddress> al = info.addresses();
    
    for ( int i = 0; i < al.size(); ++i ) 
    { 
        mIPs.append( al.at( i ).toString() );
    }
    
    if ( mIPs.isEmpty() )
    {
        emit netError();
    }*/
    
    mDB = "db-thread";

    QDir dir = QDir( QDesktopServices::storageLocation( QDesktopServices::DataLocation ) );
    
    Database::openDbConnection( mDB );
    
    QTimer* queueTimer = new QTimer( this );
    queueTimer->start( 94726 );
    
    connect( queueTimer, SIGNAL( timeout() ),
             this, SLOT( updateQueue() ) );
    
    updateQueue();

    if ( Settings::monitorEnabled() )
    {
        Database::dropQmon();
        
        QTimer* qmonTimer = new QTimer( this );
        qmonTimer->start( 24219 );
    
        connect( qmonTimer, SIGNAL( timeout() ),
                 this, SLOT( updateQmon() ) );
        
        updateQmon();
    }
    
    if ( Settings::statsEnabled() )
    {
        QTimer* statsTimer = new QTimer( this );
        statsTimer->start( 1800000 );
    
        connect( statsTimer, SIGNAL( timeout() ),
                 this, SLOT( updateStats() ) );
        
        updateStats();
    }
}

Data::~Data()
{
    qDebug() << "[DATA] Destroying";
    QSqlDatabase::database( mDB ).close();
    QSqlDatabase::removeDatabase( mDB );
}

QNetworkReply* Data::get( const QString& u )
{
    //int r = qrand() % mIPs.size();
    //QNetworkRequest request( QUrl( "http://" + mIPs.at(r) + ":8080/" + u ) );
    QNetworkRequest request( QUrl( "http://" + Settings::dBServer() + ":8080/" + u ) );
    
    QByteArray os;
    
#ifdef IS_WIN32
    os = "kueue " + QApplication::applicationVersion().toUtf8() + " (win)";
#elif defined IS_OSX
    os = "kueue " + QApplication::applicationVersion().toUtf8() + " (osx)";
#else
    os = "kueue " + QApplication::applicationVersion().toUtf8() + " (linux)";
#endif
    
    request.setRawHeader( "User-Agent", os );
    
    QNetworkReply* reply = mNAM->get( request );
    
    connect( reply, SIGNAL( error( QNetworkReply::NetworkError ) ),
             this, SLOT( getError( QNetworkReply::NetworkError ) ) );
    
    return reply;
}

void Data::getError( QNetworkReply::NetworkError error )
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>( QObject::sender() );
    
    reply->abort();
    reply->close();
    
    //Kueue::notify( "kueue-general", "Update failed", "Failed to update your data. Networking issues or no VPN connection?", "NONE" );
    
    qDebug() << "[DATA] Error getting" << reply->url();
    
    mQueueUpdateRunning = false;
    
    reply->deleteLater();
}

void Data::updateQueue()
{
    if ( !mQueueUpdateRunning )
    { 
        QNetworkReply* r = get( "userqueue/full/" + Settings::engineer() );
    
        connect( r, SIGNAL( finished() ), 
                this, SLOT( queueUpdateFinished() ) );
    }
}

void Data::updateQmon()
{
    QNetworkReply* r = get( "qmon" );
    
    connect( r, SIGNAL( finished() ), 
             this, SLOT( qmonUpdateFinished() ) );
}

void Data::updateStats()
{
    QNetworkReply* r = get( "stats/" + Settings::engineer() );
    
    connect( r, SIGNAL( finished() ), 
             this, SLOT( statsUpdateFinished() ) );
}

void Data::queueUpdateFinished()
{
    QNetworkReply* r = qobject_cast<QNetworkReply*>( sender() );
    QString xml = QString::fromUtf8( r->readAll() );
    
    if ( r->error() )
    {
        qDebug() << "[DATA] Error downloading queue data" << r->errorString();
    }
    else if ( !xml.endsWith( "</queue>\n" ) )
    {
        qDebug() << "[DATA] Received incomplete queue xml";
    }
    else
    {
        QDomDocument doc;
        doc.setContent( xml );
        QStringList goneList = Database::getGoneSRs( mDB );
        QDomNodeList list = doc.elementsByTagName( "sr" );
        
        PersonalQueue q;
        
        for ( int i = 0; i < list.size(); ++i ) 
        {   
            QueueSR sr;
            
            sr.id = list.at( i ).namedItem( "id" ).toElement().text(); 
            sr.srtype = list.at( i ).namedItem( "srtype" ).toElement().text(); 
            sr.creator = list.at( i ).namedItem( "creator" ).toElement().text(); 
            sr.cus_account = list.at( i ).namedItem( "cus_account" ).toElement().text(); 
            sr.cus_firstname = list.at( i ).namedItem( "cus_firstname" ).toElement().text(); 
            sr.cus_lastname = list.at( i ).namedItem( "cus_lastname" ).toElement().text(); 
            sr.cus_title = list.at( i ).namedItem( "cus_title" ).toElement().text(); 
            sr.cus_email = list.at( i ).namedItem( "cus_email" ).toElement().text(); 
            sr.cus_phone = list.at( i ).namedItem( "cus_phone" ).toElement().text(); 
            sr.cus_onsitephone = list.at( i ).namedItem( "cus_onsitephone" ).toElement().text(); 
            sr.cus_lang = list.at( i ).namedItem( "cus_lang" ).toElement().text(); 
            sr.alt_contact = list.at( i ).namedItem( "alt_contact" ).toElement().text();
            sr.bug = list.at( i ).namedItem( "bug" ).toElement().text(); 
            sr.bugtitle = list.at( i ).namedItem( "bug_desc" ).toElement().text(); 
            sr.severity = list.at( i ).namedItem( "severity" ).toElement().text(); 
            sr.status = list.at( i ).namedItem( "status" ).toElement().text(); 
            sr.bdesc = list.at( i ).namedItem( "bdesc" ).toElement().text().replace( "]]&gt;", "]]>" ); 
            sr.ddesc = list.at( i ).namedItem( "ddesc" ).toElement().text().replace( "]]&gt;", "]]>" );; 
            sr.geo = list.at( i ).namedItem( "geo" ).toElement().text(); 
            sr.hours = list.at( i ).namedItem( "hours" ).toElement().text(); 
            sr.contract = list.at( i ).namedItem( "contract" ).toElement().text(); 
            sr.created = list.at( i ).namedItem( "created" ).toElement().text(); 
            sr.lastupdate = list.at( i ).namedItem( "lastupdate" ).toElement().text(); 
            sr.service_level = list.at( i ).namedItem( "service_level" ).toElement().text().toInt();
            sr.highvalue = list.at( i ).namedItem( "highvalue" ).toElement().text().toInt(); 
            sr.critsit = list.at( i ).namedItem( "critsit" ).toElement().text().toInt();
            sr.owner = list.at( i ).namedItem( "owner" ).toElement().text();
            sr.subowner = list.at( i ).namedItem( "subowner" ).toElement().text();
            sr.crsr = list.at( i ).namedItem( "crsr" ).toElement().text();
            
            if ( sr.subowner.isEmpty() )
            {
                sr.subowned = false;
            }
            else
            {
                sr.subowned = true;
            }
            
            q.srList.append( sr );
            
            if ( goneList.contains( sr.id ) )
            {
                Database::delGoneSR( sr.id, mDB );
            }
        }
        
        QList<Notification> nl = Database::updateQueue( q, mDB );   
        
        for ( int i = 0; i < nl.size(); ++i ) 
        {   
            emit notify( nl.at( i ).type, nl.at( i ).title, nl.at( i ).body, nl.at( i ).sr );
        }
        
        if ( Settings::cleanupDownloadDirectory() )
        {
            QDir dlDir = Settings::downloadDirectory();
            dlDir.setFilter( QDir::Dirs );
            
            QStringList subDirs = dlDir.entryList();
            QStringList existList;
            QStringList currentList;
            QStringList deleteList;
            
            for ( int i = 0; i < subDirs.size(); ++i ) 
            {
                QString dir = subDirs.at( i );
                
                if ( ( dir != "." ) &&
                     ( dir != ".." ) &&
                     ( Kueue::isSrNr( dir ) ) )
                    {
                        existList.append( subDirs.at( i ) );
                    }
            }
            
            for ( int i = 0; i < q.srList.size(); ++i ) 
            {
                currentList.append( q.srList.at( i ).id );
                
                if ( !q.srList.at( i ).crsr.isEmpty() )
                {
                    currentList.append( q.srList.at( i ).crsr );
                }
            }
            
            for ( int i = 0; i < existList.size(); ++i ) 
            {
                mQueueUpdateRunning = true;
                
                QString sr = existList.at( i );
                
                if ( !currentList.contains( sr ) )
                {
                    if ( !goneList.contains( sr ) )
                    {
                        if ( srIsClosed( sr ) )
                        {
                            Database::addGoneSR( sr, mDB );
                        }
                        else
                        {
                            Database::addGoneSR( sr, mDB );
                        }
                    }
                    else
                    {
                        if ( Database::getGoneDays( sr, mDB ) > 3 )
                        {
                            Database::delGoneSR( sr, mDB );
                            deleteList.append( Settings::downloadDirectory() + "/" + sr );
                        }
                    }
                }
                
                mQueueUpdateRunning = false;
            }
                            
            if ( deleteList.size() > 0 )
            {
                emit dirsToDelete( deleteList );
            }
        }
        
        updateQueueBrowser( mCurrentQueueFilter );
        updateSubownerBrowser( mCurrentSubownerFilter );
    }

    r->deleteLater();
}

void Data::qmonUpdateFinished()
{
    QNetworkReply* r = qobject_cast<QNetworkReply*>( sender() );
    QString xml = QString::fromUtf8( r->readAll() );
    
    if ( r->error() )
    {
        qDebug() << "[DATA] Error downloading qmon data" << r->errorString();
    }
    else if ( !xml.endsWith( "</qmon>\n" ) )
    {
        qDebug() << "[DATA] Received incomplete qmon xml";
    }
    else
    {
        QmonData q;
        
        QDomDocument doc;
        doc.setContent( xml );
        QDomNodeList list = doc.elementsByTagName( "sr" );
        
        for ( int i = 0; i < list.size(); ++i ) 
        {   
            QmonSR sr;
            
            sr.id = list.at( i ).namedItem( "id" ).toElement().text(); 
            sr.queue = list.at( i ).namedItem( "queue" ).toElement().text(); 
            sr.bomgarQ = list.at( i ).namedItem( "bomgarQ" ).toElement().text(); 
            sr.srtype = list.at( i ).namedItem( "srtype" ).toElement().text(); 
            sr.creator = list.at( i ).namedItem( "creator" ).toElement().text(); 
            sr.cus_account = list.at( i ).namedItem( "cus_account" ).toElement().text(); 
            sr.cus_firstname = list.at( i ).namedItem( "cus_firstname" ).toElement().text(); 
            sr.cus_lastname = list.at( i ).namedItem( "cus_lastname" ).toElement().text(); 
            sr.cus_title = list.at( i ).namedItem( "cus_title" ).toElement().text(); 
            sr.cus_email = list.at( i ).namedItem( "cus_email" ).toElement().text(); 
            sr.cus_phone = list.at( i ).namedItem( "cus_phone" ).toElement().text(); 
            sr.cus_onsitephone = list.at( i ).namedItem( "cus_onsitephone" ).toElement().text(); 
            sr.cus_lang = list.at( i ).namedItem( "cus_lang" ).toElement().text(); 
            sr.alt_contact = list.at( i ).namedItem( "alt_contact" ).toElement().text();
            sr.bug = list.at( i ).namedItem( "bug" ).toElement().text(); 
            sr.severity = list.at( i ).namedItem( "severity" ).toElement().text(); 
            sr.status = list.at( i ).namedItem( "status" ).toElement().text(); 
            sr.bdesc = list.at( i ).namedItem( "bdesc" ).toElement().text().replace( "]]&gt;", "]]>" );; 
            sr.ddesc = list.at( i ).namedItem( "ddesc" ).toElement().text().replace( "]]&gt;", "]]>" );; 
            sr.geo = list.at( i ).namedItem( "geo" ).toElement().text(); 
            sr.hours = list.at( i ).namedItem( "hours" ).toElement().text(); 
            sr.source = list.at( i ).namedItem( "source" ).toElement().text(); 
            sr.support_program = list.at( i ).namedItem( "support_program" ).toElement().text(); 
            sr.support_program_long = list.at( i ).namedItem( "support_program_long" ).toElement().text(); 
            sr.routing_product = list.at( i ).namedItem( "routing_product" ).toElement().text(); 
            sr.support_group_routing = list.at( i ).namedItem( "support_group_routing" ).toElement().text(); 
            sr.int_type = list.at( i ).namedItem( "int_type" ).toElement().text(); 
            sr.subtype = list.at( i ).namedItem( "subtype" ).toElement().text(); 
            sr.service_level = list.at( i ).namedItem( "service_level" ).toElement().text().toInt(); 
            sr.category = list.at( i ).namedItem( "category" ).toElement().text(); 
            sr.respond_via = list.at( i ).namedItem( "respond_via" ).toElement().text(); 
            sr.agesec = list.at( i ).namedItem( "age" ).toElement().text().toInt(); 
            sr.lastupdatesec = list.at( i ).namedItem( "lastupdate" ).toElement().text().toInt(); 
            sr.timeinqsec = list.at( i ).namedItem( "timeinQ" ).toElement().text().toInt(); 
            sr.slasec = list.at( i ).namedItem( "sla" ).toElement().text().toInt(); 
            sr.highvalue = list.at( i ).namedItem( "highvalue" ).toElement().text().toInt();
            sr.critsit = list.at( i ).namedItem( "critsit" ).toElement().text().toInt();
            sr.lupdate = list.at( i ).namedItem( "lastupdatedate" ).toElement().text();
            sr.cdate = list.at( i ).namedItem( "agedate" ).toElement().text();
            sr.crsr = list.at( i ).namedItem( "crsr" ).toElement().text();
            sr.hasLTSS = list.at( i ).namedItem( "ltss" ).toElement().text().toInt();
            sr.subowner = list.at( i ).namedItem( "subowner" ).toElement().text();
            
            if ( sr.creator.isEmpty() )
            {
                sr.isCr = false;
            }
            else
            {
                sr.isCr = true;
            }
            
            q.srList.append( sr );
        }
            
        q.total = q.srList.size();
        QList<Notification> nl = Database::updateQmon( q, mDB );
        
        for ( int i = 0; i < nl.size(); ++i ) 
        {   
            emit notify( nl.at( i ).type, nl.at( i ).title, nl.at( i ).body, nl.at( i ).sr );
        }
 
        updateQmonBrowser( mCurrentQmonFilter );
    }
    
    r->deleteLater();
}

void Data::statsUpdateFinished()
{
    QNetworkReply* r = qobject_cast<QNetworkReply*>( sender() );
    QString xml = QString::fromUtf8( r->readAll() );
    
    if ( r->error() )
    {
        qDebug() << "[DATA] Error downloading stats" << r->errorString();
    }
    else if ( !xml.endsWith( "</stats>\n" ) )
    {
        qDebug() << "[DATA] Received incomplete stats xml";
    }
    else
    {
        Statz statz;
        
        QDomDocument doc;
        doc.setContent( xml );
        
        QDomElement root = doc.documentElement();
        
        QList<Survey> surveyItemList;
        QList<ClosedItem> closedItemList;
        
        QDomNode n = root.firstChild();
        
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
    
            if( !e.isNull() )
            {     
                if( e.tagName() == "closed" )
                {
                    QDomNodeList cList = e.childNodes();

                    for ( int i = 0; i < cList.size(); ++i ) 
                    {   
                        ClosedItem ci;
                        
                        ci.id = cList.at( i ).namedItem( "sr" ).toElement().text();
                        ci.customer =  cList.at( i ).namedItem( "customer" ).toElement().text();
                        ci.bdesc = cList.at( i ).namedItem( "bdesc" ).toElement().text();
                        ci.tts = cList.at( i ).namedItem( "tts" ).toElement().text().toInt();
                        
                        statz.closedList.append( ci );
                    }
                }
                else if ( e.tagName() == "csat" )
                {
                    QDomNodeList csatList = e.childNodes();

                    for ( int i = 0; i < csatList.size(); ++i ) 
                    {   
                        Survey s;
                        bool ok;
                        s.id = csatList.at( i ).namedItem( "sr" ).toElement().text();
                        s.customer = csatList.at( i ).namedItem( "customer" ).toElement().text();
                        s.bdesc = csatList.at( i ).namedItem( "bdesc" ).toElement().text();
                        s.rts = csatList.at( i ).namedItem( "rts" ).toElement().text().toInt();
                        s.engsat = csatList.at( i ).namedItem( "engsat" ).toElement().text().toInt(&ok);
                        if ( !ok )
                        {
                            s.engsat = -1;
                        }
                        s.srsat = csatList.at( i ).namedItem( "srsat" ).toElement().text().toInt(&ok);
                        if ( !ok )
                        {
                            s.srsat = -1;
                        }
                        statz.surveyList.append( s );
                    }
                }
                else if ( e.tagName() == "closeddata" )
                {
                    QDomNode closednode = e.firstChild();
                    
                    while ( !closednode.isNull() )
                    {
                        QDomElement closedelement = closednode.toElement();
                        
                        if ( !closedelement.isNull() )
                        {
                            if ( closedelement.tagName() == "srs" )
                            {
                                statz.closedSr = closedelement.text().toInt();
                            }
                            else if ( closedelement.tagName() == "crs" )
                            {
                                statz.closedCr = closedelement.text().toInt();
                            }
                            else if ( closedelement.tagName() == "srttsavg" )
                            {
                                statz.srTtsAvg = closedelement.text().toInt();
                            }
                        }
                        
                        closednode = closednode.nextSibling();
                    }
                }
                else if ( e.tagName() == "csatdata" )
                {
                    QDomNode csatnode = e.firstChild();
                    
                    while ( !csatnode.isNull() )
                    {
                        QDomElement csatelement = csatnode.toElement();
                        
                        if ( !csatelement.isNull() )
                        {
                            if ( csatelement.tagName() == "engavg" )
                            {
                                statz.csatEngAvg = csatelement.text().toInt();
                            }
                            else if ( csatelement.tagName() == "sravg" )
                            {
                                statz.csatSrAvg = csatelement.text().toInt();
                            }
                            else if ( csatelement.tagName() == "rtsavg" )
                            {
                                statz.csatRtsPercent = csatelement.text().toInt();
                            }
                        }
                        
                        csatnode = csatnode.nextSibling();
                    }
                }
            }
            
            n = n.nextSibling();
        }
        
        Database::updateStats( statz, mDB );
        updateStatsBrowser();
    }
    
    r->deleteLater();
}

void Data::updateQueueBrowser( const QString& filter )
{
    mCurrentQueueFilter = filter;
    
    QString html;
    QString subhtml;
    int age = 0;
    QList<QueueSR> srlist = Database::getSrList( Settings::sortAge(), Settings::sortAsc(), mDB, mCurrentQueueFilter );
        
    html += HTML::styleSheet();
    html += HTML::pageHeader( Settings::engineer(), srlist.size() );

    for ( int i = 0; i < srlist.size(); ++i )
    {
        QueueSR sr = srlist.at( i );
        
        if ( !Settings::showAwaitingCustomer() && 
              sr.status == "Awaiting Customer" )
        {
                //qDebug() << "[QUEUEBROWSER] Skipping" << sr.status << sr.id;
        }
        else if ( !Settings::showAwaitingSupport() && 
                   ( sr.status == "Awaiting Technical Support" ||
                   sr.status == "Awaiting Novell Support" ) )
        {
                //qDebug() << "[QUEUEBROWSER] Skipping 2" << sr.status << sr.id;
        }
        else if ( !Settings::showStatusOthers() &&
                  ( sr.status == "Suspended" ||
                    sr.status == "Unassigned" ||
                    sr.status == "Assigned" ||
                    sr.status == "Escalate" || 
                    sr.status == "Monitor Solution" ||
                    sr.status == "Schedule For Close" ||
                    sr.status == "Awaiting Engineering" ||
                    sr.status == "Awaiting Novell Engineering" ||
                    sr.status == "Awaiting Third Party" ||
                    sr.status == "Awaiting Public Patch Release" ) )
        {
            //qDebug() << "[QUEUEBROWSER] Skipping 3" << sr.status << sr.id;
        }
        else if ( !Settings::showCR() &&
          ( sr.srtype == "cr" ) )
        {
          //qDebug() << "[QUEUEBROWSER] Skipping 4" << sr.srtype;
        }
        else if ( !Settings::showSR() &&
          ( sr.srtype == "sr" ) )
        {
          //qDebug() << "[QUEUEBROWSER] Skipping 5" << sr.srtype;
        }
        else
        {
            html += HTML::SRTable( srlist.at( i ) );
        }
    }

    if ( srlist.size() > 0 )
    {
        int avgAge = age / srlist.size();
    }
    
    if ( !html.isEmpty() )
    {
        emit queueDataChanged( html );
    }
}

void Data::updateSubownerBrowser( const QString& filter )
{
    mCurrentSubownerFilter = filter;
    QString html;
    int age = 0;
    QList<QueueSR> srlist = Database::getSubSrList( Settings::subSortAge(), Settings::subSortAsc(), mDB, mCurrentSubownerFilter );
   
    if ( srlist.size() == 0 && filter.isEmpty() )
    {
        emit showSubownerBrowser( false );
    }
    else
    {
        emit showSubownerBrowser( true );
  
        html += HTML::styleSheet();
        html += HTML::subPageHeader( srlist.size() );
    
        for ( int i = 0; i < srlist.size(); ++i )
        {
            QueueSR sr = srlist.at( i );
            
            if ( !Settings::subShowAwaitingCustomer() && 
                sr.status == "Awaiting Customer" )
            {
                qDebug() << "[QUEUEBROWSER] Skipping" << sr.status << sr.id;
            }
            else if ( !Settings::subShowAwaitingSupport() && 
                ( sr.status == "Awaiting Technical Support" ||
                sr.status == "Awaiting Novell Support" ) )
            {
                qDebug() << "[QUEUEBROWSER] Skipping 2" << sr.status << sr.id;
            }
            else if ( !Settings::subShowStatusOthers() &&
                ( sr.status == "Suspended" ||
                sr.status == "Unassigned" ||
                sr.status == "Assigned" ||
                sr.status == "Escalate" || 
                sr.status == "Monitor Solution" ||
                sr.status == "Schedule For Close" ||
                sr.status == "Awaiting Engineering" ||
                sr.status == "Awaiting Novell Engineering" ||
                sr.status == "Awaiting Third Party" ||
                sr.status == "Awaiting Public Patch Release" ) )
            {
                qDebug() << "[QUEUEBROWSER] Skipping 3" << sr.status << sr.id;
            }
            else if ( !Settings::subShowCR() &&
              ( sr.srtype == "cr" ) )
            {
              //qDebug() << "[QUEUEBROWSER] Skipping 4" << sr.srtype;
            }
            else if ( !Settings::subShowCR() &&
              ( sr.srtype == "sr" ) )
            {
              //qDebug() << "[QUEUEBROWSER] Skipping 5" << sr.srtype;
            } 
            else
            {
                html += HTML::SRTable( sr );
            }
        }
        
        if ( !html.isEmpty() )
        {
            emit subownerDataChanged( html );
        }
    }
}

void Data::updateQmonBrowser( const QString& filter )
{
    mCurrentQmonFilter = filter;
    
    QString html;
    int age = 0;
    
    QStringList list = Settings::queuesToMonitor();
    
    html += HTML::styleSheet();
    html += HTML::qmonPageHeader();

    for ( int i = 0; i < list.size(); ++i )
    {
        QList< QmonSR > l( Database::getQmonQueue( list.at( i ).split( "|" ).at( 1 ), Settings::qmonFilter(), mDB, mCurrentQmonFilter ) );

        if ( l.size() == 0 && !Settings::showEmptyQueues() )
        {
            //qDebug() << "[DATA] Skipping empty queue";
        }
        else
        {
            if ( list.at( i ).split( "|" ).at( 0 ).isEmpty() )
            {
                html += HTML::qmonTableHeader( list.at( i ).split( "|" ).at( 1 ), l.size() );
            }
            else
            {
                html += HTML::qmonTableHeader( list.at( i ).split( "|" ).at( 0 ), l.size() );
            }
            
                    for ( int i = 0; i < l.size(); ++i ) 
            {
                html += HTML::qmonSrInQueue( l.at( i ) );
            }
            
            html += HTML::qmonTableFooter();
        }
    }

    if ( !html.isEmpty() )
    {
        emit qmonDataChanged( html );
    }
}

void Data::updateStatsBrowser()
{
    Statz s = Database::getStatz( mDB );
    
    QString html = HTML::statsPageHeader( Database::getStatz( mDB ) );
    
    if ( !html.isEmpty() )
    {
        emit statsDataChanged( html );
    }
}

bool Data::srIsClosed( const QString& sr )
{
    QEventLoop loop;
    QString o;
    QNetworkReply* r;
            
    r = get( "srstatus/" + sr );
    
    QObject::connect( r, SIGNAL( finished() ), 
                      &loop, SLOT( quit() ) );
    
    loop.exec();
    
    o = r->readAll();
    
    if ( !r->error() )
    {
        r->deleteLater();
        
        if ( o == "Open" )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    
    r->deleteLater();
    return false;
}

#include "data.moc"
