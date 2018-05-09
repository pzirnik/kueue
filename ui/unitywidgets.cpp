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

#include <QDebug>
#include <QMessageBox>

#include "unitywidgets.h"
#include "settings.h"

CalendarDialog::CalendarDialog( QObject* parent, QString sr )
{
    qDebug() << "[CALENDARDIALOG] Constructing";
    
    mSr = sr;
     
    setupUi( this ); 
       
    mCalWidget = new QCalendarWidget( this );
    mDateTimeEdit = new QDateTimeEdit( this );
    
    mDateTimeEdit->setDate( mCalWidget->selectedDate() );
    
    calendarLayout->addWidget( mCalWidget );
    calendarLayout->addWidget( mDateTimeEdit );

    setWindowTitle( "Schedule For Close" );
    textLabel->setText( "<b>Schedule For Close for SR#" + mSr + "</b>" );
    
    connect( mCalWidget, SIGNAL( selectionChanged() ),
             this, SLOT( calendarDateChanged() ) );
    
    connect( mDateTimeEdit, SIGNAL( dateChanged( QDate ) ),
             this, SLOT( widgetDateChanged() ) );
}

CalendarDialog::~CalendarDialog()
{
    qDebug() << "[CALENDARDIALOG] Destroying";
}

void CalendarDialog::calendarDateChanged()
{
    mDateTimeEdit->setDate( mCalWidget->selectedDate() );
}

void CalendarDialog::widgetDateChanged()
{
    mCalWidget->setSelectedDate( mDateTimeEdit->date() );
}

void CalendarDialog::closeEvent( QCloseEvent* event )
{
    emit rejected();
    QDialog::closeEvent( event );
}

/*
 * 
 * SsDialog
 * 
 */

SsDialog::SsDialog( QObject* parent, QString sr )
{
    qDebug() << "[SSDIALOG] Constructing";
    
    mSr = sr;
    
    setupUi( this ); 
    setWindowTitle( "Solution Suggested for SR#" + mSr );
}

SsDialog::~SsDialog()
{
    qDebug() << "[SSDIALOG] Destroying";
}

void SsDialog::closeEvent( QCloseEvent* event )
{
    emit rejected();
    QDialog::closeEvent( event );
}

/*
 * 
 * CloseDialog
 * 
 */

CloseDialog::CloseDialog( QObject* parent, const QString& sr )
{
    qDebug() << "[CLOSEDIALOG] Constructing";
    
    mSr = sr;
    
    setupUi( this ); 
    
    closeLabel->setText( "<b>Close SR#" + mSr + "?</b>" );
}

CloseDialog::~CloseDialog()
{
    qDebug() << "[CLOSEDIALOG] Destroying";
}

void CloseDialog::closeEvent( QCloseEvent* event )
{
    emit rejected();
    QDialog::closeEvent( event );
}

/*
 * 
 * NoteDialog
 * 
 */

NoteDialog::NoteDialog( QObject* parent, QString sr, bool isCr )
{
    qDebug() << "[NOTEDIALOG] Constructing";

    mSr = sr;
    setupUi( this );
    
    connect( this, SIGNAL( accepted() ), 
             this, SLOT( noteAccepted() ) );
    
    connect( checkBoxInt, SIGNAL( toggled( bool ) ),
             this, SLOT( changeCheckBoxExt( bool ) ) );
    
    connect( checkBoxExt, SIGNAL( toggled( bool ) ),
             this, SLOT( changeCheckBoxInt( bool ) ) );
    
    if ( isCr )
    {
        titleLabel->setText( "<b>Add a note to CR#" + mSr + "</b>" );
        noteTypeCombo->addItem( "Collaboration Response" );
    }
    else
    {
        titleLabel->setText( "<b>Add a note to SR#" + mSr + "</b>" );
        noteTypeCombo->addItem( "Collaboration Request" );
    }
}

NoteDialog::~NoteDialog()
{
    qDebug() << "[NOTEDIALOG] Destroying";
}

void NoteDialog::changeCheckBoxInt( bool checked )
{
    checkBoxInt->setChecked( !checked );
}

void NoteDialog::changeCheckBoxExt( bool checked )
{
    checkBoxExt->setChecked( !checked );
}

void NoteDialog::noteAccepted()
{
    if ( checkBoxInt->isChecked() )
    {
        mType = NoteDialog::Internal;
    }
    else
    {
        mType = NoteDialog::Public;
    }
    
    emit addNoteAccepted();
}

void NoteDialog::closeEvent( QCloseEvent* event )
{
    emit rejected();
    QDialog::closeEvent( event );
}

/*
 * 
 * PasswordPrompt
 * 
 */

PasswordPrompt::PasswordPrompt( QObject* parent )
{
    qDebug() << "[PASSWORDPROMPT] Constructing";
    
    setupUi( this ); 
    
    connect( this, SIGNAL( accepted() ), 
             this, SLOT( passwordEntered() ) );
}

PasswordPrompt::~PasswordPrompt()
{
    qDebug() << "[PASSWORDPROMPT] Destroying";
}

void PasswordPrompt::passwordEntered()
{
    Settings::setUnityPassword( passwordLine->text() );
    emit passwordReceived();
}

#include "unitywidgets.moc"
