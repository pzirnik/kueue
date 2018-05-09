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

#include "database.h"
#include "ui/html.h"
#include "kueue.h"

#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QDesktopServices>

void Database::openDbConnection( QString dbname )
{
    qDebug() << "[DATABASE] Opening DB connection" << dbname;
    
    QDir dir = QDir( QDesktopServices::storageLocation( QDesktopServices::DataLocation ) );

    if ( !dir.exists() )
    {
        dir.mkpath( dir.path() );
    }
    
    if ( dbname.isNull() )
    {
        dbname = "sqliteDB";
    }
    
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", dbname );
    db.setDatabaseName( dir.path() + "/database.sqlite" );
        
    if ( !db.open() )
    {
        qDebug() << "[DATABASE] Failed to open the database " + dbname ;
    }
    else
    {                         
        qDebug() << "[DATABASE] Opened DB" << dir.path() << dbname;
        
        QSqlQuery query( db );
        
        if ( !query.exec("PRAGMA temp_store = MEMORY") )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec("PRAGMA synchronous = OFF") )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec("PRAGMA journal_mode = MEMORY") )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec("PRAGMA locking_mode = NORMAL") )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS " + Settings::engineer().toUpper() +
                            "( ID INTEGER PRIMARY KEY UNIQUE, SRTYPE TEXT, CREATOR TEXT, CUS_ACCOUNT TEXT, "
                            "  CUS_FIRSTNAME TEXT, CUS_LASTNAME TEXT, CUS_TITLE TEXT, CUS_EMAIL TEXT, CUS_PHONE TEXT, "
                            "  CUS_ONSITEPHONE TEXT, CUS_LANG TEXT, SEVERITY TEXT, STATUS TEXT, BDESC TEXT, DDESC TEXT, "
                            "  GEO TEXT, HOURS TEXT, CONTRACT TEXT, SERVICE_LEVEL INTEGER, CREATED TEXT, LASTUPDATE TEXT, "
                            "  HIGHVALUE INTEGER, CRITSIT INTEGER, DISPLAY TEXT, ALT_CONTACT TEXT, BUG TEXT, BUGTITLE TEXT, "
                            "  OWNER TEXT, SUBOWNER TEXT, CRSR TEXT, CUS_RATING )" ) )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }

        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS STATS "
                            "( CLOSED_SR INTEGER, CLOSED_CR INTEGER, SR_TTS_AVG INTEGER, CSAT_ENG_AVG INTEGER, "
                            "  CSAT_SR_AVG INTEGER, CSAT_RTS_PERCENT INTEGER )" ) )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }

        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS STATS_SURVEYS "
                            "( ID INTEGER PRIMARY KEY UNIQUE, RTS INTEGER, ENGSAT INTEGER, SRSAT INTEGER, "
                            " CUSTOMER TEXT, BDESC TEXT )" ) )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }

        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS STATS_CLOSED "
                            "( ID INTEGER PRIMARY KEY UNIQUE, TTS INTEGER, CUSTOMER TEXT, BDESC TEXT )" ) )
            
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS SR_GONE "
                            "( ID INTEGER PRIMARY KEY UNIQUE, DATE TEXT )" ) )
            
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
        
        if ( !query.exec(   "CREATE TABLE IF NOT EXISTS QMON "
                            "( ID INTEGER PRIMARY KEY UNIQUE, QUEUE TEXT, BOMGARQ TEXT, SRTYPE TEXT, CREATOR TEXT, "
                            "  CUS_ACCOUNT TEXT, CUS_FIRSTNAME TEXT, CUS_LASTNAME TEXT, CUS_TITLE TEXT, CUS_EMAIL TEXT, "
                            "  CUS_PHONE TEXT, CUS_ONSITEPHONE TEXT, CUS_LANG TEXT, SEVERITY TEXT, STATUS TEXT, BDESC TEXT, "
                            "  DDESC TEXT, GEO TEXT, HOURS TEXT, SOURCE TEXT, SUPPORT_PROGRAM TEXT, SUPPORT_PROGRAM_LONG TEXT, "
                            "  ROUTING_PRODUCT TEXT, SUPPORT_GROUP_ROUTING TEXT, INT_TYPE TEXT, SUBTYPE TEXT, "
                            "  SERVICE_LEVEL INTEGER, CATEGORY TEXT, RESPOND_VIA TEXT, AGE TEXT, LASTUPDATE TEXT, "
                            "  TIMEINQ TEXT, SLA TEXT, HIGHVALUE INTEGER, CRITSIT INTEGER, DISPLAY TEXT, ALT_CONTACT TEXT, "
                            "  BUG TEXT, BUGTITLE TEXT, LUPDATE TEXT, CREATEDATE TEXT, CRSR TEXT, LTSS INTEGER, SUBOWNER TEXT, CUS_RATING )" ) )
        {
            qDebug() << "[DATABASE] Error:" << query.lastError();
        }
    }
}


/*


                P E R S O N A L   Q U E U E


*/

QList<Notification> Database::updateQueue( PersonalQueue pq, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );

    QStringList newList;
    QStringList existList = getSrNrList( dbname );
    QList< QueueSR > srList = pq.srList;
    bool initial = existList.isEmpty();
    QList<Notification> nl;
    
    db.transaction();
        
    for ( int i = 0; i < srList.size(); ++i ) 
    {
        QueueSR sr = srList.at( i );
        
        if ( queueSrExists( sr.id, dbname ) )
        {
            if ( ( srWasUpdated( sr, dbname ) &&
                 ( sr.status != "Awaiting Customer" ) ) )
            {
                Notification n;
                n.type = "kueue-sr-update";
                n.title = "SR Updated";
                n.body = "<b>SR#" + sr.id + "</b><br>" + sr.bdesc;
                n.sr = sr.id;
                nl.append( n );
            }
            
            updateQueueSR( sr, dbname );
        }
        else
        {
            insertQueueSR( sr, dbname );
            
            if ( !initial )
            {
                Notification n;
                n.type = "kueue-sr-new";
                n.title = "New SR in your queue";
                n.body = "<b>" + sr.id + "</b><br>" + sr.bdesc;
                n.sr = sr.id;
                nl.append( n );
            }
        }
        
        newList.append( sr.id );
    }
    
    for ( int i = 0; i < existList.size(); ++i ) 
    {   
        if ( !newList.contains( existList.at( i ) ) )
        {
            deleteQueueSR( existList.at( i ), dbname );
        }
    }
    
    db.commit();
    return nl;
}

void Database::updateQueueSR( QueueSR sr, const QString& dbname )
{   
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    query.prepare(  "UPDATE " + Settings::engineer().toUpper() + " SET "
                    "SRTYPE=:srtype, CREATOR=:creator, CUS_ACCOUNT=:cus_account, CUS_FIRSTNAME=:cus_firstname, "
                    "CUS_LASTNAME=:cus_lastname, CUS_TITLE=:cus_title, CUS_EMAIL=:cus_email, CUS_PHONE=:cus_phone, "
                    "CUS_ONSITEPHONE=:cus_onsitephone, CUS_LANG=:cus_lang, SEVERITY=:severity, STATUS=:status, "
                    "BDESC=:bdesc, DDESC=:ddesc, GEO=:geo, HOURS=:hours, CONTRACT=:contract, SERVICE_LEVEL=:service_level, "
                    "CREATED=:created, LASTUPDATE=:lastupdate, HIGHVALUE=:highvalue, CRITSIT=:critsit, ALT_CONTACT=:alt_contact, "
                    "BUG=:bug, BUGTITLE=:bugtitle, OWNER=:owner, SUBOWNER=:subowner, CRSR=:crsr, CUS_RATING=:rating WHERE ID = :id" );
    
    query.bindValue( ":srtype", sr.srtype );
    query.bindValue( ":creator", sr.creator );
    query.bindValue( ":cus_account", sr.cus_account );
    query.bindValue( ":cus_firstname", sr.cus_firstname );
    query.bindValue( ":cus_lastname", sr.cus_lastname );
    query.bindValue( ":cus_title", sr.cus_title );
    query.bindValue( ":cus_email", sr.cus_email );
    query.bindValue( ":cus_phone", sr.cus_phone );
    query.bindValue( ":cus_onsitephone", sr.cus_onsitephone );
    query.bindValue( ":cus_lang", sr.cus_lang );
    query.bindValue( ":severity", sr.severity );
    query.bindValue( ":status", sr.status );
    query.bindValue( ":bdesc", sr.bdesc );
    query.bindValue( ":ddesc", sr.ddesc );
    query.bindValue( ":geo", sr.geo );
    query.bindValue( ":hours", sr.hours );
    query.bindValue( ":contract", sr.contract );
    query.bindValue( ":service_level", sr.service_level );
    query.bindValue( ":created", sr.created );
    query.bindValue( ":lastupdate", sr.lastupdate );
    query.bindValue( ":highvalue", sr.highvalue );
    query.bindValue( ":critsit", sr.critsit );
    query.bindValue( ":alt_contact", sr.alt_contact );
    query.bindValue( ":bug", sr.bug );
    query.bindValue( ":bugtitle", sr.bugtitle );
    query.bindValue( ":id", sr.id );
    query.bindValue( ":owner", sr.owner );
    query.bindValue( ":subowner", sr.subowner );
    query.bindValue( ":crsr", sr.crsr );
    query.bindValue( ":rating", sr.rating );

    if ( !query.exec() ) 
    {
        qDebug() << "[DATABASE] Failed to update" << sr.id << "in DB" << query.executedQuery() << query.lastError();
    }
}

void Database::insertQueueSR( QueueSR sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare(  "INSERT INTO " + Settings::engineer().toUpper() +
                    "( ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                    "  CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                    "  CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING )"
                    "VALUES" 
                    "( :id, :srtype, :creator, :cus_account, :cus_firstname, :cus_lastname, :cus_title, :cus_email, :cus_phone, "
                    "  :cus_onsitephone, :cus_lang, :severity, :status, :bdesc, :ddesc, :geo, :hours, :contract, :service_level, "
                    "  :created, :lastupdate, :highvalue, :critsit, 'none', :alt_contact, :bug, :bugtitle, :owner, :subowner, :crsr, :rating )" );
        
    query.bindValue( ":id", sr.id );
    query.bindValue( ":srtype", sr.srtype );
    query.bindValue( ":creator", sr.creator );
    query.bindValue( ":cus_account", sr.cus_account );
    query.bindValue( ":cus_firstname", sr.cus_firstname );
    query.bindValue( ":cus_lastname", sr.cus_lastname );
    query.bindValue( ":cus_title", sr.cus_title );
    query.bindValue( ":cus_email", sr.cus_email );
    query.bindValue( ":cus_phone", sr.cus_phone );
    query.bindValue( ":cus_onsitephone", sr.cus_onsitephone );
    query.bindValue( ":cus_lang", sr.cus_lang );
    query.bindValue( ":severity", sr.severity );
    query.bindValue( ":status", sr.status );
    query.bindValue( ":bdesc", sr.bdesc );
    query.bindValue( ":ddesc", sr.ddesc );
    query.bindValue( ":geo", sr.geo );
    query.bindValue( ":hours", sr.hours );
    query.bindValue( ":contract", sr.contract );
    query.bindValue( ":service_level", sr.service_level );
    query.bindValue( ":created", sr.created );
    query.bindValue( ":lastupdate", sr.lastupdate );
    query.bindValue( ":highvalue", sr.highvalue );
    query.bindValue( ":critsit", sr.critsit );
    query.bindValue( ":alt_contact", sr.alt_contact );
    query.bindValue( ":bug", sr.bug );
    query.bindValue( ":bugtitle", sr.bugtitle );
    query.bindValue( ":owner", sr.owner );
    query.bindValue( ":subowner", sr.subowner );
    query.bindValue( ":crsr", sr.crsr );
    query.bindValue( ":rating", sr.rating );
    
    if ( !query.exec() ) 
    {
        qDebug() << "[DATABASE] Failed to add" << sr.id << "into DB" << query.executedQuery() << query.lastError();
    }
}

void Database::deleteQueueSR( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "DELETE FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

bool Database::queueSrExists( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query(db);
    
    query.prepare( "SELECT ID FROM " + Settings::engineer().toUpper() +" WHERE ( ID = :id )" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return true;
    }
    else 
    {    
        return false;
    }
}

bool Database::srWasUpdated( QueueSR sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    query.prepare( "SELECT LASTUPDATE FROM " + Settings::engineer().toUpper() + " WHERE ( ID = :id )" );
    query.bindValue( ":id", sr.id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        if ( sr.lastupdate != query.value( 0 ).toString() )
        {
            return true;
        }
    }
    
    return false;
}

QString Database::getStatus( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT STATUS FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return query.value( 0 ).toString();
    }
    else
    {
        query.prepare( "SELECT STATUS FROM QMON WHERE ID = :id" );
        query.bindValue( ":id", id );
    
        if ( !query.exec() ) 
        {
            qDebug() << query.lastError().text();
        }
        
        if ( query.next() ) 
        {
            return query.value( 0 ).toString();
        }
        else
        {
            return "ERROR";
        }
    }
}

void Database::setQueueDisplay( const QString& d, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare(  "UPDATE " + Settings::engineer().toUpper() + " SET "
                    "DISPLAY=:display "
                    "WHERE id = :id" );
    
    query.bindValue( ":display", d.split( "-" ).at( 1 ) );
    query.bindValue( ":id", d.split( "-" ).at( 0 ) );
    
    if ( !query.exec() ) 
    { 
        qDebug() << query.lastError().text();
    }
}

void Database::closeAllTables( const QString& dbname)
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare(  "UPDATE " + Settings::engineer().toUpper() + " SET "
                    "DISPLAY='none' " );
        
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

void Database::expandAllTables( const QString& dbname)
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare(  "UPDATE " + Settings::engineer().toUpper() + " SET "
                    "DISPLAY='block' " );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

QList<QueueSR> Database::getSrList( bool s, bool a, const QString& dbname, const QString& filter )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QStringList l;
    QString f;
    QDateTime now = QDateTime::currentDateTime();
    QList<QueueSR> srlist;

    QSqlQuery query( db );
    
    if ( !filter.isNull() )
    {
        f = " WHERE ( CUS_ACCOUNT LIKE '%" + filter + "%' )"
            "OR ( ID LIKE '%" + filter + "%' )"
            "OR ( CREATOR LIKE '%" + filter + "%' )"
            "OR ( CUS_FIRSTNAME LIKE '%" + filter + "%' )"
            "OR ( CUS_LASTNAME LIKE '%" + filter + "%' )"
            "OR ( BDESC LIKE '%" + filter + "%' ) ";
    }
   
    db.transaction();

    if ( a )
    {
        if ( s ) 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + f + " ORDER BY CREATED ASC" );
        }
        else 
        {   
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + f + " ORDER BY LASTUPDATE ASC" );
        }
    }
    else
    {
        if ( s ) 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + f + " ORDER BY CREATED DESC" );
        }
        else 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + f + " ORDER BY LASTUPDATE DESC" );
        }
    }
        
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    while( query.next() )
    {
        QueueSR sr;
        
        sr.id = query.value(0).toString();
        sr.srtype = query.value(1).toString();
        sr.creator = query.value(2).toString();
        sr.cus_account = query.value(3).toString();
        sr.cus_firstname = query.value(4).toString();
        sr.cus_lastname = query.value(5).toString();
        sr.cus_title = query.value(6).toString();
        sr.cus_email = query.value(7).toString();
        sr.cus_phone = query.value(8).toString();
        sr.cus_onsitephone = query.value(9).toString();
        sr.cus_lang = query.value(10).toString();
        sr.severity = query.value(11).toString();
        sr.status = query.value(12).toString();
        sr.bdesc = query.value(13).toString();
        sr.ddesc = query.value(14).toString();
        sr.geo = query.value(15).toString();
        sr.hours = query.value(16).toString();
        sr.contract = query.value(17).toString();
        sr.service_level = query.value(18).toInt();
        sr.created = query.value(19).toString();
        sr.lastupdate = query.value(20).toString();
        sr.highvalue = query.value(21).toBool();
        sr.critsit = query.value(22).toBool();
        sr.display = query.value(23).toString();
        sr.alt_contact = query.value(24).toString();
        sr.bug = query.value(25).toString();
        sr.bugtitle = query.value(26).toString();
        sr.owner = query.value(27).toString();
        sr.subowner = query.value(28).toString();
        sr.crsr = query.value( 29 ).toString();
        sr.rating = query.value( 30 ).toString();
        
        QDateTime a = QDateTime::fromString( sr.created, "yyyy-MM-dd hh:mm:ss" );
        QDateTime u = QDateTime::fromString( sr.lastupdate, "yyyy-MM-dd hh:mm:ss" );
        
        sr.age = a.daysTo( now );
        sr.lastUpdateDays = u.daysTo( now );
     
        if( sr.owner == Settings::engineer().toUpper() ) srlist.append( sr );
    }
    
    db.commit();
     
    return srlist;
}

QList<QueueSR> Database::getSubSrList( bool s, bool a, const QString& dbname, const QString& filter )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QStringList l;
    QString f;
    QDateTime now = QDateTime::currentDateTime();
    QList<QueueSR> srlist;
    
    QSqlQuery query( db );
    
    if ( !filter.isNull() )
    {
        f = " AND ( CUS_ACCOUNT LIKE '%" + filter + "%' )"
        "OR ( ID LIKE '%" + filter + "%' )"
        "OR ( CREATOR LIKE '%" + filter + "%' )"
        "OR ( CUS_FIRSTNAME LIKE '%" + filter + "%' )"
        "OR ( CUS_LASTNAME LIKE '%" + filter + "%' )"
        "OR ( BDESC LIKE '%" + filter + "%' ) ";
    }
    
    db.transaction();
    
    if ( a )
    {
        if ( s ) 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + " WHERE SUBOWNER != ''" + f + "ORDER BY CREATED ASC" );
        }
        else 
        {   
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + "  WHERE SUBOWNER != ''" + f + "ORDER BY LASTUPDATE ASC" );
        }
    }
    else
    {
        if ( s ) 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + "  WHERE SUBOWNER != ''" + f + "ORDER BY CREATED DESC" );
        }
        else 
        {    
            query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
            "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
            "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, OWNER, SUBOWNER, CRSR, CUS_RATING FROM " + Settings::engineer().toUpper() + "  WHERE SUBOWNER != ''" + f + "ORDER BY LASTUPDATE DESC" );
        }
    }
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    else
    {
        while( query.next() )
        {
            QueueSR sr;
            
            sr.id = query.value(0).toString();
            sr.srtype = query.value(1).toString();
            sr.creator = query.value(2).toString();
            sr.cus_account = query.value(3).toString();
            sr.cus_firstname = query.value(4).toString();
            sr.cus_lastname = query.value(5).toString();
            sr.cus_title = query.value(6).toString();
            sr.cus_email = query.value(7).toString();
            sr.cus_phone = query.value(8).toString();
            sr.cus_onsitephone = query.value(9).toString();
            sr.cus_lang = query.value(10).toString();
            sr.severity = query.value(11).toString();
            sr.status = query.value(12).toString();
            sr.bdesc = query.value(13).toString();
            sr.ddesc = query.value(14).toString();
            sr.geo = query.value(15).toString();
            sr.hours = query.value(16).toString();
            sr.contract = query.value(17).toString();
            sr.service_level = query.value(18).toInt();
            sr.created = query.value(19).toString();
            sr.lastupdate = query.value(20).toString();
            sr.highvalue = query.value(21).toBool();
            sr.critsit = query.value(22).toBool();
            sr.display = query.value(23).toString();
            sr.alt_contact = query.value(24).toString();
            sr.bug = query.value(25).toString();
            sr.bugtitle = query.value(26).toString();
            sr.owner = query.value(27).toString();
            sr.subowner = query.value(28).toString();
            sr.crsr = query.value( 29 ).toString();
            sr.rating = query.value( 30 ).toString();
            
            QDateTime a = QDateTime::fromString( sr.created, "yyyy-MM-dd hh:mm:ss" );
            QDateTime u = QDateTime::fromString( sr.lastupdate, "yyyy-MM-dd hh:mm:ss" );
            
            sr.age = a.daysTo( now );
            sr.lastUpdateDays = u.daysTo( now );
            
            if ( sr.subowner.toUpper() == Settings::engineer().toUpper() ) 
            {
                srlist.append( sr );
            }
        }
    }
    
    db.commit();
    
    return srlist;
}

QueueSR Database::getSrInfo( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( "sqliteDB" );
    QDateTime now = QDateTime::currentDateTime();
    QueueSR sr;

    QSqlQuery query( db );
    
    query.prepare(  "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, CUS_PHONE, "
                    "CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, CONTRACT, SERVICE_LEVEL, "
                    "CREATED, LASTUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, CRSR, SUBOWNER, CUS_RATING FROM " + Settings::engineer().toUpper() + " WHERE ( ID = :id )" );
    
    query.bindValue( ":id", id );
        
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if( !query.next() )
    {

        query.prepare( "SELECT ID, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, "
                       "       CUS_PHONE, CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, SUPPORT_PROGRAM_LONG, "
                       "       SERVICE_LEVEL, CREATEDATE, LUPDATE, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, CRSR, SUBOWNER, CUS_RATING "
                       "       FROM QMON WHERE ( ID = :id )" );
        
        query.bindValue( ":id", id );
        
        if ( !query.exec() ) 
        {
            qDebug() << query.lastError().text();
        }
        
        if ( !query.next() )
        {
            return sr;
        }
    }
    
    sr.id = query.value(0).toString();
    sr.srtype = query.value(1).toString();
    sr.creator = query.value(2).toString();
    sr.cus_account = query.value(3).toString();
    sr.cus_firstname = query.value(4).toString();
    sr.cus_lastname = query.value(5).toString();
    sr.cus_title = query.value(6).toString();
    sr.cus_email = query.value(7).toString();
    sr.cus_phone = query.value(8).toString();
    sr.cus_onsitephone = query.value(9).toString();
    sr.cus_lang = query.value(10).toString();
    sr.severity = query.value(11).toString();
    sr.status = query.value(12).toString();
    sr.bdesc = query.value(13).toString();
    sr.ddesc = query.value(14).toString();
    sr.geo = query.value(15).toString();
    sr.hours = query.value(16).toString();
    sr.contract = query.value(17).toString();
    sr.service_level = query.value(18).toInt();
    sr.created = query.value(19).toString();
    sr.lastupdate = query.value(20).toString();
    sr.highvalue = query.value(21).toBool();
    sr.critsit = query.value(22).toBool();
    sr.display = query.value(23).toString();
    sr.alt_contact = query.value(24).toString();
    sr.bug = query.value(25).toString();
    sr.bugtitle = query.value(26).toString();
    sr.crsr = query.value(27).toString();
    sr.subowner = query.value(28).toString();
    sr.rating = query.value(29).toString();
    
    if ( !sr.creator.isEmpty() )
    {
        sr.isCr = true;
    }
    else
    {
        sr.isCr = false;
    }
    
    QDateTime a = QDateTime::fromString( sr.created, "yyyy-MM-dd hh:mm:ss" );
    QDateTime u = QDateTime::fromString( sr.lastupdate, "yyyy-MM-dd hh:mm:ss" );
    
    sr.age = a.daysTo( now );
    sr.lastUpdateDays = u.daysTo( now );

    return sr;
}

QStringList Database::getSrNrList( const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    QStringList list;
    
    query.prepare( "SELECT ID FROM " + Settings::engineer().toUpper() );
    query.exec();
    
    while ( query.next() )
    {
        list.append( query.value( 0 ).toString() );
    }
    
    return list;
}

QString Database::getDetailedDescription( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT DDESC FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return query.value( 0 ).toString();
    }
    else
    {
        query.prepare( "SELECT DDESC FROM QMON WHERE ID = :id" );
        query.bindValue( ":id", id );
        
        if ( !query.exec() ) 
        {
            qDebug() << query.lastError().text();
        }
        
        if ( query.next() )
        {
            return query.value( 0 ).toString();
        }
        else
        {
            return "ERROR";
        }
    }
}

bool Database::isCr( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( db );
    
    query.prepare( "SELECT SRTYPE FROM " + Settings::engineer().toUpper() + " WHERE ( ID = :id )" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        if ( query.value( 0 ).toString() == "cr" )
        {
            return true; 
        }
        else
        {
            return false;
        }
    }
    else 
    {    
        query.prepare( "SELECT SRTYPE FROM QMON WHERE ( ID = :id )" );
        query.bindValue( ":id", id );
        
        if ( !query.exec() ) 
        {
            qDebug() << query.lastError().text();
        }
    
        if ( query.next() )
        {
            if ( query.value( 0 ).toString() == "cr" )
            {
                return true; 
            }
            else
            {
                return false;
            }
        }
    }
    
    return false;
}

QString Database::getCustomer( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return query.value( 0 ).toString() + " (" + query.value( 1 ).toString() + " " + query.value( 2 ).toString() + ")";
    }
    else
    {
        query.prepare( "SELECT CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME FROM QMON WHERE ID = :id" );
        query.bindValue( ":id", id );
        
        if ( !query.exec() )
        {
            qDebug() << query.lastError().text();
        }
       
        if ( query.next() )
        {
            return query.value( 0 ).toString() + " (" + query.value( 1 ).toString() + " " + query.value( 2 ).toString() + ")";
        }
        else
        {
            return "ERROR";
        }
    }
}

QString Database::getCreator( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT CREATOR FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return query.value( 0 ).toString();
    }
    else
    {
        query.prepare( "SELECT CREATOR FROM QMON WHERE ID = :id" );
        query.bindValue( ":id", id );
        
        if ( !query.exec() )
        {
            qDebug() << query.lastError().text();
        }
       
        if ( query.next() )
        {
            return query.value( 0 ).toString();
        }
        else
        {
            return "ERROR";
        }
    }
}

QString Database::getBriefDescription( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT BDESC FROM " + Settings::engineer().toUpper() + " WHERE ID = :id" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return query.value( 0 ).toString();
    }
    else
    {
        query.prepare( "SELECT BDESC FROM QMON WHERE ID = :id" );
        query.bindValue( ":id", id );
        
        if ( !query.exec() ) 
        {
            qDebug() << query.lastError().text();
        }
        
        if ( query.next() )
        {
            return query.value( 0 ).toString();
        }
        else
        {
            return "ERROR";
        }
    }
}

/*


                M O N I T O R


*/

void Database::insertQmonSR( QmonSR sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
           
    query.prepare( "INSERT INTO QMON "
                   "( ID, QUEUE, BOMGARQ, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, "
                   "  CUS_PHONE, CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, SOURCE, SUPPORT_PROGRAM, "
                   "  SUPPORT_PROGRAM_LONG, ROUTING_PRODUCT, SUPPORT_GROUP_ROUTING, INT_TYPE, SUBTYPE, SERVICE_LEVEL, CATEGORY, "
                   "  RESPOND_VIA, AGE, LASTUPDATE, TIMEINQ, SLA, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, LUPDATE, CREATEDATE, CRSR, LTSS, "
                   "  SUBOWNER, CUS_RATING ) "
                   " VALUES "
                   "( :id, :queue, :bomgarq, :srtype, :creator, :cus_account, :cus_firstname, :cus_lastname, :cus_title, :cus_email, "
                   "  :cus_phone, :cus_onsitephone, :cus_lang, :severity, :status, :bdesc, :ddesc, :geo, :hours, :source, :support_program, "
                   "  :support_program_long, :routing_product, :support_group_routing, :int_type, :subtype, :service_level, :category, "
                   "  :respond_via, :age, :lastupdate, :timeinq, :sla, :highvalue, :critsit, 'none', :alt_contact, :bug, :bugtitle, :lupdate, :createdate, :crsr, :ltss, "
                   "  :subowner, :rating )" );
  
    query.bindValue( ":id", sr.id );
    query.bindValue( ":queue", sr.queue );
    query.bindValue( ":bomgarq", sr.bomgarQ );
    query.bindValue( ":srtype", sr.srtype );
    query.bindValue( ":creator", sr.creator );
    query.bindValue( ":cus_account", sr.cus_account );
    query.bindValue( ":cus_firstname", sr.cus_firstname );
    query.bindValue( ":cus_lastname", sr.cus_lastname );
    query.bindValue( ":cus_title", sr.cus_title );
    query.bindValue( ":cus_email", sr.cus_email );
    query.bindValue( ":cus_phone", sr.cus_phone );
    query.bindValue( ":cus_onsitephone", sr.cus_onsitephone );
    query.bindValue( ":cus_lang", sr.cus_lang );
    query.bindValue( ":severity", sr.severity );
    query.bindValue( ":status", sr.status );
    query.bindValue( ":bdesc", sr.bdesc );
    query.bindValue( ":ddesc", sr.ddesc );
    query.bindValue( ":geo", sr.geo );
    query.bindValue( ":hours", sr.hours );
    query.bindValue( ":source", sr.source );
    query.bindValue( ":support_program", sr.support_program );
    query.bindValue( ":support_program_long", sr.support_program_long );
    query.bindValue( ":routing_product", sr.routing_product );
    query.bindValue( ":support_group_routing", sr.support_group_routing );
    query.bindValue( ":int_type", sr.int_type );
    query.bindValue( ":subtype", sr.subtype );
    query.bindValue( ":service_level", sr.service_level );
    query.bindValue( ":category", sr.category );
    query.bindValue( ":respond_via", sr.respond_via );
    query.bindValue( ":age", sr.agesec );
    query.bindValue( ":lastupdate", sr.lastupdatesec );
    query.bindValue( ":timeinq", sr.timeinqsec );
    query.bindValue( ":sla", sr.slasec );
    query.bindValue( ":highvalue", sr.highvalue );
    query.bindValue( ":critsit", sr.critsit );
    query.bindValue( ":alt_contact", sr.alt_contact );
    query.bindValue( ":bug", sr.bug );
    query.bindValue( ":bugtitle", sr.bugtitle );
    query.bindValue( ":lupdate", sr.lupdate );
    query.bindValue( ":createdate", sr.cdate );
    query.bindValue( ":crsr", sr.crsr );
    query.bindValue( ":ltss", sr.hasLTSS );
    query.bindValue( ":subowner", sr.subowner );
    query.bindValue( ":rating", sr.rating );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

void Database::dropQmon( const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    if ( !query.exec( "DELETE FROM QMON" ) ) 
    {
        qDebug() << query.lastError().text();
    }
}

void Database::updateQmonSR( QmonSR sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( db );
    
    query.prepare( "UPDATE QMON SET "
                   " QUEUE=:queue, BOMGARQ=:bomgarQ, SRTYPE=:srtype, CREATOR=:creator, CUS_ACCOUNT=:cus_account, CUS_FIRSTNAME=:cus_firstname, "
                   " CUS_LASTNAME=:cus_lastname, CUS_TITLE=:cus_title, CUS_EMAIL=:cus_email, CUS_PHONE=:cus_phone, CUS_ONSITEPHONE=:cus_onsitephone, "
                   " CUS_LANG=:cus_lang, SEVERITY=:severity, STATUS=:status, BDESC=:bdesc, DDESC=:ddesc, GEO=:geo, HOURS=:hours, SOURCE=:source, "
                   " SUPPORT_PROGRAM=:support_program, SUPPORT_PROGRAM_LONG=:support_program_long, ROUTING_PRODUCT=:routing_product, "
                   " SUPPORT_GROUP_ROUTING=:support_group_routing, INT_TYPE=:int_type, SUBTYPE=:subtype, SERVICE_LEVEL=:service_level, "
                   " CATEGORY=:category, RESPOND_VIA=:respond_via, AGE=:age, LASTUPDATE=:lastupdate, TIMEINQ=:timeinq, SLA=:sla, "
                   " HIGHVALUE=:highvalue, CRITSIT=:critsit, ALT_CONTACT=:alt_contact, BUG=:bug, BUGTITLE=:bugtitle, LUPDATE=:lupdate, CREATEDATE=:createdate, CRSR=:crsr, "
                   " LTSS=:ltss, SUBOWNER=:subowner, CUS_RATING=:rating WHERE ID=:id" );
                     
    query.bindValue( ":queue", sr.queue );
    query.bindValue( ":bomgarq", sr.bomgarQ );
    query.bindValue( ":srtype", sr.srtype );
    query.bindValue( ":creator", sr.creator );
    query.bindValue( ":cus_account", sr.cus_account );
    query.bindValue( ":cus_firstname", sr.cus_firstname );
    query.bindValue( ":cus_lastname", sr.cus_lastname );
    query.bindValue( ":cus_title", sr.cus_title );
    query.bindValue( ":cus_email", sr.cus_email );
    query.bindValue( ":cus_phone", sr.cus_phone );
    query.bindValue( ":cus_onsitephone", sr.cus_onsitephone );
    query.bindValue( ":cus_lang", sr.cus_lang );
    query.bindValue( ":severity", sr.severity );
    query.bindValue( ":status", sr.status );
    query.bindValue( ":bdesc", sr.bdesc );
    query.bindValue( ":ddesc", sr.ddesc );
    query.bindValue( ":geo", sr.geo );
    query.bindValue( ":hours", sr.hours );
    query.bindValue( ":source", sr.source );
    query.bindValue( ":support_program", sr.support_program );
    query.bindValue( ":support_program_long", sr.support_program_long );
    query.bindValue( ":routing_product", sr.routing_product );
    query.bindValue( ":support_group_routing", sr.support_group_routing );
    query.bindValue( ":int_type", sr.int_type );
    query.bindValue( ":subtype", sr.subtype );
    query.bindValue( ":service_level", sr.service_level );
    query.bindValue( ":category", sr.category );
    query.bindValue( ":respond_via", sr.respond_via );
    query.bindValue( ":age", sr.agesec );
    query.bindValue( ":lastupdate", sr.lastupdate );
    query.bindValue( ":timeinq", sr.timeinqsec );
    query.bindValue( ":sla", sr.slasec );
    query.bindValue( ":highvalue", sr.highvalue );
    query.bindValue( ":critsit", sr.critsit );
    query.bindValue( ":alt_contact", sr.alt_contact );
    query.bindValue( ":bug", sr.bug );
    query.bindValue( ":bugtitle", sr.bugtitle );
    query.bindValue( ":id", sr.id );    
    query.bindValue( ":lupdate", sr.lupdate );    
    query.bindValue( ":createdate", sr.cdate );    
    query.bindValue( ":crsr", sr.crsr );
    query.bindValue( ":ltss", sr.hasLTSS );
    query.bindValue( ":subowner", sr.subowner );
    query.bindValue( ":rating", sr.rating );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
}

void Database::setQmonDisplay( const QString& display, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( "UPDATE QMON SET DISPLAY = :display WHERE id = :id", db );
                
    query.bindValue( ":display", display.split( "-" ).at( 1 ) );
    query.bindValue( ":id", display.split( "-" ).at( 0 ) );
                
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();   
    }
}

QList<Notification> Database::updateQmon( QmonData qd, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QList< QmonSR > srList = qd.srList;
    QStringList existList = getQmonSrNrs( dbname );
    QStringList newList;
    QList<Notification> nl;
    
    bool initial = existList.isEmpty();
    
    QStringList q;
    
    for ( int i = 0; i < Settings::queuesToMonitor().size(); ++i )
    {
        q.append( Settings::queuesToMonitor().at( i ).split( "|" ).at( 1 ) );
    }
    
    db.transaction();
    
    for ( int i = 0; i < srList.size(); ++i ) 
    {
        QmonSR sr = srList.at( i );
        
        QString type;
        
        if ( sr.isCr )
        {
            type = "CR";
        }
        else
        {
            type = "SR";
        }
        
        if ( qmonExists( sr.id ) )
        {
            if ( qmonQueueChanged( sr, dbname ) && q.contains( sr.queue ) )
            {
                if ( sr.severity == "Low" )
                {
                    Notification n;
                    n.type = "kueue-monitor-low";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "Medium" )
                {
                    Notification n;
                    n.type = "kueue-monitor-medium";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "Urgent" )
                {
                    Notification n;
                    n.type = "kueue-monitor-urgent";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "High" )
                {
                    Notification n;
                    n.type = "kueue-monitor-high";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
            }
            
            updateQmonSR( sr, dbname );
        }
        else
        {
            insertQmonSR( sr, dbname );
            
            if ( !initial && q.contains( sr.queue ) )
            {
                if ( sr.severity == "Low" )
                {
                    Notification n;
                    n.type = "kueue-monitor-low";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "Medium" )
                {
                    Notification n;
                    n.type = "kueue-monitor-medium";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "Urgent" )
                {
                    Notification n;
                    n.type = "kueue-monitor-urgent";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
                else if ( sr.severity == "High" )
                {
                    Notification n;
                    n.type = "kueue-monitor-high";
                    n.title = "New " + type + " in " + QString( sr.queue );
                    n.body = "<b>" + type + "#" + sr.id + "</b><br>" + sr.bdesc;
                    n.sr = sr.id;
                    nl.append( n );
                }
            }
        }
         
        newList.append( sr.id );
    }
    
    for ( int i = 0; i < existList.size(); ++i ) 
    {   
        if ( !newList.contains( existList.at( i ) ) )
        {
            deleteQmonSR( existList.at( i ), dbname );
        }
    }
    
    db.commit();
    return nl;
}

void Database::deleteQmonSR( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    query.prepare( "DELETE FROM QMON WHERE ID = :id" );
    
    query.bindValue( ":id", id );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
}

QStringList Database::getQmonSrNrs( const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( db );
    QStringList l;
    
    query.prepare( "SELECT ID FROM QMON" );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    while( query.next() )
    {
        l.append( query.value( 0 ).toString() );
    }
    
    return l;    
}

bool Database::qmonExists( const QString& id, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    query.prepare( "SELECT ID FROM QMON WHERE ( ID = :id )" );
    query.bindValue( ":id", id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        return true;
    }
    else 
    {    
        return false;
    }
}

QList< QmonSR > Database::getQmonQueue( const QString& queue, QString geo, const QString& dbname, const QString& filter )
{
    QDateTime now = QDateTime::currentDateTime();
    QList< QmonSR > list;
    QString f;
    
    if ( !filter.isNull() )
    {
        f = " AND ( CUS_ACCOUNT LIKE '%" + filter + "%' "
            "OR ID LIKE '%" + filter + "%' "
            "OR CREATOR LIKE '%" + filter + "%' "
            "OR CUS_FIRSTNAME LIKE '%" + filter + "%' "
            "OR CUS_LASTNAME LIKE '%" + filter + "%' "
            "OR BDESC LIKE '%" + filter + "%' ) ";
    }
    
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    db.transaction();
    
    if ( geo != "ALL" )
    {
        query.prepare( "SELECT ID, QUEUE, BOMGARQ, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, "
                       "       CUS_PHONE, CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, SOURCE, SUPPORT_PROGRAM, "
                       "       SUPPORT_PROGRAM_LONG, ROUTING_PRODUCT, SUPPORT_GROUP_ROUTING, INT_TYPE, SUBTYPE, SERVICE_LEVEL, CATEGORY, "
                       "       RESPOND_VIA, AGE, LASTUPDATE, TIMEINQ, SLA, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, CRSR, "
                       "       LTSS, SUBOWNER, CUS_RATING FROM QMON WHERE ( GEO = :geo ) AND ( QUEUE = :queue ) OR ( SUBOWNER = :queue1 ) " + f );

        query.bindValue( ":geo", geo );
        query.bindValue( ":queue", queue );
        query.bindValue( ":queue1", queue );
        
    }
    else
    {
        query.prepare( "SELECT ID, QUEUE, BOMGARQ, SRTYPE, CREATOR, CUS_ACCOUNT, CUS_FIRSTNAME, CUS_LASTNAME, CUS_TITLE, CUS_EMAIL, "
                       "       CUS_PHONE, CUS_ONSITEPHONE, CUS_LANG, SEVERITY, STATUS, BDESC, DDESC, GEO, HOURS, SOURCE, SUPPORT_PROGRAM, "
                       "       SUPPORT_PROGRAM_LONG, ROUTING_PRODUCT, SUPPORT_GROUP_ROUTING, INT_TYPE, SUBTYPE, SERVICE_LEVEL, CATEGORY, "
                       "       RESPOND_VIA, AGE, LASTUPDATE, TIMEINQ, SLA, HIGHVALUE, CRITSIT, DISPLAY, ALT_CONTACT, BUG, BUGTITLE, CRSR, LTSS, "
                       "       SUBOWNER, CUS_RATING FROM QMON WHERE ( QUEUE = :queue ) OR ( SUBOWNER = :queue1 ) " + f  );

        query.bindValue( ":queue", queue );
        query.bindValue( ":queue1", queue );
    }
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }

    while ( query.next() )
    {
        QmonSR sr;
        
        sr.id = query.value( 0 ).toString();
        sr.queue = query.value( 1).toString();
        sr.bomgarQ = query.value(2).toString();
        sr.srtype = query.value(3).toString();
        sr.creator = query.value(4).toString();
        sr.cus_account = query.value(5).toString();
        sr.cus_firstname = query.value(6).toString();
        sr.cus_lastname = query.value(7).toString();
        sr.cus_title = query.value(8).toString();
        sr.cus_email = query.value(9).toString();
        sr.cus_phone = query.value(10).toString();
        sr.cus_onsitephone = query.value(11).toString();
        sr.cus_lang = query.value(12).toString();
        sr.severity = query.value(13).toString();
        sr.status = query.value(14).toString();
        sr.bdesc = query.value(15).toString();
        sr.ddesc = query.value(16).toString();
        sr.geo = query.value(17).toString();
        sr.hours = query.value(18).toString();
        sr.source = query.value(19).toString();
        sr.support_program = query.value(20).toString();
        sr.support_program_long = query.value(21).toString();
        sr.routing_product = query.value(22).toString();
        sr.support_group_routing = query.value(23).toString();
        sr.int_type = query.value(24).toString();
        sr.subtype = query.value(25).toString();
        sr.service_level = query.value(26).toInt();
        sr.category = query.value(27).toString();
        sr.respond_via = query.value(28).toString();
        sr.agesec = query.value(29).toInt();
        sr.lastupdatesec = query.value(30).toInt();
        sr.timeinqsec = query.value(31).toInt();
        sr.slasec = query.value(32).toInt();
        sr.highvalue = query.value(33).toBool();
        sr.critsit = query.value(34).toBool();
        sr.display = query.value(35).toString();
        sr.alt_contact = query.value(36).toString();
        sr.bug = query.value(37).toString();
        sr.bugtitle = query.value(38).toString();
        sr.crsr = query.value(39).toString();
        sr.hasLTSS = query.value(40).toBool();
        sr.subowner = query.value(41).toString();
        sr.rating = query.value(42).toString();
        
        if ( sr.srtype == "cr" )
        {
            sr.isCr = true;
        }
        else
        {
            sr.isCr = false;
        }
        
        if ( sr.bomgarQ.isEmpty() )
        {
            sr.isChat = false;
        }
        else
        {
            sr.isChat = true;
        }
        
        list.append( sr );
    }
        
    db.commit();
    
    return list;
}

bool Database::qmonQueueChanged( QmonSR sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT QUEUE, SUBOWNER FROM QMON WHERE ( ID = :id )" );
    query.bindValue( ":id", sr.id );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }

    if ( query.next() )
    {
        if ( query.value( 0 ).toString() == sr.queue ) 
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

bool Database::qmonSeverityChanged( QmonSR sr, const QString& dbname  )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query(db);
    
    query.prepare( "SELECT SEVERITY FROM QMON WHERE ( ID = :id )" );
    query.bindValue( ":id", sr.id );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }

    if ( query.next() )
    {
        if ( query.value( 0 ).toString() == sr.severity )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

void Database::updateStats( Statz s, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( db );
    
    db.transaction();
    
    query.exec( "DELETE FROM STATS" );
    query.exec( "DELETE FROM STATS_SURVEYS" );
    query.exec( "DELETE FROM STATS_CLOSED" );
    
    query.prepare( "INSERT INTO STATS( CLOSED_SR, CLOSED_CR, SR_TTS_AVG, CSAT_ENG_AVG, CSAT_SR_AVG, CSAT_RTS_PERCENT ) "
                   "VALUES"
                   "( :closed_sr, :closed_cr, :sr_tts_avg, :csat_eng_avg, :csat_sr_avg, :csat_rts_percent )" );
    
    query.bindValue( ":closed_sr", s.closedSr );
    query.bindValue( ":closed_cr", s.closedCr );
    query.bindValue( ":sr_tts_avg", s.srTtsAvg );
    query.bindValue( ":csat_eng_avg", s.csatEngAvg );
    query.bindValue( ":csat_sr_avg", s.csatSrAvg );
    query.bindValue( ":csat_rts_percent", s.csatRtsPercent );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    QList<ClosedItem> cl = s.closedList;
    QList<Survey> sl = s.surveyList;
    
    for ( int i = 0; i < sl.size(); ++i ) 
    {   
        Database::insertSurvey( sl.at( i ), dbname );
    }
    
    for ( int i = 0; i < cl.size(); ++i ) 
    {   
        Database::insertClosed( cl.at( i ), dbname );
    }
    
    db.commit();
}

void Database::insertSurvey( Survey s, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( "INSERT INTO STATS_SURVEYS( ID, RTS, ENGSAT, SRSAT, CUSTOMER, BDESC ) "
                     "VALUES"
                     "( :id, :rts, :engsat, :srsat, :customer, :bdesc )", db );
  
    query.bindValue( ":id", s.id );
    query.bindValue( ":rts", s.rts );
    query.bindValue( ":engsat", s.engsat );
    query.bindValue( ":srsat", s.srsat );
    query.bindValue( ":customer", s.customer );
    query.bindValue( ":bdesc", s.bdesc );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
}

void Database::insertClosed( ClosedItem c, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( "INSERT INTO STATS_CLOSED( ID, TTS, CUSTOMER, BDESC ) "
                     "VALUES"
                     "( :id, :tts, :customer, :bdesc )", db );
  
    query.bindValue( ":id", c.id );
    query.bindValue( ":tts", c.tts );
    query.bindValue( ":customer", c.customer );
    query.bindValue( ":bdesc", c.bdesc );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

Statz Database::getStatz( const QString& dbname )
{
    Statz statz;
    
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( db );
 
    db.transaction();
    
    query.prepare( "SELECT CLOSED_SR, CLOSED_CR, SR_TTS_AVG, CSAT_ENG_AVG, CSAT_SR_AVG, CSAT_RTS_PERCENT FROM STATS" );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        statz.closedSr = query.value( 0 ).toInt();
        statz.closedCr = query.value( 1 ).toInt();
        statz.srTtsAvg = query.value( 2 ).toInt();
        statz.csatEngAvg = query.value( 3 ).toInt();
        statz.csatSrAvg = query.value( 4 ).toInt();
        statz.csatRtsPercent = query.value( 5 ).toInt();
    }
    
    query.prepare( "SELECT ID, RTS, ENGSAT, SRSAT, CUSTOMER, BDESC FROM STATS_SURVEYS" );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    while ( query.next() )
    {
        Survey s;
        
        s.id = query.value( 0 ).toString();
        s.rts = query.value( 1 ).toBool();
        s.engsat = query.value( 2 ).toInt();
        s.srsat = query.value( 3 ).toInt();
        s.customer = query.value( 4 ).toString();
        s.bdesc = query.value( 5 ).toString();
        
        statz.surveyList.append( s );
    }
    
    query.prepare( "SELECT ID, TTS, CUSTOMER, BDESC FROM STATS_CLOSED" );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    while ( query.next() )
    {
        ClosedItem c;
        
        c.id = query.value( 0 ).toString();
        c.tts = query.value( 1 ).toInt();
        c.customer = query.value( 2 ).toString();
        c.bdesc = query.value( 3 ).toString();
        
        statz.closedList.append( c );
    }
    
    db.commit();
    
    return statz;
}

void Database::newDB( bool ask )
{
    int reply;

    if ( ask )
    {
        QMessageBox box;

        box.setWindowTitle( "Rebuild Database" );
        box.setText( "This will delete and rebuild your Database, and restart kueue when finished. Continue?" );
        box.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        box.setDefaultButton( QMessageBox::No );
        box.setIcon( QMessageBox::Question );

        reply = box.exec();
    }
    else
    {
        reply = QMessageBox::Yes;
    }

    if ( reply == QMessageBox::Yes )
    {
        
        QStringList con = QSqlDatabase::connectionNames();
        
        for ( int i = 0; i < con.size(); ++i ) 
        {
            QSqlDatabase::database( con.at( i ) ).close();
            QSqlDatabase::removeDatabase( con.at( i ) );
            qDebug() << "[DATABASE] Removed DB connection" << con.at( i );
        }
        
        QDir dir = QDir( QDesktopServices::storageLocation( QDesktopServices::DataLocation ) );
        QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", "killDB" );
        db.setDatabaseName( dir.path() + "/database.sqlite" );
        
        if ( !db.open() )
        {
            qDebug() << "[DATABASE] Failed to open the database killDB";
        }
        
        QSqlQuery query( db );
        query.prepare( "DROP TABLE QMON" );
        if ( !query.exec() ) qDebug() << "no exec" << query.lastError();
        
        query.prepare( "DROP TABLE " + Settings::engineer().toUpper() );
        if ( !query.exec() ) qDebug() << "no exec" << query.lastError();
        
        query.prepare( "DROP TABLE STATS" );
        if ( !query.exec() ) qDebug() << "no exec" << query.lastError();
        
        query.prepare( "DROP TABLE STATS_CLOSED" );
        if ( !query.exec() ) qDebug() << "no exec" << query.lastError();
        
        query.prepare( "DROP TABLE STATS_SURVEYS" );
        if ( !query.exec() ) qDebug() << "no exec" << query.lastError();
        
        db.close();
        QSqlDatabase::removeDatabase( "killDB" );

        QStringList arg;
        arg << "restart";

        qApp->quit();
        QProcess::startDetached( qApp->arguments()[0], arg );
    }
}

void Database::addGoneSR( const QString& sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    
    QSqlQuery query( "INSERT INTO SR_GONE( ID, DATE ) "
                     "VALUES"
                     "( :id, :date )", db );
  
    query.bindValue( ":id", sr );
    query.bindValue( ":date", QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) );
    
    if ( !query.exec() )
    {
        qDebug() << query.lastError().text();
    }
}

void Database::delGoneSR( const QString& sr, const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "DELETE FROM SR_GONE WHERE ID = :id" );
    query.bindValue( ":id", sr );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
}

QStringList Database::getGoneSRs( const QString& dbname )
{
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    QStringList list;
    
    query.prepare( "SELECT ID FROM SR_GONE" );
    query.exec();
    
    while ( query.next() )
    {
        list.append( query.value( 0 ).toString() );
    }
    
    return list;
}

int Database::getGoneDays( const QString& sr, const QString& dbname )
{
    int days;
    QDateTime date;
    QSqlDatabase db = QSqlDatabase::database( dbname );
    QSqlQuery query( db );
    
    query.prepare( "SELECT DATE FROM SR_GONE WHERE ID = :id" );
    query.bindValue( ":id", sr );
    
    if ( !query.exec() ) 
    {
        qDebug() << query.lastError().text();
    }
    
    if ( query.next() )
    {
        date = QDateTime::fromString( query.value( 0 ).toString(), "yyyy-MM-dd hh:mm:ss" );
        days = date.daysTo( QDateTime::currentDateTime() );
    }
    
    return days;
}

QString Database::convertTime( const QString& dt )
{
    QDateTime d = QDateTime::fromString( dt, "M/d/yyyy h:mm:ss AP" );
    return ( d.toString("yyyy-MM-dd hh:mm:ss") );
}
