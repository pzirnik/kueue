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

#ifndef UNITYPAGE_H
#define UNITYPAGE_H

#include "unity/unitynetwork.h"
#include "ui/statusbar.h"
#include "ui/unitywidgets.h"
#include "kueueglobal.h"

#include <QWebPage>
#include <QWebElement>

class UnityPage : public QWebPage
{
    Q_OBJECT
     
    public:
        explicit UnityPage( QObject* parent, QString sr = QString::Null() );
        ~UnityPage();
        
        bool navReady() { return mNavReady; }
        bool loggedIn() { return mLoggedIn; }
        bool isNsaReport() { return mIsNsaReport; }
        bool isCr() { return mIsCr; }
        QWebHitTestResult getElementAt( const QPoint& );
        QString currentSr() { return mCurrentSR; }
        QWebFrame* viewFrame() { return mViewFrame; }
        bool isOwnerField( QWebElement );
        bool isSubownerField( QWebElement );

        
    public slots:
        void querySR( const QString& );
        void setElementText( QWebElement, const QString& );
        void sendEmail();
        void setSS( const QString& = "NONE" );
        void setSC( const QString& = "NONE" );
        void logout();
        void saveNsaReport();
        void handleUnsupportedContent(QNetworkReply*);
        void fillOutProduct( const QString&, const QString&, QString = QString::Null(), const QString& = QString::Null() );
        void saveCurrentSR();
        void addNote();
        void closeSr();
        void setStatus( const QString& );
        void goToService();
        void exportSr();
        void setOwner( const QString&, const QString& = QString::Null() );
        void setSubowner( const QString&, const QString& = QString::Null() );
        void busyWidgetCancelled();

    private slots:
        void pageLoaded();
        void loginToUnity();
        void loggedOut();
        void passwordReceived();
        void antiIdle();
        void addFrame( QWebFrame* );
        void viewFrameStarted();
        void fixQueryBox();
        void fixMenuBoxes();
        void goHome();
        void goHomeFirst();
        void doQuery();
        void goToActivities();
        void actionDone();
        void newActivity();
        void checkIfCR();
         
        void setSSconfirmed();
        void setSSrejected();
        void setSSfirst();
        void setSSsecond();
        void setSSthird();
        
        void unsetJsConfirm();
        void getServiceJS();
	void getEmailJS();
        void getCurrentSR();
        void selectionToClipboard();
        void download( const QNetworkRequest& );
        void fillOutProductNext();
        void saveCurrentActivity();
        
        void setScConfirmed();
        void setScRejected();
        void setScFirst();
        void setScSecond();
        void setScThird();
        void setScFourth();
        void setScFifth();
        void setScSixth();
        
        void closeSrAccepted();
        void closeSrRejected();
        void closeSrFirst();
        void closeSrSecond();
        void closeSrThird();
        
        void setOwnerJob();
        void setSubownerJob();
	
        void addNoteAccepted();
        void addNoteRejected();
        void addNoteFirst();
        void addNoteSecond();
        void addNoteThird();
        
        void setStatusFirst();
        void setStatusSecond();
        
    private:
        QWebPage* createWindow( QWebPage::WebWindowType type );
        QWebPage* mNewPage;
        
        QWebFrame* mViewFrame;
        QWebFrame* mBarFrame;
        QWebFrame* mMenuFrame;
        QWebFrame* mViewBarFrame;
        
        QWebElement mProdElement;
        
        StatusBar* mStatusBar;
        KueueGlobal* mKueueGlobal;
        
        NoteDialog* mNoteDialog;
        CloseDialog* mCloseDialog;
        CalendarDialog* mCalendarDialog;
        SsDialog* mSsDialog;
        
        QString mQuerySR;
        QString mStartSR;
        QString mServiceJS;
        QString mLogoutJS;
        QString mNewJS();
        QString mCurrentSR;
        QString mNewStatus;
        QString mFileName;
        QString mProduct;
        QString mComponent;
        QString mSaveJS;
	QString mEmailJS;
        QString mOwner;
        QString mSubowner;

        QTimer* mTimer;

        UnityNetwork* mNAM;
        
        bool mDontLogin;
        bool mNavReady;
        bool mLoggedIn;
        bool mIsNsaReport;
        bool mSetSS;
        bool mSetSC;
        bool mCloseSR;
        bool mSetOwner;
        bool mSetSubowner;
        bool mAddNote;
        bool mNoJsConfirm;
        bool mPageErbert;
        bool mSetStatus;
        bool mIsCr;
        bool checkMandatoryForClose();
        bool mSaveSr;
        bool mSaveAct;
        

    protected:
        void javaScriptAlert( QWebFrame*, const QString& );
        bool javaScriptConfirm( QWebFrame*, const QString& );

    signals:
        void loggedIn( bool );
        void loggedOutFromUnity();
        void contentChanged();
        void currentSrChanged( QString );
        void pageErbert();
        void pageErbert( QString );
        void pageErbertNed();
        void hideNextPopup();
};

#endif
