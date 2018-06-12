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

#ifndef UNITYBROWSER_H
#define UNITYBROWSER_H

#include "network.h"
#include "ui/kueuewindow.h"
#include "unity/unitypage.h"
#include "ui/browsers/search/webviewsearch.h"
#include "ui/busywidget.h"

#include <QWebView>
#include <QWebFrame>
#include <QDialog>
#include <QMenu>
#include <QToolBar>
#include <QShortcut>
#include <QToolButton>

class UnityBrowser : public QWebView
{
    Q_OBJECT
     
    public:
        explicit UnityBrowser( QWidget* parent = 0L, QString sr = QString::Null() );
        ~UnityBrowser();
        
        QString currentSR();
        bool isCR();
          
    public slots:
        void querySR( const QString& = QString::Null() );
        void historyBack();
        QWebPage* newWindow();
        void sendEmail();
        void saveSr();
        void solutionSuggested();
        void scheduleForClose();
        void addNote();
        void closeSr();
        void openFileBrowser();
        void goBackToSr();
        void setStatus( const QString& );
        void goToService();
        void setTabID( int );
        void busyWidgetCancelled();

    private slots:
        void createPage( const QString& );
        void disconnectShortcuts();
        void connectShortcuts();
        void pageErbert( const QString& = QString::Null() );
        void deleteWebInspector( QObject* );
        void pageErbertNed();
        void mousePressEvent( QMouseEvent* );
        void wheelEvent( QWheelEvent* );
        void urlHovered( const QString&, const QString& = 0, const QString& = 0 );
        void contextMenu( QMouseEvent*, const QString& );
        void contextMenuEvent( QContextMenuEvent * ev );
        void openSearch();
        void fillOutProduct();
        void openInBugzilla();
        void openTidInBrowser();
        void processFinished();
        void setPopupHidden();
        void applyZoom();
        void openWebEditor();
        void openWebEditorFormat();     
        void copyToClipboard();
        void saveImage();
        void openWebInspector();
        void linkClicked( const QUrl& );
        void loggedOut();
        void setOwner();
        void setSubowner();

    private:
        UnityPage* mUnityPage;
        QUrl mUrl;
        
        QMenu* productMenu( QMenu* parent );
        QAction* ownerAction( QString, QMenu* );
        QAction* subownerAction( QString, QMenu* );
        bool isTextArea( QWebElement );     
        bool isProductField( QWebElement );
        bool isBugzillaField( QWebElement );
        bool isEngineerField( QWebElement );
        bool isTidField( QWebElement );
        bool mShowPopup;
        
        int mCurrentZoom;
        int mTabID;
    
        QShortcut* mSendEmailSC;
        QShortcut* mSaveSrSC;
        QShortcut* mFileBrowserSC;
        QShortcut* mGoBackSC;
        QShortcut* mSsSC;
        QShortcut* mScSC;
        QShortcut* mAddNoteSC;
        QShortcut* mCloseSrSC;
        QShortcut* mLogOutSC;
        QShortcut* mWebInspectorSC;
        QShortcut* mExportSrSC;
        
    signals:
        void searchRequested( QString );
        void currentSrChanged( QString );
        void loggedIn( bool );
        void disableToolbar();
        void enableToolbar();
        void enableProgressIndicator( QString );
        void disableProgressIndicator();
};

class UnityWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit UnityWidget( QObject* parent = 0L, QString sr = QString::Null() );
        ~UnityWidget();
        
        UnityBrowser* browser() { return mUnityBrowser; }
        WebViewWithSearch* webViewWithSearch() { return mWebViewWithSearch; }
        int tabId() { return mTabId; }
        
    private:
        UnityBrowser* mUnityBrowser;
        WebViewWithSearch* mWebViewWithSearch;
        QToolBar* mToolBar;
        BusyWidget* mBusyWidget;
        int mTabId;
        bool mToolbarDisabled;
        QMenu* statusMenu();
        
        QLineEdit* mQueryLine;
        QToolButton* mSrButton;
        QToolButton* mSendEmailButton;
        QToolButton* mSaveSrButton;
        QToolButton* mFileBrowserButton;
        QToolButton* mGoBackButton;
        QToolButton* mSsButton;
        QToolButton* mScButton;
        QToolButton* mAddNoteButton;
        QToolButton* mCloseSrButton;
        QToolButton* mHomeButton;
        QToolButton* mQueryGoButton;
        QToolButton* mBackButton;
        QToolButton* mChangeStatusButton;
        QLabel* mQueryLabel;
        
    public slots:
        void currentSrChanged( QString );
        void setTabId( int );
        void activateProgressWidget( const QString& );
        void deactivateProgressWidget();
        
    private slots:
        void disableToolbar();
        void enableToolbar(); 
        void setToolbarButtonsEnabled( bool );
        void setOtherButtonsEnabled( bool );
        void querySR();
        void changeStatus( QAction* );
};

#endif
