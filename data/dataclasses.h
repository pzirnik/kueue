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

#ifndef DATACLASSES_H_
#define DATACLASSES_H_

#include <QStringList>

/* An SR in the personal queue */

class QueueSR
{
    public:
        QString id;
        QString srtype;
        QString creator;
        QString cus_account;
        QString cus_firstname;
        QString cus_lastname;
        QString cus_title;
        QString cus_email;
        QString cus_phone;
        QString cus_onsitephone;
        QString cus_lang;
        QString severity;
        QString status;
        QString bdesc;
        QString ddesc;
        QString geo;
        QString hours;
        QString contract;
        QString created;
        QString lastupdate;
        QString display;
        QString alt_contact;
        QString bug;
        QString bugtitle;
        QString owner;
        QString subowner;
        QString crsr;
        QStringList todoList;
        qint32 age;
        qint32 lastUpdateDays;
        qint32 service_level;
        bool subowned;
        bool highvalue;
        bool critsit;
        bool isCr;
        QString rating;
};

/* An SR in the queue monitor */

class QmonSR
{
    public:
        QString id;
        QString queue;
        QString bomgarQ;
        QString srtype;
        QString creator;
        QString cus_account;
        QString cus_firstname;
        QString cus_lastname;
        QString cus_title;
        QString cus_email;
        QString cus_phone;
        QString cus_onsitephone;
        QString cus_lang;
        QString severity;
        QString status;
        QString bdesc;
        QString ddesc;
        QString geo;
        QString hours;
        QString source;
        QString support_program;
        QString support_program_long;
        QString routing_product;
        QString support_group_routing;
        QString int_type;
        QString subtype;
        QString category;
        QString respond_via;
        QString created;
        QString lastupdate;
        QString queuedate;
        QString sla;
        QString display;
        QString alt_contact;
        QString bug;
        QString bugtitle;
        QString lupdate;
        QString cdate;
        QString crsr;
        QString subowner;
        qint32 agesec;
        qint32 timeinqsec;
        qint32 slasec;
        qint32 service_level;
        qint32 lastupdatesec;
        bool isCr;
        bool isChat;
        bool highvalue;
        bool critsit;
        bool hasLTSS;
        QString rating;
};

/* A customer satisfaction survey */

class Survey
{
    public:
        QString id;
        QString customer;
        QString bdesc;
        bool rts;
        int engsat;
        int srsat;
};

/* A closed SR */

class ClosedItem
{
    public:
        QString id;
        QString customer;
        QString bdesc;
        int tts;
};

/* Statistics */

class Statz
{
    public:
        int closedSr;
        int closedCr;
        int srTtsAvg;
        int csatEngAvg;
        int csatSrAvg;
        int csatRtsPercent;
        QList<Survey> surveyList;
        QList<ClosedItem> closedList;
};

/* The personal queue */

class PersonalQueue
{
    public:
        int total;
        int avgAge;
        QList<QueueSR> srList;
};

class QmonData
{
    public:
        int total;
        QList<QmonSR> srList;
};

class Notification
{
    public:
        QString type;
        QString title;
        QString body;
        QString sr;
};
    

#endif
