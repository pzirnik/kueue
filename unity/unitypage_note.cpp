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
    
    PLEASE NOTE:
    
    This is all very ugly and hacky, mostly due to the fact that there is no
    API and this has to be workarounded by parsing urls, titles, elements etc.
    
    If somebody knows a better way to achieve similar functionality, I'm
    open for suggestions :-)
*/

#include "unitypage.h"

void UnityPage::addNote()
{
    mNoteDialog = new NoteDialog( this, mCurrentSR, mIsCr );
     
    connect( mNoteDialog, SIGNAL( addNoteAccepted() ), 
             this, SLOT( addNoteAccepted() ) );
    
    connect( mNoteDialog, SIGNAL( rejected() ),
             this, SLOT( addNoteRejected() ) );
    
    mNoteDialog->exec();
}

void UnityPage::addNoteAccepted()
{
    if ( mIsCr )
    {
        emit pageErbert( "Adding Note to CR#" + mNoteDialog->sr() );
    }
    else
    {
        emit pageErbert( "Adding Note to SR#" + mNoteDialog->sr() );
    }
    
    if ( ( ( mViewFrame->findFirstElement( "title" ).toInnerXml() == "Service Request Activities" ) ||
           ( mViewFrame->findFirstElement( "title" ).toInnerXml() == "Service Request Related SRs" ) ) &&
           ( mCurrentSR == mNoteDialog->sr() ) )
    {
        mAddNote = true;
        newActivity();
    }
    else
    {
        mAddNote = true;
        querySR( mNoteDialog->sr() );
    }
}

void UnityPage::addNoteRejected()
{
    delete mNoteDialog;
}

void UnityPage::addNoteFirst()
{
    mNoJsConfirm = true;
    
    disconnect( mViewFrame, 0, 0, 0 );
    
    connect( mViewFrame, SIGNAL( loadFinished(bool) ),
             this, SLOT( addNoteSecond() ) );

    QString changeJS;
    QString id;
    QString id2;
    
    if ( mNoteDialog->noteType() == "Collaboration Request" && mViewFrame->findFirstElement( "title" ).toInnerXml() == "Service Request Related SRs") {
        //fake a CR we fix this back in step3
        mIsCr=true;
    }
    
    if ( mIsCr )
    {
        id = "s_5_2_23";
        id2 = "s_5_2_32";
    }
    else
    {
        id = "s_2_2_20";
        id2 = "s_2_2_32";
    }

    QWebElementCollection c = mViewFrame->findAllElements( "select" );
    QWebElement done;
    QWebElement e;
    
    for ( int i = 0; i < c.count(); ++i )
    {
        if ( c.at(i).attribute( "id" ).contains( id ) )
        {
            changeJS = c.at(i).attribute( "onchange" );
            
            QWebElementCollection d = c.at(i).findAll( "*" );
            
            for ( int i = 0; i < d.count(); ++i )
            {
                d.at(i).removeAttribute( "selected");
                
                if ( ( d.at( i ).attribute( "value" ) == "Public" ) && ( mNoteDialog->type() == NoteDialog::Public ) )
                {
                    e = d.at( i );
                }
                
                if ( ( d.at( i ).attribute( "value" ) == "Internal" ) && ( mNoteDialog->type() == NoteDialog::Internal ) )
                {
                    e = d.at( i );
                }
            }

            e.setAttribute( "selected", "Yes" );

            mViewFrame->evaluateJavaScript( changeJS );
        }
        
        else if ( c.at(i).attribute( "id" ).contains( id2 ) )
        {
            changeJS = c.at(i).attribute( "onchange" );
            QWebElementCollection d = c.at( i ).findAll( "*" );
            
            for ( int i = 0; i < d.count(); ++i )
            {
                d.at(i).removeAttribute( "selected" );
                
                if ( d.at( i ).attribute( "value" ) == mNoteDialog->noteType() )
                {
                    e = d.at( i );
                }
            }
        
            e.setAttribute( "selected", "Yes" );
            mViewFrame->evaluateJavaScript( changeJS );
        }
    }
    
    mAddNote = false;
    
    if ( mIsCr )
    {
        addNoteSecond();
    }
}

void UnityPage::addNoteSecond()
{
    mAddNote = true; 
    
    disconnect( mViewFrame, 0, 0, 0 );
    
    connect( mViewFrame, SIGNAL( loadFinished(bool) ),
             this, SLOT( addNoteThird() ) );

    QString changeJS;
    QString id; 
    
    if ( mIsCr )
    {
        id = "s_5_2_35";
    }
    else
    {
        id = "s_2_2_35";
    }
    
    QWebElementCollection c = mViewFrame->findAllElements( "select" );
    QWebElement e;
    
    for ( int i = 0; i < c.count(); ++i )
    {
        if ( c.at(i).attribute( "id" ).contains( id ) )
        {
            changeJS = c.at( i ).attribute( "onchange" );
            
            QWebElementCollection d = c.at(i).findAll( "*" );
            
            for ( int i = 0; i < d.count(); ++i )
            {
                d.at( i ).removeAttribute( "selected" );
                
                if ( d.at( i ).attribute( "value" ) == "Done" )
                {
                    e = d.at( i );
                }
            }
            
            e.setAttribute( "selected", "Yes" );
            mViewFrame->evaluateJavaScript( changeJS );
        }
    }
    
    mAddNote = false; 

    if ( mIsCr )
    {
        addNoteThird();
    }
}

void UnityPage::addNoteThird()
{
    mAddNote = true;
    mSaveAct = true;
    
    disconnect( mViewFrame, 0, 0, 0 );

    connect( mViewFrame, SIGNAL( loadFinished( bool ) ), 
             this, SLOT( actionDone() ) );
        
    QWebElementCollection rc = mViewFrame->findAllElements( "textarea" );
    
    for ( int i = 0; i < rc.count(); ++i ) 
    {  
        if ( ( ( rc.at(i).attribute( "id" ).contains( "s_2_2" ) ) && ( rc.at(i).attribute( "tabindex" ).contains( "2014" ) ) ) ||
             ( ( rc.at(i).attribute( "id" ).contains( "s_5_2_38" ) ) && ( rc.at(i).attribute( "tabindex" ).contains( "3997" ) ) ) )
        {
            rc.at(i).setInnerXml( mNoteDialog->comment() );
        }
        
        if ( ( ( rc.at(i).attribute( "id" ).contains( "s_2_2" ) ) && ( rc.at(i).attribute( "tabindex" ).contains( "2005" ) ) ) ||
             ( ( rc.at(i).attribute( "id" ).contains( "s_5_2_37" ) ) && ( rc.at(i).attribute( "tabindex" ).contains( "3997" ) ) ) )
        {
            rc.at(i).setInnerXml( mNoteDialog->briefDesc() );
        }
    }

    if ( mNoteDialog->noteType() == "Collaboration Request" && mViewFrame->findFirstElement( "title" ).toInnerXml() == "Service Request Related SRs") {
      // we missused the CR flag in step one, change it back.
      mIsCr==false;
    }
    delete mNoteDialog;
    mAddNote = false;
    actionDone();
}
