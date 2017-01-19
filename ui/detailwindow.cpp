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

#include "detailwindow.h"
#include "data/database.h"
#include "data/datathread.h"
#include <data/data.h>
#include "config.h"
#include "kueue.h"
#include "settings.h"

#include <QtGui>
#include <QDesktopServices>
#include <QHttp>
#include <QtXml>

#ifdef QT_HAS_DBUS
    #include <QtDBus/QDBusConnection>
    #include <QtDBus/QDBusConnectionInterface>
#endif

DetailWindow::DetailWindow( QString id, bool nb )
{
    qDebug() << "[DETAILWINDOW] Constructing";
    
    setupUi( this ); 
        
    mSr = id;
    mIsCr = false;
    
    
        contactLabel->setVisible( false );
        contactLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        contactLabel1->setVisible( false );
        contactEmailLabel->setVisible( false );
        contactEmailLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        contactEmailLabel1->setVisible( false );
        contactPhoneLabel->setVisible( false );
        contactPhoneLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        contactPhoneLabel1->setVisible( false );
        ageLabel->setVisible( false );
        ageLabel1->setVisible( false );
        lastUpdateLabel->setVisible( false );
        lastUpdateLabel1->setVisible( false );
        contractLabel->setVisible( false );
        contactLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        contractLabel1->setVisible( false );
        hvLabel->setVisible( false );
        hvLabel1->setVisible( false );
        customerLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        briefDescLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        statusLabel->setTextInteractionFlags( Qt::TextSelectableByMouse );
        
    mDetailed = false;
    
    QShortcut* closesc = new QShortcut( Qt::Key_Escape, this );
    /* mTransSC = new QShortcut( Qt::Key_F1, this ); */
   
    connect( closesc, SIGNAL( activated() ),
             this, SLOT( close() ) );
    
    /* deactivate, because google kicks us away with a 301 moved reply,
     * also it is not very wise to send probably customer related data
     * to google .... even if they claim "do no evil"
     * connect( mTransSC, SIGNAL( activated() ),
             this, SLOT( translate() ) );*/
    
    connect( moreDetailButton, SIGNAL( clicked() ), 
             this, SLOT( toggleMoreDetails() ) );
    
    QueueSR sr = Database::getSrInfo( mSr );
    
    if ( !sr.id.isEmpty() )
    {
        mIsCr = sr.isCr;
        fillDetails( sr );
    }
    else
    {
        downloadDetails();
    }
    
    for ( int i = 0; i < Settings::engineerList().size(); ++i ) 
    {
        assignCombo->addItem( Settings::engineerList().at( i ) );
    }
   
    if ( ( nb ) || 
         ( !Settings::qbossFeatures() ) )
    {
        assignButton->setVisible( false );
        assignCombo->setVisible( false );
        assignLabel->setVisible( false );
    }
    else
    {
        assignButton->setVisible( true );
        assignCombo->setVisible( true );
        assignLabel->setVisible( true );
        
        QShortcut* assignsc = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_A ), this );
        
        connect( assignsc, SIGNAL( activated() ),
                 this, SLOT( assignSR() ) );
    }
    
    if ( ( !Settings::qbossFeatures() ) && ( !nb ) )
    {
        takeButton->setVisible( true );

        QShortcut* takesc = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_T ), this );
        
        connect( takesc, SIGNAL( activated() ),
                 this, SLOT( takePressed() ) );
    }
    else
    {
        takeButton->setVisible( false );
    }
    
    resize( Settings::detWinSize() );
    move( Settings::detWinPos() );
    restoreState( Settings::detWinState() );
    
    setWindowIcon( QIcon( ":/icons/kueue.png" ) );
    
    connect( closeButton, SIGNAL( clicked() ), 
             this, SLOT( close() ) );
    connect( assignButton, SIGNAL( clicked() ), 
             this, SLOT( assignSR() ) );
    connect( takeButton, SIGNAL( clicked() ),
             this, SLOT( takePressed() ) );
    
#ifdef IS_OSX
    closeButton->hide();
#endif
    
}

DetailWindow::~DetailWindow()
{
    qDebug() << "[DETAILWINDOW] Destroying";
    Settings::setDetWinPos( pos() );
    Settings::setDetWinState( saveState() );
    Settings::setDetWinSize( size() );
}

void DetailWindow::fillDetails( QueueSR sr )
{
    if ( sr.isCr )
    {
        srLabel->setText( "<font size='+1'><b>Details for CR#" + mSr + "</b></font>" );
    }
    else
    {
        srLabel->setText( "<font size='+1'><b>Details for SR#" + mSr + "</b></font>" );
    }
    
    briefDescLabel->setText( sr.bdesc );
    detailBrowser->setText( sr.ddesc );
        
        if ( sr.isCr )
        {
            customerLabel1->setText( "Created by:" );
            customerLabel->setText( sr.creator );
        }
        else
        {
            customerLabel->setText( sr.cus_account );
        }
        
        statusLabel->setText( sr.status );
  
        contactLabel->setText( sr.cus_firstname + " " + sr.cus_lastname + " (" + sr.cus_lang + ")" );
        
        QString mailto = "mailto:" + QUrl::toPercentEncoding( sr.cus_email ) + "?cc=techsup@novell.com&subject=SR" + QUrl::toPercentEncoding( " " + sr.id + " - " + sr.bdesc )
                         + "&body=" + QUrl::toPercentEncoding( sr.cus_firstname + " " + sr.cus_lastname );
                
        contactEmailLabel->setTextInteractionFlags( Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse );
        
        contactEmailLabel->setOpenExternalLinks( true );
        contactEmailLabel->setText( "<a href=" + mailto + ">" + sr.cus_email + "</a>" );
        contactPhoneLabel->setText( sr.cus_phone );
        
        ageLabel->setText( QString::number( sr.age ) + " days" );
        lastUpdateLabel->setText( QString::number( sr.lastUpdateDays ) + " days ago" );
        hvLabel->setText( "No / No" );
        contractLabel->setText( sr.contract );
}

void DetailWindow::closeEvent( QCloseEvent* e )
{
    e->ignore();
    emit done( this );
}

void DetailWindow::downloadDetails()
{
            QNetworkReply* r = Network::get( "srinfo/" + mSr );
        
        connect( r, SIGNAL( finished() ), 
                 this, SLOT( detailFinished() ) );
        
        
}

void DetailWindow::detailFinished()
{
    QNetworkReply* r = qobject_cast<QNetworkReply*>( sender() );
    QString det = r->readAll();
    QDateTime now = QDateTime::currentDateTime();
    
    if( r->error() )
    {
        QMessageBox::critical( this, "Error", "Downloading details for SR# " + mSr + " failed." );
        close();
    }
    else if( det.startsWith( "No SR") )
    {
        QMessageBox::critical( this, "Error", "Something went wrong while trying to fetch details for " + mSr );
        close();
    }
    else
    {
        QDomDocument doc;
        doc.setContent( det );
        QDomNode node = doc.documentElement();
        
        QueueSR sr;
            
        sr.id = node.namedItem( "id" ).toElement().text(); 
        sr.srtype = node.namedItem( "srtype" ).toElement().text(); 
        sr.creator = node.namedItem( "creator" ).toElement().text(); 
        sr.cus_account = node.namedItem( "cus_account" ).toElement().text(); 
        sr.cus_firstname = node.namedItem( "cus_firstname" ).toElement().text(); 
        sr.cus_lastname = node.namedItem( "cus_lastname" ).toElement().text(); 
        sr.cus_title = node.namedItem( "cus_title" ).toElement().text(); 
        sr.cus_email = node.namedItem( "cus_email" ).toElement().text(); 
        sr.cus_phone = node.namedItem( "cus_phone" ).toElement().text(); 
        sr.cus_onsitephone = node.namedItem( "cus_onsitephone" ).toElement().text(); 
        sr.cus_lang = node.namedItem( "cus_lang" ).toElement().text(); 
        sr.alt_contact = node.namedItem( "alt_contact" ).toElement().text();
        sr.bug = node.namedItem( "bug" ).toElement().text(); 
        sr.bugtitle = node.namedItem( "bug_desc" ).toElement().text(); 
        sr.severity = node.namedItem( "severity" ).toElement().text(); 
        sr.status = node.namedItem( "status" ).toElement().text(); 
        sr.bdesc = node.namedItem( "bdesc" ).toElement().text(); 
        sr.ddesc = node.namedItem( "ddesc" ).toElement().text(); 
        sr.geo = node.namedItem( "geo" ).toElement().text(); 
        sr.hours = node.namedItem( "hours" ).toElement().text(); 
        sr.contract = node.namedItem( "contract" ).toElement().text(); 
        sr.created = node.namedItem( "created" ).toElement().text(); 
        sr.lastupdate = node.namedItem( "lastupdate" ).toElement().text(); 
        sr.service_level = node.namedItem( "service_level" ).toElement().text().toInt();
        sr.highvalue = node.namedItem( "highvalue" ).toElement().text().toInt(); 
        sr.critsit = node.namedItem( "critsit" ).toElement().text().toInt();
        
        QDateTime a = QDateTime::fromString( sr.created, "yyyy-MM-dd hh:mm:ss" );
        QDateTime u = QDateTime::fromString( sr.lastupdate, "yyyy-MM-dd hh:mm:ss" );
        
        sr.age = a.daysTo( now );
        
        sr.lastUpdateDays = u.daysTo( now );
        if ( !sr.creator.isEmpty() )
        {
            mIsCr = true;
            sr.isCr = true;
        }
        
        fillDetails( sr );
    }
}

void DetailWindow::detail2Finished()
{
}

void DetailWindow::takePressed()
{
    mEngineer = Settings::engineer();
    assignNow();
}

void DetailWindow::translate()
{
    disconnect( mTransSC, 0, 0, 0 );
    
    QHttp* http = new QHttp(this);
    QString text = detailBrowser->toPlainText();
    http->setHost("www.google.com");
    
    connect(http, SIGNAL( done( bool ) ), 
            this, SLOT( transaction_done() ) );
    
    QString url = QString("/translate_a/t?client=j&sl=auto&tl=en" );
    
    QByteArray textByteArray("text=");
    textByteArray.append( detailBrowser->toPlainText().toUtf8() ); 
    
    QHttpRequestHeader header = QHttpRequestHeader("POST", url, 1, 1);
    
    header.setValue("Host", "www.google.com");
    header.setValue("User-Agent", "Mozilla/5.0");
    header.setValue("Accept-Encoding", "deflate");
    header.setContentLength(text.length());
    header.setValue("Connection", "Close");
    
    http->request(header,textByteArray);
}

void DetailWindow::transaction_done()
{
    QHttp* http = qobject_cast< QHttp* >( sender() );
    
    QString   mytext = http->readAll();
    
    QString   replyText = replyText.fromUtf8( http->readAll() ).split("[").at(1).split("]").at(0);
   
    replyText = replyText.replace(QString("\\\""),QString("\""));
    replyText = replyText.replace(QString("\\n"),QString("\n"));
    replyText = replyText.replace(QString("\n "),QString("\n"));
    replyText = replyText.replace(QString("\\x3c"),QString("<"));
    replyText = replyText.replace(QString("\\x3e"),QString(">"));
    replyText = replyText.replace(QString("\\u003c"),QString("<"));
    replyText = replyText.replace(QString("\\u003e"),QString(">"));

    QStringList translatedList;
 
    replyText = replyText.remove( "{" );
    translatedList = replyText.split(QString("}"));
    
    replyText.clear();
    
    for( int i=0; i < translatedList.count(); i++) 
    {
        replyText.append( translatedList.at(i).split( "\",\"" ).at(0) );
    }
    
    replyText = replyText.remove(",\"").remove( 0, 1 ).remove( "trans\":\"" );
    detailBrowser->setText( replyText );
}

void DetailWindow::assignSR() 
{
    if ( assignCombo->currentText().isEmpty() )
    {
        QMessageBox* box = new QMessageBox( this );
        box->setText( "Please select/enter an engineer" );
        box->setWindowTitle( "Assign SR - Error" );
        box->setStandardButtons(  QMessageBox::Ok );
        box->setIcon( QMessageBox::Critical );
    }
    else
    {
        mEngineer = assignCombo->currentText();
        assignNow();
    }
}

void DetailWindow::assignNow()
{
    QMessageBox* box = new QMessageBox( this );
    
    box->setText( "Assign <b>" + mSr + "</b> to " + mEngineer + "?" );
    box->setWindowTitle( "Assign SR" );
    box->setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    box->setDefaultButton( QMessageBox::Yes );
    box->setIcon( QMessageBox::Question );
    
    int reply = box->exec();
    
    if ( reply == QMessageBox::Yes )
    {
        showProgress( assignCombo->currentText().toUpper() );
        
        mAssign = Network::get( QString( "assign/" + mSr + "|" + mEngineer ), true );
        
        connect( mAssign, SIGNAL( finished() ), 
                 this, SLOT( assignJobDone() ) );
    }
    
    delete box;
}

void DetailWindow::assignJobDone()
{
    mProgress->close();
    
    if ( mAssign->error() )
    {
        QMessageBox::critical( this, "Error", "Error sending data to Server" );
        qDebug() << "[DETAILWINDOW] Assign error: " << mAssign->errorString();
    }
    
    QString data = mAssign->readAll();
    
    if ( data.startsWith( "ASSIGNED" ) )
        QMessageBox::critical( this, "Error", "SR already assigned to " + data.split( "|" ).at( 2 ) );

    else if ( data.startsWith( "SR_NOT_FOUND" ) )
        QMessageBox::critical( this, "Error", "SR not found");

    else if ( data.startsWith( "NO_SR" ) )
        QMessageBox::critical( this, "Error", "No SR number specified" );

    else if ( data.startsWith( "NO_OWNER" ) )
        QMessageBox::critical( this, "Error", "No assignee specified" );

    else if ( data.startsWith( "FAILURE" ) )
        QMessageBox::critical( this, "Error", "General failure" );

    else if ( data.startsWith( "SUCCESS" ) )
    {
        QMessageBox::information( this, "Done", "SR successfully assigned to " + mEngineer );
     
    }
    
    else QMessageBox::critical( this, "Error", "Unknown reply: " + data );
    
    DataThread::updateQmon();
}

void DetailWindow::showProgress( const QString& eng )
{
    mProgress = new QProgressDialog( this );
    mProgress->setWindowTitle( "Assigning SR" );
    mProgress->setLabelText( "Assigning SR to " + eng );
    mProgress->setMinimum( 0 );
    mProgress->setMaximum( 0 );
    mProgress->setWindowModality( Qt::WindowModal );
    mProgress->show();
}

void DetailWindow::toggleMoreDetails()
{
    if ( mDetailed )
    {
        contactLabel->setVisible( false );
        contactLabel1->setVisible( false );
        contactEmailLabel->setVisible( false );
        contactEmailLabel1->setVisible( false );
        contactPhoneLabel->setVisible( false );
        contactPhoneLabel1->setVisible( false );
        ageLabel->setVisible( false );
        ageLabel1->setVisible( false );
        lastUpdateLabel->setVisible( false );
        lastUpdateLabel1->setVisible( false );
        hvLabel->setVisible( false );
        hvLabel1->setVisible( false );
        contractLabel->setVisible( false );
        contractLabel1->setVisible( false );
        moreDetailButton->setIcon( QIcon( ":/icons/menus/add.png" ).pixmap( QSize( 16, 16 ) ) );
        mDetailed = false;
    }
    else
    {
        if ( !mIsCr )
        {
            
            contactLabel->setVisible( true );
            contactLabel1->setVisible( true );
            contactEmailLabel->setVisible( true );
            contactEmailLabel1->setVisible( true );
            contactPhoneLabel->setVisible( true );
            contactPhoneLabel1->setVisible( true );
            contractLabel->setVisible( true );
            contractLabel1->setVisible( true );
            hvLabel->setVisible( true );
            hvLabel1->setVisible( true );
        }
       
        ageLabel->setVisible( true );
        ageLabel1->setVisible( true );
        lastUpdateLabel->setVisible( true );
        lastUpdateLabel1->setVisible( true );
        moreDetailButton->setIcon( QIcon( ":/icons/menus/remove.png" ).pixmap( QSize( 16, 16 ) ) );
        mDetailed = true;
    }
}


#include "detailwindow.moc"
