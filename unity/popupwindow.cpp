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

#include "kueue.h"
#include "settings.h"
#include "popupwindow.h"
#include "webeditor.h"

#include <QDebug>
#include <QMenu>
#include <QWidgetAction>
#include <QWebFrame>
#include <QToolButton>
#include <QDesktopServices>
#include <QWebInspector>

PopupWindow::PopupWindow( QNetworkAccessManager* nam, QWidget* parent, bool shown, QString sr )
{
    qDebug() << "[POPUPWINDOW] Constructing";

    setWindowTitle( "kueue - Unity" );
    
    QGridLayout *l = new QGridLayout( this );
    setLayout( l );
    
    mWebView = new PopupWindowWebView( this, sr );
    QShortcut* wi = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_I ), this );
    
    connect( wi, SIGNAL( activated() ),
             this, SLOT( openWebInspector() ) );
    
    l->addWidget( mWebView );
    
    mWebView->page()->setNetworkAccessManager( nam );
    
    connect( mWebView->page(), SIGNAL( geometryChangeRequested( QRect ) ), 
             this, SLOT( resizeRequested( QRect ) ) );
    connect( mWebView->page(), SIGNAL( windowCloseRequested() ), 
             this, SLOT( closeWindow() ) );
    connect( mWebView->page(), SIGNAL( loadFinished( bool ) ), 
             this, SLOT( loadFinished() ) );

    connect( this, SIGNAL( rejected() ), 
             this, SLOT( closeWindow() ) );
    connect( this, SIGNAL( accepted() ),
             this, SLOT( closeWindow() ) );
    
    show();
    
    if ( !shown ) window()->hide();
}

PopupWindow::~PopupWindow()
{
    qDebug() << "[POPUPWINDOW] Destroying";
}

void PopupWindow::resizeRequested( const QRect& r )
{
    // first get the current geometry (we don't want to move the window around)
    
    QRect currentGeometry( geometry() );
    
    // unity does strange stuff here. since it seems to be required for netscape browsers, 
    // they resize the window 4x (!) .. 2x with +400 in height so I'm excluding that and 
    // every resizeRequest that has either 0 for either x or y.
    
    if ( !( ( r.x() == 0 ) || 
            ( r.y() == 0 ) || 
            ( r.height() == currentGeometry.height() + 400 ) ) )
    {
        // finally set the geometry to the saved x and y coordinates
        // but new width and height
        
        setGeometry( Settings::popupWindowX(), Settings::popupWindowY(), r.width(), r.height() );
    }
}

void PopupWindow::loadFinished()
{
    QString title = mWebView->page()->mainFrame()->findFirstElement( "title" ).toPlainText();
    setWindowTitle( title );
    qDebug() << "[POPUPWINDOW]" << title;
}

void PopupWindow::openWebInspector()
{
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );

    QWebInspector* i = new QWebInspector();
    i->setPage( mWebView->page() );
    i->setWindowTitle( "Webinspector - unitybrowser" );
    i->show();
}

void PopupWindow::closeWindow()
{
    // save the x and y coordinates so we can restore them next time 
    
    QRect newGeometry( geometry() );

    Settings::setPopupWindowX( newGeometry.x() );
    Settings::setPopupWindowY( newGeometry.y() );

    close();
    deleteLater();
}

void PopupWindow::moveEvent( QMoveEvent* event )
{
    QRect newGeometry( geometry() );

    if ( ( newGeometry.x() != 0 ) && ( newGeometry.y() != 0 ) )
    {
        Settings::setPopupWindowX( newGeometry.x() );
        Settings::setPopupWindowY( newGeometry.y() );
    }
        
    QWidget::moveEvent( event );
}

/*
 * 
 *      PopupWindowWebView class
 *
 */

PopupWindowWebView::PopupWindowWebView( QWidget* parent, QString sr )
    : QWebView( parent )
{
    qDebug() << "[POPUPWINDOWWEBVIEW] Constructing";
    
    mSr = sr;
    
    PopupWindowWebPage* p = new PopupWindowWebPage( this );
    setPage( p );
}

PopupWindowWebView::~PopupWindowWebView()
{
    qDebug() << "[POPUPWINDOWWEBVIEW] Destroying";
}

QWebElement PopupWindowWebView::getFocusedElement( const QPoint& pos )
{
    QWebHitTestResult res = page()->mainFrame()->hitTestContent( pos );
    return res.element();
}

QWebPage* PopupWindowWebView::newWindow()
{
    PopupWindow* p = new PopupWindow( page()->networkAccessManager(), this );
    return p->webPage();
}

void PopupWindowWebView::mousePressEvent( QMouseEvent* event )
{
    QString sr = mSr;
    
    if ( sr == QString::Null() )
    {
        sr = "kueue";
    }
    
    if ( event->button() == 2 )
    {
        QWebElement element = getFocusedElement( event->pos() );

        contextMenu( event, sr );
    }
      
    if ( ( event->button() == 4 ) && ( mUrl.toString().startsWith(  "sr://" ) ) )
    {
    }
    
    return QWebView::mouseMoveEvent( event );
}

void PopupWindowWebView::contextMenu( QMouseEvent* event, const QString& id )
{
    // Create a custom context menu plus a widget to hold the actual menu
    // This makes it look more like a KMenu (way more beautiful)
    
    QWebElement element = page()->mainFrame()->hitTestContent( event->pos() ).element();
    
    QMenu* menu = new QMenu();
            
    QAction* ba = new QAction( menu );
    QAction* ac = new QAction( "Open in external editor", menu );
    QAction* ac_f = new QAction( "Open in external editor (quoted)", menu );
    QAction* cb = new QAction( "Copy to clipboard", menu );
    QWidgetAction* wa = new QWidgetAction( menu );
    
    QFont font = ba->font();
    font.setBold( true );
    
    QToolButton* titleButton = new QToolButton( this );
    
    wa->setDefaultWidget( titleButton );
    wa->setObjectName( "MENUTITLE" );
    
    titleButton->setDefaultAction( ba );
    titleButton->setDown( true );
    titleButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    
    ba->setFont( font );
    ba->setIcon( QIcon( QIcon( ":/icons/kueue.png" ).pixmap( QSize( 16, 16 ) ) ) );
    ba->setText( id );
    
    ac->setData( QString::number( event->pos().x() ) + "||" + QString::number( event->pos().y() ) +
                 "||" + id + "|ed" );
    ac->setIcon( QIcon( ":/icons/menus/toggle.png" ) );
    
    ac_f->setData( QString::number( event->pos().x() ) + "||" + QString::number( event->pos().y() ) +
                 "||" + id + "|fe" );
    ac_f->setIcon( QIcon( ":/icons/menus/toggle.png" ) );

    cb->setData( selectedText() + "|cb" );
    cb->setIcon( QIcon( ":/icons/menus/clipboard.png" ) );
    
    menu->addAction( wa );
    
    if ( ( element.attribute( "type" ) == "input" ) ||
         ( element.attribute( "type" ) == "text" ) ||
         ( isTextArea( element ) ) )
    {
        menu->addAction( ac );
        menu->addAction( ac_f );
    }
    
    menu->addAction( cb );
    
    if ( selectedText().isEmpty() )
    {  
        cb->setEnabled( false );
    }
    
    connect( menu, SIGNAL( triggered( QAction* ) ), 
             this, SLOT( contextMenuItemTriggered( QAction* ) ) );
    
    menu->exec( event->globalPos() );
    
    delete menu;
}

bool PopupWindowWebView::isTextArea( QWebElement element )
{
    QWebElementCollection c = page()->mainFrame()->findAllElements( "textarea" );
    
    for ( int i = 0; i < c.count(); ++i )
    {
        if ( c.at( i ) == element )
        {
            return true;
        }
    }
    
    return false;
}

void PopupWindowWebView::contextMenuEvent(QContextMenuEvent* event)
{
    // Ignore the contextMenuEvent
    // This is necessary to make the right button events work
    
    event->ignore();
}

void PopupWindowWebView::contextMenuItemTriggered( QAction* a )
{   
    // This is called when a QAction from within the contextMenu is clicked
    // The data.contains thing is an ugly hack... But it works
    
    if ( a->data().toString().contains( "|ed" ) )
    {
        QString data = a->data().toString().remove("|ed");
   
        QPoint p;
        p.setX( data.split("||").at(0).toInt() );
        p.setY( data.split("||").at(1).toInt() );
        
        QWebHitTestResult res = page()->mainFrame()->hitTestContent( p );
        
        WebEditor* w = new WebEditor( res.element(), data.split("||").at( 2 ), false );
        
        connect( page(), SIGNAL( loadStarted() ),
                 w, SLOT( killYourself()) );
    }
    else if ( a->data().toString().contains( "|fe" ) )
    {
        QString data = a->data().toString().remove("|fe");
   
        QPoint p;
        p.setX( data.split("||").at(0).toInt() );
        p.setY( data.split("||").at(1).toInt() );
        
        QWebHitTestResult res = page()->mainFrame()->hitTestContent( p );
        
        WebEditor* w = new WebEditor( res.element(), data.split("||").at( 2 ), true );
        
        connect( page(), SIGNAL( loadStarted() ),
                 w, SLOT( killYourself()) );
    }
    else if ( a->data().toString().contains( "|cb" ) )
    {
        Kueue::setClipboard(  a->data().toString().remove( "|cb" ) );
    }
}

/*
 * 
 *         PopupWindowWebPage class
 * 
 */

PopupWindowWebPage::PopupWindowWebPage( QObject* parent ) : QWebPage( parent )
{
    qDebug() << "[POPUPWINDOWWEBPAGE] Constructing";
    
    settings()->setFontSize( QWebSettings::MinimumFontSize, Settings::minimumFontSize() );
    settings()->setFontSize( QWebSettings::DefaultFontSize, Settings::defaultFontSize() );
    settings()->setAttribute( QWebSettings::JavaEnabled, false );
    settings()->setAttribute( QWebSettings::JavascriptEnabled, true );
    settings()->setAttribute( QWebSettings::JavascriptCanOpenWindows, true );
    settings()->setAttribute( QWebSettings::JavascriptCanCloseWindows, true );

    // mEmailStage is used for controlling the email window 
    
    mStatusBar = &mStatusBar->getInstance();
    mEmailStage = 0;
    
    setForwardUnsupportedContent( true );
    setLinkDelegationPolicy( QWebPage::DelegateAllLinks );

    connect( mainFrame(), SIGNAL( loadFinished( bool ) ),
             this, SLOT( pageLoaded() ) );
    connect( this, SIGNAL( unsupportedContent( QNetworkReply* ) ),
             this, SLOT( handleUnsupportedContent(QNetworkReply*) ) );
    connect( this, SIGNAL( downloadRequested( QNetworkRequest ) ),
             this, SLOT( download(QNetworkRequest)));
}

PopupWindowWebPage::~PopupWindowWebPage()
{
    qDebug() << "[POPUPWINDOWWEBPAGE Destroying";
}

QWebPage* PopupWindowWebPage::createWindow( QWebPage::WebWindowType type )
{
    PopupWindowWebView* browser = qobject_cast< PopupWindowWebView* >( view() );
    QWebPage* webPage = qobject_cast< QWebPage* > ( browser->newWindow() );
    return webPage;
}

void PopupWindowWebPage::pageLoaded()
{
    if ( mainFrame()->url().toString().contains( "SWEMethod=*Server*Communications+Client+*FileSendMail*+*Email" ) &&
       ( mEmailStage == 0 ) )
    {
        // This determines that the popup window is showing an email window
        
        QString pickJS;
        mEmailStage = 1;
        
        // Find all <a> elements...
        
        QWebElementCollection c = mainFrame()->findAllElements( "a" );
    
        for ( int i = 0; i < c.count(); ++i )
        {
            // ...pick the one that holds the sr contact and get the javascript...
            
            if ( c.at(i).attribute( "href" ).contains( "VRId-1" ) )
            {
                pickJS = c.at( i ).attribute( "href" ).remove( "JavaScript:" );
            }
            
            // ...also, get the javascript for the OK button...
            
            if ( c.at( i ).toInnerXml().contains( "OK" ) )
            {
                okJS = c.at( i ).attribute( "href" ).remove( "JavaScript:" );
            }
        }
    
        // ...and finally send the javascript to select the default recipient
        // The OK button will be activated in the next stage
        
        mainFrame()->evaluateJavaScript( pickJS ); 
    }
    
    if ( mainFrame()->url().toString().contains( "https://siebelprd.innerweb.novell.com/callcentersi_enu/start.swe#SWEApplet" ) && 
       ( mEmailStage == 1 ) )
    {
        // Activate the OK button 
        
        if (!Settings::autopickRecipientDisabled()) {
          mainFrame()->evaluateJavaScript( okJS );
        }
        mEmailStage = 2;
    }    
    
    else if ( mainFrame()->url().toString().contains( "https://siebelprd.innerweb.novell.com/callcentersi_enu/start.swe#SWEApplet" ) && 
       ( mEmailStage == 2 ) )
    {    
        // First, reset the mEmailStage as we are in the final stage now
        
        mEmailStage = 0;
        //QString defaultJS;
        
        // Find all "select" and "option" elements on the email page
        
        /*QWebElementCollection c = mainFrame()->findAllElements( "select" );
        QWebElementCollection d = mainFrame()->findAllElements( "option" );
        
        for ( int i = 0; i < c.count(); ++i )
        {
            // Get the javascript for the onchange attribute of the combobox
            // This is required to set the default response 
            
            if ( c.at(i).attribute( "id" ).contains( "s_4_1_177_11" ) )
            {
                defaultJS = c.at(i).attribute( "onchange" );
            }
        }
        
        for ( int i = 0; i < d.count(); ++i )
        {
            // Set the combobox to the default response 
            
            if ( d.at( i ).attribute( "value" ).contains( Settings::defaultEmailTemplate() ) )
            {
                d.at( i ).setAttribute( "selected", "Yes" );
            }
        }
        
        // ... and execute the onchange javascript to make it active
        // The email window will now display the default response template
        
        mainFrame()->evaluateJavaScript( defaultJS );
	    } */
	}
}

static bool contentSniff( const QByteArray &data )
{
    if ( data.contains( "<!doctype" ) || 
         data.contains( "<script" ) || 
         data.contains( "<html" ) || 
         data.contains( "<!--" ) || 
         data.contains( "<head" )|| 
         data.contains( "<iframe" ) || 
         data.contains( "<h1" ) || 
         data.contains( "<div" ) || 
         data.contains( "<font" ) || 
         data.contains( "<table" ) || 
         data.contains( "<a" ) || 
         data.contains( "<style" ) || 
         data.contains( "<title" ) || 
         data.contains( "<b" ) || 
         data.contains( "<body" ) || 
         data.contains( "<br" ) || 
         data.contains( "<p" ) )
       {
            return true;
       }
        
    return false;
}

void PopupWindowWebPage::handleUnsupportedContent( QNetworkReply* reply )
{
    if ( !reply )
    {
        return;
    }

    QUrl replyUrl = reply->url();

    if ( replyUrl.scheme() == QLatin1String( "abp" ) )
    {
        return;
    }

    switch ( reply->error() ) 
    {
        case QNetworkReply::NoError:
        {
            if ( reply->header( QNetworkRequest::ContentTypeHeader ).isValid() ) 
            {
                QString ddir;
                
                if ( Settings::useSrDirectory() )
                {
                    mStatusBar->addDownloadJob( reply, Settings::downloadDirectory() + "/" + mCurrentSR, false);
                }
                else
                {
                    mStatusBar->addDownloadJob( reply, Settings::downloadDirectory(), true );
                }
                
                return;
            }
            
            break;
        }
        
        case QNetworkReply::ProtocolUnknownError: 
        {
            QSettings settings;
            settings.beginGroup( QLatin1String( "WebView" ) );
            QStringList externalSchemes = settings.value( QLatin1String( "externalSchemes" ) ).toStringList();
        
            if (externalSchemes.contains( replyUrl.scheme() ) ) 
            {
                QDesktopServices::openUrl( replyUrl );
                return;
            }
        
            break;
        }
    
        default:
        {
            break;
        }
    }

    QWebFrame *notFoundFrame = mainFrame();
    
    if ( !notFoundFrame )
    {
        return;
    }

    if ( reply->header( QNetworkRequest::ContentTypeHeader ).toString().isEmpty() ) 
    {
        QByteArray data = reply->readAll();
        
        if ( contentSniff( data ) ) 
        {
            notFoundFrame->setHtml( QLatin1String( data ), replyUrl );
            return;
        }
    }
}

void PopupWindowWebPage::download( const QNetworkRequest& req )
{
    if ( Settings::useSrDirectory() )
    {
        mStatusBar->addDownloadJob( req, networkAccessManager(), Settings::downloadDirectory() + "/" + mCurrentSR, false);
    }
    else
    {
        mStatusBar->addDownloadJob( req, networkAccessManager(), Settings::downloadDirectory(), true );
    }
}

#include "popupwindow.moc"
