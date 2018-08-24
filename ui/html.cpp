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

#include "html.h"
#include "settings.h"
#include "kueue.h"
#include "data/dataclasses.h"

QString HTML::styleSheet()
{
    return QString( "<head><style type='text/css'>\n"
                    "#Head1 {\n" 
                    "    position: absolute;\n"
                    "    z-index: 1; top: 0px;\n"
                    "    left: 0px; width: 50px;\n"
                    "    visibility: visible;\n"
                    "}"
                    
                    "#logo {\n"
                    "    position: absolute;\n"
                    "    z-index: 2;\n"
                    "    top: 0px;\n"
                    "    right: 5px;\n"
                    "    width: 175px;\n"
                    "    height: 80px;\n"
                    "    visibility: visible;\n"
                    "}\n"
                    
                    "#logo1 {\n"
                    "    position: absolute;\n"
                    "    z-index: 3;\n"
                    "    top: 0px;\n"
                    "    right: 0px;\n"
                    "    width: 12px;\n"
                    "    height: 80px;\n"
                    "    visibility: visible;\n"
                    "}\n"
                   
                    "#Apptitle {\n"
                    "    position: absolute;\n"
                    "    overflow:hidden;\n"
                    "    z-index: 1;\n"
                    "    top: 0px;\n"
                    "    left: 10px;\n"
                    "    right: 20px;\n"
                    "    height: 80px;\n"
                    "    visibility: visible;\n"
                    "}\n"
                    
                    "body {\n"
                    "    background: white url('qrc:/images/filler.gif') repeat-x 0% 0%;\n"
                    "}\n"
                    
                    ".apptitle1 {\n"
                    "    color: white;\n"
                    "    text-decoration: none;\n"
                    "    font-family: arial,helvetica,sans-serif;\n"
                    "    font-weight: normal;\n"
                    "    font-size: 1.7em;\n"
                    "    line-height: 2.7em;\n"
                    "    background: url('qrc:/images/bg.gif') repeat-x 0% 0%;\n"
                    "}\n"
                    
                    ".apptitle2 {\n"
                    "    color: white;\n"
                    "    font-family: arial,helvetica,sans-serif;\n"
                    "    text-decoration: none;\n"
                    "    font-weight: normal;\n"
                    "    font-size: 0.9em;\n"
                    "    line-height: 1.2em;\n"
                    "    right: 40px;\n"
                    "    background: url('qrc:/images/logo_im.gif') background-repeat:no-repeat;\n"
                    "}\n"
                    
                    "#abstand {\n"
                    "    margin-bottom:10px;\n"
                    "}\n"
                    
                    "#content {\n"
                    "    margin-top:90px;\n"
                    "    font-family: arial,helvetica,sans-serif;\n"
                    "}\n"
                    
                    "hr {\n"
                    "    background: url('qrc:/images/bg.gif') repeat-x 0% 0%;\n"
                    "    height: 1px;\n"
                    "    margin-top: 2px;\n"
                    "    margin-bottom: 10px;\n"
                    "}\n"
                    
                    "table {}\n"
                    
                    "td {}\n"
                    
                    ".qmonheader {\n"
                    "    background: url('qrc:/images/bg.gif') repeat-x 0% 0%;\n"
                    "    padding:5;\n"
                    "    margin:5;\n"
                    "    text-align: center;\n"
                    "    color: white;\n"
                    "    font-weight: bold;\n"
                    "    font-size: 1.2em;\n"
                    "    line-height: 1.2em;\n"
                    "    width: 100%;\n"
                    "    height: 55px;\n"
                    "}\n"
                    
                    "p {\n" 
                    "    margin-bottom:5px;\n"
                    "    margin-top:5px;\n"
                    "}\n"
                    
                    ".qph {\n"
                    "    font-family: arial,helvetica,sans-serif;\n"
                    "    font-weight: bold;\n"
                    "    font-size: 1.2em;\n"
                    "    margin-bottom: 1px;\n"
                    "}\n"
                    
                    "a {\n"
                    "    color:#333;\n"
                    "    text-decoration:none;\n"
                    "    -khtml-user-select: none;\n"
                    "}\n"
                    
                    ".dotlinehoriz {\n"
                    "    background-image: url('qrc:/images/dotlinebg_horiz.gif');\n"
                    "    height:1px;\n"
                    "    line-height:1px;\n"
                    "    overflow:hidden;\n"
                    "}\n"
                    
                    ".dotlinevert {\n"
                    "    background-image: url('qrc:/images/dotlinebg_vert.gif');\n"
                    "    width:1px;\n"
                    "}\n"
                    
                    ".gadgetHead {\n"
                    "    font:14px/18px arial,helvetica,sans-serif;\n"
                    "    margin-top:10px;\n"
                    "    margin-bottom:10px;\n"
                    "}\n"
                    
                    ".gadgetText {\n"
                    "    font:14px/24px arial,helvetica,sans-serif;\n"
                    "}\n" 
                  
                    "</style></head><body>\n" );
}

QString HTML::pageHeader( const QString& engineer, int total )
{
    QString ph;
    bool used;
    
    ph += QString( "<div id='Head1'>\n"
                   "    <img src='qrc:/images/gfx_top_in.gif' width='32' height='80' border='0'/>\n"
                   "</div>\n"
                   
                   "<div id='Apptitle' style='left:30px; right:60px; top:0px; height: 80px'>\n"
                   "    <div id='AppTitleDiv' class='apptitle1' style='padding-top: 3px; padding-bottom: 3px; width: 100%'>\n"
                   "        <b>" + engineer + "'s Queue</b>\n"
                   "    </div>\n"
                   "</div>\n"
                   
                   "<div id='logo' style='z-index:2'>\n"
                   "    <img src='qrc:/images/logo_im.gif' width='175' height='80' border='0'>\n"
                   "</div>\n"
                   
                   "<div id='Apptitle' style='left:20px; right:20px; top:0px; height: 80px; z-index: 4'>\n"
                   "    <div id='AppTitleDiv' class='apptitle2' style='text-align:center; padding-top: 21px; \n"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>\n"
		   "<b>Awaiting:</b>&nbsp;" );
    used = false;
    if (Settings::showAwaitingCustomer()) {
      ph += "Customer,&nbsp;";
      used = true;
    }
    if (Settings::showAwaitingSupport()) {
      ph += "Support,&nbsp;";
      used = true;
    }
    if (Settings::showStatusOthers()) {
      ph += "Others,&nbsp;";
      used = true;
    }
    if (used) {
      ph.resize(ph.size()-7);
    } else {
      ph += "All";
    }
    ph += QString( "&nbsp;&nbsp;<b>Type:</b>&nbsp;" );
    used = false;
    if (Settings::showSR()) {
      ph += "SRs,&nbsp;"; 
      used = true;
    }
    if (Settings::showCR()) {  
      ph += "CRs,&nbsp;";
      used = true;
    }
    if (used) {
      ph.resize(ph.size()-7);
    } else {
      ph += "No Filter set";
    }
    ph += QString( "</br><small>F1=Support | F2=Customer | F3=Others | F4=All | F5=Expand/Colapse | F6=ToogleSortType | F7=ToogleSortOrder</small></div></div>"
		   "<div id='Apptitle' style='left:20px; right:20px; top:0px; height: 80px; z-index: 4'>\n"
                   "    <div id='AppTitleDiv' class='apptitle2' style='text-align:right; padding-top: 21px; \n"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>\n"
                   "<b>Total Tickets:</b>&nbsp;" + QString::number( total ) + "<br><b>Sorted by:</b>&nbsp;\n" );
    if ( Settings::sortAge() )
    {
        ph += "age"; 
    }
    else 
    {    
        ph += "last activity";
    }

    ph += QString( "<br>\n"
                   "    </div>\n"
                   "</div>\n"
                   "<div id='logo' style='z-index:2'>\n"
                   "    <img src='qrc:/images/logo_im.gif' width='175' height='80' border='0'>\n"
                   "</div>\n"
                   "<div id='content'>\n" );
    
    return ph;
}

QString HTML::subPageHeader( int total )
{
    QString ph;
    bool used;
    
    ph += QString( "<div id='Head1'>\n"
                   "    <img src='qrc:/images/gfx_top_in.gif' width='32' height='80' border='0'/>\n"
                   "</div>\n"
                   "<div id='Apptitle' style='left:30px; right:60px; top:0px; height: 80px'>\n"
                   "    <div id='AppTitleDiv' class='apptitle1' style='padding-top: 3px; padding-bottom: 3px; width: 100%'>\n"
                   "        <b>Subowned SRs</b>\n"
                   "    </div>\n"
                   "</div>\n"
                   "<div id='logo' style='z-index:2'>\n"
                   "    <img src='qrc:/images/logo_im.gif' width='175' height='80' border='0'>\n"
                   "</div>\n"
                   "<div id='Apptitle' style='left:20px; right:20px; top:0px; height: 80px; z-index: 4'>\n"
                   "    <div id='AppTitleDiv' class='apptitle2' style='text-align:center; padding-top: 21px; \n"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>\n"
		   "<b>Awaiting:</b>&nbsp;" );
    used=false;
    if (Settings::subShowAwaitingCustomer()) {
      ph += "Customer,&nbsp;";
      used = true;
    }
    if (Settings::subShowAwaitingSupport()) {
      ph += "Support,&nbsp;";
      used = true;
    }
    if (Settings::subShowStatusOthers()) {
      ph += "Others,&nbsp;";
      used = true;
    }
    if (used) {
      ph.resize(ph.size()-7);
    } else {
      ph += "All";
    }
    ph += QString( "&nbsp;&nbsp;<b>Type:</b>&nbsp;" );
    used = false;
    if (Settings::subShowSR()) {
      ph += "SRs,&nbsp;";
      used=true;
    }
    if (Settings::subShowCR()) {  
      ph += "CRs,&nbsp;";
      used=true;
    }
    if (used) {
      ph.resize(ph.size()-7);
    } else {
      ph += "No Filter set";
    }
    ph += QString( "</br><small>F1=Support | F2=Customer | F3=Others | F4=All | F5=Expand/Colapse | F6=ToogleSortType | F7=ToogleSortOrder</small></div></div>"
		   "<div id='Apptitle' style='left:20px; right:20px; top:0px; height: 80px; z-index: 4'>\n"
                   "    <div id='AppTitleDiv' class='apptitle2' style='text-align:right; padding-top: 21px; \n"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>\n"
                   "<b>Total Tickets:</b>&nbsp;" + QString::number( total ) + "<br><b>Sorted by:</b>&nbsp;\n" );
    if ( Settings::subSortAge() )
    {
        ph += "age"; 
    }
    else 
    {    
        ph += "last activity";
    }

    ph += QString( "<br>\n"
                   "    </div>\n"
                   "</div>\n"
                   "<div id='logo' style='z-index:2'>\n"
                   "    <img src='qrc:/images/logo_im.gif' width='175' height='80' border='0'>\n"
                   "</div>\n"
                   "<div id='content'>\n" );
        
    return ph;
}

QString HTML::SRTable( QueueSR sr )
{
    QString srtab;
    QString t;
    QString bg = BgColor( sr.rating );

    if ( sr.id == "00000000000" )
    {
        return "Updating...\n";
    }
    
    if ( sr.srtype == "cr" )
    {
        sr.isCr = true;
    }
    else
    {
        sr.isCr = false;
    }
    
    srtab += QString (  "<table id='" + sr.id + "_head' width='100%' cellspacing='0' cellpadding='0' border='0'>\n"
                            "<tr>\n"
                                "<td colspan='3'>\n"
                                    "<div class='dotlinehoriz'>\n"
                                        "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''>\n"
                                    "</div>\n"
                                "</td>\n"
                            "</tr>\n"
                            "<tr>\n"
                                "<td width='1' rowspan='4' class='dotlinevert'>\n"
                                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''>\n"
                                "</td>\n"
                                "<td width='100%' bgcolor='" + bg + "'>\n"
                                    "<a href='sr://" + sr.id + "'><table width='100%' cellpadding='0' cellspacing='0' border='0'>\n"
                                        "<tr width='100%'>\n"
                                            "<td class='gadgetHead' valign='center' width='15%'><font" );

    if ( sr.severity ==  "Critical" )
    {
        srtab += ( " color='DarkRed'" );
    }
    else if ( sr.severity == "High" )
    {
        srtab += ( " color='DarkBlue'" );
    }
    
    if ( sr.isCr )
    {    
        srtab += QString("><p style='line-height:0.8em;'><b>&nbsp;CR# " + sr.id + "&nbsp;</b></font></p><p style='line-height:0.7em;'><font size='-1'><i>&nbsp;" + sr.status + "</i></p>\n" );
    }
    else 
    {    
        srtab += QString("><p style='line-height:0.8em;'><b>&nbsp;SR# " + sr.id + "&nbsp;</b></font>" );
        
        if ( ( sr.highvalue ) || ( sr.critsit ) )
        {
            srtab += QString( "<img src='qrc:/images/obacht.png'></img>" );
        }   
        
        srtab += QString( "</p><p style='line-height:0.7em;'><font size='-1'><i>&nbsp;" + sr.status + "</i></p>" );
    }
    
    srtab += QString( "</font></td><td class='gadgetText' width='60%'><p style='line-height:0.7em;'>" + sr.bdesc + "</p>"
                      "<p style='line-height:0.5em;'><b><i><font size='-2'>" );
    
    if ( sr.isCr )
    {
        srtab += sr.creator;
    }
    else
    {
        srtab += sr.cus_account;
    }
    
    QString uday;
    QString aday;
    
    if ( sr.lastUpdateDays == 1 )
    {
        uday = " day";
    }
    else
    {
        uday = " days";
    }
    
    if ( sr.age == 1 )
    {
        aday = " day";
    }
    else
    {
        aday = " days";
    }
    
    srtab += QString( "</font></p></i></b></td><td width='25%' align='right'><p style='line-height:0.6em;'><font size='-1'>"
                      "Age: " + QString::number( sr.age ) + aday + "&nbsp;</p><p style='line-height:0.6em;'>"
                      "Last update: " + QString::number( sr.lastUpdateDays ) + uday + " ago&nbsp;</p></td>" );
    
   
    srtab += QString (  "</td></a>"
                        "<td align='right'>"
                            "<a href='arrow://" + sr.id + "'><img src='qrc:/images/ni_gadget_arrow.gif'></img></a>"
                        "</td>"
                    "</tr>"
                "</table>"
            "</td>"
            "<td width='1' rowspan='4' class='dotlinevert'>"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>"
            "</td>"
        "</tr>"
        "<tr>"
            "<td colspan='3'>"
                "<div class='dotlinehoriz'>"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></img>"
                "</div>"
            "</td>"
        "</tr>"
    "</table>"
    "<a href='sr://" + sr.id + "'><table id='" + sr.id + "_body' width='100%' cellspacing='0' cellpadding='0' border='0' style='display:" );
  
    if( sr.display.isEmpty() ) 
    {   
        srtab+="none";
    }
    else if ( !sr.display.isEmpty() )
    {    
        srtab+=QString( sr.display );
    }
    else
    {
        srtab+="block";
    }
  
    srtab+=(    "'><tr>"
                "<td width='1' rowspan='4' class='dotlinevert'>"
                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>"
                "</td>"
                "<td width='100%'>"
                    "<table cellpadding='0' cellspacing='0' border='0' width='100%'>"
                                "<tr>"
                                    "<td class='gadgetText' width='15%'>&nbsp;Severity</td>"
                                    "<td class='gadgetText' width='85%'>" + sr.severity + "</td>"
                                "</tr>" );
                                
    if ( sr.isCr )
    {
        QString mailto = "mailto:" + QUrl::toPercentEncoding( sr.creator + "@suse.com" ) + "?cc=techsup@novell.com&subject=SR" + QUrl::toPercentEncoding( " " + sr.id + " - " + sr.bdesc )
        + "&body=" + QUrl::toPercentEncoding( sr.creator );
        
        srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Created by</td>"
                    "<td class='gadgetText'><a href=" + mailto + ">" + sr.creator + "</a></td>"
                 "</tr><tr>"
                    "<td class='gadgetText'>&nbsp;Original SR</td>"
                    "<td class='gadgetText'><a href='crsr://" + sr.crsr + "'>" + sr.crsr + "</a></td>"
                "</tr>" );
    }
    else
    {
        srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Customer</td>"
                    "<td class='gadgetText'>" + sr.cus_account + "</td>"
                "</tr>"
                "<tr>"
                    "<td class='gadgetText'>&nbsp;Contact</td>"
                    "<td class='gadgetText'>" + sr.cus_firstname + " " + sr.cus_lastname );
    
        if ( !sr.cus_title.isEmpty() )
        {
            srtab+=( " (" + sr.cus_title + ")" );
        }
        
        if ( !sr.cus_lang.isEmpty() )
        {
            srtab+=( " (" + sr.cus_lang + ")" );
        }
        
        srtab+=( "</td></tr>" );
        
        if (  !sr.cus_email.isEmpty() )
        {
            QString mailto = "mailto:" + QUrl::toPercentEncoding( sr.cus_email ) + "?cc=techsup@novell.com&subject=SR" + QUrl::toPercentEncoding( " " + sr.id + " - " + sr.bdesc )
                                    + "&body=" + QUrl::toPercentEncoding( sr.cus_firstname + " " + sr.cus_lastname );
            
            srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Email</td>"
                    "<td class='gadgetText'><a href=" + mailto + ">" + sr.cus_email + "</a></td>"
                    "</tr>" );
        }
        
        if (  !sr.cus_phone.isEmpty() )
        {
            srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Phone</td>"
                    "<td class='gadgetText'>" + sr.cus_phone + "</td>"
                    "</tr>" );
        }
        
        if (  !sr.cus_onsitephone.isEmpty() )
        {
            srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Onsite Phone</td>"
                    "<td class='gadgetText'>" + sr.cus_onsitephone + "</td>"
                    "</tr>" );
        }
        
        if (  !sr.alt_contact.isEmpty() )
        {
            srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Alternate contact</td>"
                    "<td class='gadgetText'>" + sr.alt_contact + "</td>"
                    "</tr>" );
        }
    }
    
    if ( !sr.bug.isEmpty() )
    {
        srtab+=( "<tr>"
                    "<td class='gadgetText'>&nbsp;Bug</td>"
                    "<td class='gadgetText'><a href='https://bugzilla.suse.com/show_bug.cgi?id=" + sr.bug + "' target='_blank'>" + 
                    sr.bug + " (" + sr.bugtitle + ")</a></td>"
                "</tr>" );
    }
    
    if ( sr.highvalue )
    {
        srtab += QString(  "<tr>"
                             "<td class='gadgetText'>&nbsp;High Value</td>"
                             "<td class='gadgetText'>Yes <img src='qrc:/images/obacht.png'></img></td>"
                           "</tr>" );
    }

    if ( sr.critsit )
    {
        srtab += QString( "<tr>"
                            "<td class='gadgetText'>&nbsp;CritSit</td>"
                            "<td class='gadgetText'>Yes <img src='qrc:/images/obacht.png'></img></td>"
                          "</tr>" );
    }
    
    srtab += QString( "<tr>"
                        "<td class='gadgetText'>&nbsp;Owner</td>"
                        "<td class='gadgetText'>" + sr.owner + "</td>"
                      "</tr>" );
    
    if ( !sr.subowner.isEmpty() )
    {
        srtab += QString( "<tr>"
                            "<td class='gadgetText'>&nbsp;Subowner</td>"
                            "<td class='gadgetText'>" + sr.subowner + "</td>"
                          "</tr>" );
    }
    
    if ( !sr.rating.isEmpty() )
    {
        srtab += QString( "<tr>"
                            "<td class='gadgetText'>&nbsp;Rating</td>"
                            "<td class='gadgetText'>" + sr.rating + "</td>"
                          "</tr>" );
    }
    
    srtab += (   "</tr></table></a></a></td><td width='1' rowspan='4' class='dotlinevert'><img src='qrc:/images/spacer.gif'"
                "width='1' height='1' border='0' alt=''></td></tr><tr><td colspan='3'><div class='dotlinehoriz'>"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></div></td></tr></table></a><div id='abstand'></div>" );  

    return srtab;
}

QString HTML::pageFooter() 
{
    QDateTime now = QDateTime::currentDateTime();
    
    return QString ( "<div id='footer'>Generated by kueue at " + now.toString( "yyyy-MM-dd hh:mm:ss" ) + "</div>" );
} 

QString HTML::qmonPageHeader()
{
    QString ph;
    
    ph += QString( "<div id='Head1'><img src='qrc:/images/gfx_top_in.gif' width='32' height='80' border='0'/></div>"
                   "<div id='Apptitle' style='left:30px; right:12px; top:0px; height: 80px'>"
                   "<a id='AppTitleA' style='text-decoration: none'>"
                   "<div id='AppTitleDiv' class='apptitle1' style='padding-top: 3px; padding-bottom: 3px; width: 100%'>"
                   "<b>Queue Monitor</b>"
                   "</div></div></a>"
		   "<div id='Apptitle' style='left:20px; right:20px; top:10px; height: 80px; z-index: 4'>"
                   "<div id='AppTitleDiv' class='apptitle2' style='text-align:center; padding-top: 21px;"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>"
                   "<small>F1 = Emea | F2 = Usa F3 = Asiapac | F4 = All</small>"
		   "</div></div>"
		   "<div id='Apptitle' style='left:20px; right:20px; top:10px; height: 80px; z-index: 4'>"
                   "<div id='AppTitleDiv' class='apptitle2' style='text-align:right; padding-top: 21px;"
                   " padding-bottom: 3px; width: 100%; z-index: 3'>"
                   "<b>Active:</b>&nbsp;" + Settings::qmonFilter() + 
                   "</div>"
                   "</div>"
                   "<div id='logo1' style='z-index:3'>"
                   "<img src='qrc:/images/logo_im_end.gif' width='12' height='80' border='0'></div>"
                   "<div id='content'>" );

    return ph;
}

QString HTML::qmonTableHeader( const QString& queue, int amount )
{
    QString ph;
 
    ph += QString ( "<p class='qph'><b>" + queue + " (" + QString::number( amount ) + ")</b></p><hr>" );

    return ph;
}

QString HTML::qmonSrInQueue( QmonSR sr )
{
    QString srtab;
    QString bg = BgColor( sr.rating );
  
    if ( sr.id == "00000000000" )
    {
        return "Updating...";
    }
        
    srtab += QString (  "<table id='" + sr.id + "_head' width='100%' cellspacing='0' cellpadding='0' border='0'>\n"
                            "<tr>\n"
                                "<td colspan='3'>\n"
                                    "<div class='dotlinehoriz'>\n"
                                        "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''>\n"
                                    "</div>\n"
                                "</td>\n"
                            "</tr>\n"
                            "<tr>\n"
                                "<td width='1' rowspan='4' class='dotlinevert'>\n"
                                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''>\n"
                                "</td>\n"
                                "<td width='100%' bgcolor='" + bg + "'>\n"
                                    "<a href='sr://" + sr.id + "'><table width='100%' cellpadding='0' cellspacing='0' border='0'>\n"
                                        "<tr width='100%'>\n"
                                            "<td class='gadgetHead' valign='center' width='18%'>\n" );

    if (  sr.isCr )
    {
        srtab += QString ( "<p style='line-height:0.8em;'><b>&nbsp;CR# " );
    }
    else
    {
        srtab += QString ( "<p style='line-height:0.8em;'><b>&nbsp;SR# " );
    }
    
    srtab += QString( sr.id + "</b>" );
    
    if ( sr.isChat )
    {
        srtab += QString( "&nbsp;<img src='qrc:/images/chat.png'></img>" );
    }    
    
    if ( ( sr.highvalue ) || ( sr.critsit ) )
    {
        srtab += QString( "&nbsp;<img src='qrc:/images/obacht.png'></img>" );
    }   
   
    if ( sr.hasLTSS )
    {
        srtab += QString( "&nbsp;<img src='qrc:/images/ltss.png'></img>" );
    }
 
    srtab += QString( "</p><b><font size='-1'><p style='line-height:0.7em;'>&nbsp;" + sr.geo + " (" + sr.hours + ") - " );
    
    if ( sr.severity == "Critical" )
    {
        srtab += QString( "<font color='DarkRed'>Critical</font>" );
    }
    else if ( sr.severity == "High" )
    {
        srtab += QString( "<font color='DarkBlue'>High</font>" );
    }
    else
    {
        srtab += QString( sr.severity );
    }

    srtab += QString( "</b></font></p><p style='line-height:0.7em;'><font size='-1'><i>&nbsp;" + sr.status + "</i></font></p></td><td width='52%' valign='center' align='left' class='gadgetHead'>\n" );
   
    if ( sr.cus_account.isEmpty() )
    {
        sr.cus_account = sr.creator;
    }
    
    srtab += QString (  "<p style='line-height:0.7em;'>" +  sr.bdesc.trimmed() + "</p>"
                            "<p style='line-height:0.5em;'><font size='-2'><b><i>" + sr.cus_account.trimmed() + "</b></i></font></p>\n"
                        "</td></a>\n"
                        "<td width='30%' align='right' valign='center' class='gadgetHead'><font size='-1'>\n" 
                        "<p style='line-height:0.7em;'>Age: " + timeString( sr.agesec ) + "</p>\n" );
    
    srtab += QString( "<p style='line-height:0.7em;'>In Queue: " + timeString( sr.timeinqsec ) );
                        
    if ( sr.slasec > 0 )
    {
        srtab += QString( "</p><p style='line-height:0.7em;'>SLA left: " + timeString( sr.slasec ) );
    }
                                   
    srtab += ( "</p></font></td><td align='right'>\n"
                 "<a href='arrow://" + sr.id + "'><img src='qrc:/images/ni_gadget_arrow.gif'></img></a><br>\n"
                "</td>\n"
                "</tr>\n"
                "</table>\n"
                "</td>\n"
                  "<td width='1' rowspan='4' class='dotlinevert'>\n"
                   "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>\n"
                "</td>\n"
              "</tr>\n"
               "<tr>\n"
                 "<td colspan='3'><div class='dotlinehoriz'>\n"
                   "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></img>\n"
                   "</div>\n"
                 "</td>\n"
              "</tr>\n"
           "</table>\n" );
      
   srtab += (
    "<a href='sr://" + sr.id + "'><table id='" + sr.id + "_body' width='100%' cellspacing='0' cellpadding='0' border='0' style='display:" );
  
    if ( sr.display.isEmpty() )
    {
        srtab += "none";
    }
    else
    {
        srtab += sr.display;
    }
  
    srtab+=(    "'>\n"
                "<tr>\n"
                "<td width='1' rowspan='4' class='dotlinevert'>\n"
                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>\n"
                "</td>\n"
                "<td width='100%'>\n"
                    "<table cellpadding='0' cellspacing='0' border='0' width='100%'>\n" );

    if ( !sr.isCr )
    {
        srtab += QString( "<tr><td class='gadgetText' width='18%'>&nbsp;Contact</td>\n"
                          "<td class='gadgetText' width='82%'>" + sr.cus_firstname.trimmed() + " " + sr.cus_lastname.trimmed() );
               
        
        if ( !sr.cus_title.isEmpty() )
        {
            srtab += QString( + " (" + sr.cus_title.trimmed() + ")" );
        }
        
        if ( !sr.cus_lang.isEmpty() )
        {
            srtab += QString( + " (" + sr.cus_lang.trimmed() + ")" );
        }
        
        srtab += "</tr>";
        
        if (  !sr.cus_email.isEmpty() )
        {
            QString mailto = "mailto:" + QUrl::toPercentEncoding( sr.cus_email ) + "?cc=techsup@novell.com&subject=SR" + QUrl::toPercentEncoding( " " + sr.id + " - " + sr.bdesc )
                                    + "&body=" + QUrl::toPercentEncoding( sr.cus_firstname + " " + sr.cus_lastname );
            
            srtab+=( "<tr>\n"
                      "<td class='gadgetText'>&nbsp;Email</td>\n"
                      "<td class='gadgetText'><a href=" + mailto + ">" + sr.cus_email + "</a></td>\n"
                    "</tr>\n" );
        }
        
        if (  !sr.cus_phone.isEmpty() )
        {
            srtab+=( "<tr>\n"
                      "<td class='gadgetText'>&nbsp;Phone</td>\n"
                      "<td class='gadgetText'>" + sr.cus_phone + "</td>\n"
                    "</tr>\n" );
        }
        
        if (  !sr.cus_onsitephone.isEmpty() )
        {
            srtab+=( "<tr>\n"
                      "<td class='gadgetText'>&nbsp;Onsite Phone</td>\n"
                      "<td class='gadgetText'>" + sr.cus_onsitephone + "</td>\n"
                    "</tr>\n" );
        }
        
        if ( !sr.respond_via.isEmpty() )
        {
            srtab += QString( "<tr>\n"
                                "<td class='gadgetText'>&nbsp;Contact via</td>\n"
                                "<td class='gadgetText'>" + sr.respond_via + "</td>\n"
                              "</tr>\n" );
        }
    }
    else
    {
        srtab += QString( "<tr><td class='gadgetText' width='18%'>&nbsp;Created by</td><td class='gadgetText' width='82%'>" + sr.creator.trimmed() );
    }
        
    srtab += QString( "</td></tr><tr>\n"
                        "<td class='gadgetText'>&nbsp;Status</td>\n"
                        "<td class='gadgetText'>" + sr.status + "</td>\n"
                      "</tr>\n" );
        
    if ( sr.isChat )
    {
        srtab += QString( "<tr>\n"
                            "<td class='gadgetText'>&nbsp;Bomgar Queue/Owner</td>\n"
                            "<td class='gadgetText'>" + sr.bomgarQ + "</td>\n"
                          "</tr>\n" );
    }
  
    if ( !sr.isCr )
    {
        srtab += QString( "<tr>\n"
                            "<td class='gadgetText'>&nbsp;Contract</td>\n"
                            "<td class='gadgetText'>" + sr.support_program_long + "</td>\n"
                          "</tr>" );
    }
    
    QString uday;
    
    if ( secDays( sr.lastupdatesec ) == 1 )
    {
        uday = " day";
    }
    else
    {
        uday = " days";
    }
      
    
    srtab += QString(  "<tr>\n"
                         "<td class='gadgetText'>&nbsp;Last Update</td>\n"
                         "<td class='gadgetText'>" + QString::number( secDays( sr.lastupdatesec ) ) + uday + " ago</td>\n"
                       "</tr>\n" );
    
    if ( sr.highvalue )
    {
        srtab += QString( "<tr>\n"
                            "<td class='gadgetText'>&nbsp;High Value</td>\n"
                            "<td class='gadgetText'>Yes <img src='qrc:/images/obacht.png'></img></td>\n"
                          "</tr>\n" );
    }

    if ( sr.critsit )
    {
        srtab += QString( "<tr>\n"
                            "<td class='gadgetText'>&nbsp;CritSit</td>\n"
                            "<td class='gadgetText'>Yes <img src='qrc:/images/obacht.png'></img></td>\n"
                          "</tr>\n" );
    }
    
    if ( sr.hasLTSS )
    {
        srtab += QString( "<tr>\n"
                            "<td class='gadgetText'>&nbsp;LTSS</td>\n"
                            "<td class='gadgetText'>Yes <img src='qrc:/images/ltss.png'></img></td>\n"
                          "</tr>\n" );
    }
    
    if ( !sr.rating.isEmpty() )
    {
        srtab += QString( "<tr>"
                            "<td class='gadgetText'>&nbsp;Rating</td>"
                            "<td class='gadgetText'>" + sr.rating + "</td>"
                          "</tr>" );
    }

    srtab +=( "</td></tr>\n" );
  
    srtab+= (   "</table></a></a></td><td width='1' rowspan='4' class='dotlinevert'><img src='qrc:/images/spacer.gif'"
                "width='1' height='1' border='0' alt=''></td></tr><tr><td colspan='3'><div class='dotlinehoriz'>\n"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></div></td></tr></table><div id='abstand'></div></a>\n" );  
  
    return srtab;
}

QString HTML::qmonTableFooter()
{
    return ( "</div></table>" );  
}

QString HTML::statsPageHeader( Statz s )
{
    QString ph;
    QList<Survey> sl = s.surveyList;
    QList<ClosedItem> cl = s.closedList;
    int nr = sl.size();   
 
    ph += styleSheet();
    
    ph += QString( "<div id='Head1'><img src='qrc:/images/gfx_top_in.gif' width='32' height='80' border='0'/></div>"
                   "<div id='Apptitle' style='left:30px; right:12px; top:0px; height: 80px'>"
                   "<a id='AppTitleA' style='text-decoration: none'>"
                   "<div id='AppTitleDiv' class='apptitle1' style='padding-top: 3px; padding-bottom: 3px; width: 100%'>"
                   "<b>Quarterly Statistics</b>"
                   "</div></div></a>"
                   "<div id='logo1' style='z-index:3'>"
                   "<img src='qrc:/images/logo_im_end.gif' width='12' height='80' border='0'></div></div>"
                   "<div id='content'>" );
    
    if ( nr > 0 )
    {
        ph += csatTableHeader( s.csatRtsPercent, s.csatEngAvg, s.csatSrAvg, nr );
    }
    else
    {
        ph += "No cached data available. Check again in 30 minutes ....";
    }
    
    for ( int i = 0; i < sl.size(); ++i ) 
    {   
        ph += csatTable( sl.at( i ) );
    }
    
    if ( nr > 0 )
    {
        ph += closedTableHeader( s.closedSr, s.closedCr, s.srTtsAvg );
    }
    
    for ( int i = 0; i < cl.size(); ++i ) 
    {   
        ph += closedTable( cl.at( i ) );
    }
    
    return ph;
}

QString HTML::csatTableHeader( int rts, int eng, int sr, int nr )
{
    QString ph;
    
    if ( ( rts == 0 ) && 
         ( eng == 0 ) && 
         ( sr == 0 ) )
    {
        ph += QString ( "<p class='qph'><b>Customer Satisfaction</b></p><hr>" 
                        "No data available at present<br><br>" );
    }
    else
    {
        ph += QString ( "<p class='qph'><b>Customer Satisfaction</b></p><hr>"
                        "Surveys received: " + QString::number( nr ) + "<br>"
                        "Resolved to Satisfaction: " + QString::number( rts ) + "%<br>"
                        "Average SR Satisfaction: " + QString::number( sr ) + "<br>"
                        "Average Engineer Satisfaction: " + QString::number( eng ) + "<br><br>" );
    }
    
    return ph;
}

QString HTML::csatTable( Survey i )
{
    QString srtab;
    
    srtab += QString (  "<table id='" + i.id + "_head' width='100%' cellspacing='0' cellpadding='0' border='0'>"
                            "<tr>"
                                "<td colspan='3'>"
                                    "<div class='dotlinehoriz'>"
                                        "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''>"
                                    "</div>"
                                "</td>"
                            "</tr>"
                            "<tr>"
                                "<td width='1' rowspan='4' class='dotlinevert'>"
                                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''>"
                                "</td>"
                                "<td width='100%' bgcolor='#EDEEEC'>"
                                    "<a href='sr://" + i.id + "'><table width='100%' cellpadding='0' cellspacing='0' border='0'>"
                                        "<tr width='100%'><td class='gadgetHead'>" );
    
    if ( i.rts ) 
    {
        srtab += QString( "<img src='qrc:/images/yay.gif'></img> " );
    }
    else
    {
        srtab += QString( "<img src='qrc:/images/sad.gif'></img> " );
    }
    
    srtab += QString( "<b>SR#" + i.id + "</b> - " + i.bdesc );
                                            
    srtab += QString( "</td></a>"
                      "<td align='right'>"
                                    "<a href='arrow://" + i.id + "'><img src='qrc:/images/ni_gadget_arrow.gif'></img></a><br>"
                                "</td>"
                            "</tr>"
                        "</table>"
                    "</td>"
                    "<td width='1' rowspan='4' class='dotlinevert'>"
                        "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td colspan='3'>"
                        "<div class='dotlinehoriz'>"
                            "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></img>"
                        "</div>"
                    "</td>"
                "</tr>"
            "</table>"
            "<a href='sr://" + i.id + "'><table id='" + i.id + "_body' width='100%' cellspacing='0' cellpadding='0' "
            "border='0' style='display:none'><tr>"
                  "<td width='1' rowspan='4' class='dotlinevert'>"
                       "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''></img>"
                  "</td>"
                  "<td width='100%'>"
                        "<table cellpadding='0' cellspacing='0' border='0'>"
                            "<tr>"
                                "<td class='gadgetText'>"
                                "<tr>"
                                    "<td class='gadgetText'>Customer</td>"
                                    "<td class='gadgetText'>" + i.customer + "</td>"
                                "</tr>"
                                "<tr>"
                                    "<td class='gadgetText'>Resolved to Satisfaction&nbsp;&nbsp;&nbsp;&nbsp;</td>" );
                                    
    if ( i.rts )
    {
        srtab += QString( "<td class='gadgetText'>Yes</td>" );
    }
    else
    {
        srtab += QString( "<td class='gadgetText'>No</td>" );
    }
    
    srtab += QString( "</tr>" );
    
    if ( i.engsat != -1 )
    {
        srtab += QString ( "<tr>"
                             "<td class='gadgetText'>Engineer Satisfaction</td>"
                             "<td class='gadgetText'>" + QString::number( i.engsat ) + "</td>"
                           "</tr>" );
    }

    srtab +=( "</td></tr>" );
  
    srtab+= (   "</table></a></a></td><td width='1' rowspan='4' class='dotlinevert'><img src='qrc:/images/spacer.gif'"
                "width='1' height='1' border='0' alt=''></td></tr><tr><td colspan='3'><div class='dotlinehoriz'>"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></div></td></tr></table>" );  
    
    srtab += QString( "<div id='abstand'></div>" );
  
    return srtab;
}

QString HTML::closedTableHeader( int c, int b, int a )
{
    QString ph;
 
    ph += QString ( "<br><p class='qph'><b>Closed SRs/Time to solution</b></p><hr>"
                    "You closed " + QString::number( c ) + " SRs and " + QString::number( b ) + " CRs so far<br>"
                    "Average time to solution (SRs only): " + QString::number( a ) + " days<br><br>" 
                    "Here is a list of closed SRs:<br><br>" );

    return ph;
}

QString HTML::closedTable( ClosedItem ci )
{
    QString srtab;
    
    srtab += QString (  "<table id='" + ci.id + "_head' width='100%' cellspacing='0' cellpadding='0' border='0'>"
                            "<tr>"
                                "<td colspan='3'>"
                                    "<div class='dotlinehoriz'>"
                                        "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''>"
                                    "</div>"
                                "</td>"
                            "</tr>"
                            "<tr>"
                                "<td width='1' rowspan='4' class='dotlinevert'>"
                                    "<img src='qrc:/images/spacer.gif' width='1' height='1' border='0' alt=''>"
                                "</td>"
                                "<td width='100%' bgcolor='#EDEEEC'>"
                                    "<a href='sr://" + ci.id + "'><table width='100%' cellpadding='0' cellspacing='0' border='0'>"
                                        "<tr width='100%'><td class='gadgetHead'><b>SR#" + ci.id + "</b> - " + 
                                        QString::number( ci.tts ) + " days - " + ci.bdesc );
    

      
    srtab+= (   "</table></a></a></td><td width='1' rowspan='4' class='dotlinevert'><img src='qrc:/images/spacer.gif'"
                "width='1' height='1' border='0' alt=''></td></tr><tr><td colspan='3'><div class='dotlinehoriz'>"
                "<img src='qrc:/images/spacer.gif' width='1' height='1' alt=''></div></td></tr></table>" );  
    
    srtab += QString( "<div id='abstand'></div>" );
                    
    return srtab;
}

QString HTML::nsaPageHeader( NSASummaryItem summary )
{
    QString out;
    
    out +=( "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>"
            "<html xmlns='http://www.w3.org/1999/xhtml'><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8' />"
            "<title>NSA Report</title><style type='text/css'>"

            "body {"
                    "background: #666666;"
                    "margin: 0;"
                    "padding: 0;"
                    "text-align: center;"
                    "color: #000000;"
                    "font-family: Verdana, Arial, Helvetica, sans-serif;"
                    "font-size: 12px;"
            "}"

            ".oneColFixCtrHdr #container {"
                    "width: 1000px;"
                    "background: #FFFFFF;"
                    "border: 1px solid #000000;"
                    "text-align: left;"
                    "padding: 0px;"
                    "margin-top: 0;"
                    "margin-right: auto;"
                    "margin-bottom: 0;"
                    "margin-left: auto;"
            "}"
            
            ".oneColFixCtrHdr #header {"
                    "font-size: 18px;"
            "}"
            
            ".oneColFixCtrHdr #header h1 {"
                    "margin: 0;"
                    "padding: 10px 0;"
            "}"
            
            ".oneColFixCtrHdr #mainContent {"
                    "padding: 0 20px; "
                    "background: #FFFFFF;"
            "}"
            
            ".oneColFixCtrHdr #footer {"
                    "padding: 0 10px; "
                    "background:#DDDDDD;"
            "}"
            
            ".oneColFixCtrHdr #footer p {"
                    "margin: 0; "
                    "padding: 10px 0;"
            "}"
            
            ".oneColFixCtrHdr #container #mainContent h6 {"
                    "color: #06F;"
                    "font-size: 12px;"
            "}"
            
            "</style></head>"
            
            "<body class='oneColFixCtrHdr'><br /><div id='container'><div id='header'>"
            "<table width='100%'><tr><td width='68%'>&nbsp;</td><td width='32%' bgcolor='#0099FF'>&nbsp;</td></tr>"
            "<tr><td>&nbsp;</td><td align='right'><a href='http://support.novell.com/advisor' target='_blank'>Novell Support Advisor</a></td></tr>"
            "</table></div><div id='mainContent'><br><br><table width='100%'>" );
    
    if ( summary.slesversion.isEmpty() )
    {
        out += ( "<tr><td width='16%'><b>Archive file:</b></td><td width='64%'>" + summary.archive + "</td></tr>"
                 "<tr><td>&nbsp;</td></tr></table>"
                 "<b><i>No further info available (most likely supportconfig version is too old)</i></b>" );
    }
    else
    {
        out += ( "<tr><td width='16%'><b>Supportconfig version:</b></td><td width='84%'>" + summary.scriptversion + " (" + summary.scriptdate.toString( "yyyy-MM-dd" ) + ")</td></tr>"
                 "<tr><td><b>Archive file:</b></td><td>" + summary.archive + "</td></tr>"
                 "<tr><td>&nbsp;</td></tr>"
                 "<tr><td><b>Report Run Date:</b></td><td>" + summary.rundate.toString( "yyyy-MM-dd - hh:mm" ) + "</td></tr>"
                 "<tr><td><b>Hostname:</b></td><td>" + summary.hostname + "</td></tr>"
                 "<tr><td>&nbsp;</td></tr>"
                 "<tr><td><b>SLE Version:</b></td><td>" + summary.slestype + " " + summary.slesversion + " SP " + summary.slessp + "</td></tr>" );
        
        if ( !summary.oesversion.isEmpty() )
        {
            out += ( "<tr><td><b>OES Version:</b></td><td> Open Enterprise Server " + summary.oesversion + " SP " + summary.oessp + "</td></tr>" );
        }
        
        out += ( "<tr><td><b>Architecture:</b></td><td>" + summary.arch + "</td></tr>"
                 "<tr><td>&nbsp;</td></tr>"
                 "<tr><td><b>Kernel:</b></td><td>" + summary.kernel + "</td></tr></table>" );
    }
            
    out += ( "<h6>Results Overview<hr /></h6>" );
    
    return out;
}

QString HTML::nsaTableStart( const QString& table )
{
    return "<strong>" + table + "</strong><table cellpadding='5'>";
}

QString HTML::nsaTableItem( NSATableItem ti )
{
    QString output;
    QList<QStringList> linkList = ti.linkList;
    
    if ( ti.type == NSATableItem::Critical )
    {
        output += "<tr bgcolor='#FF0000'>";
    }
    else if ( ti.type == NSATableItem::Warning )
    {
        output += "<tr bgcolor='#FFFF99'>";
    }
    else if ( ti.type == NSATableItem::Recommended )
    {
        output += "<tr bgcolor='#33CCFF'>";
    }
    
    output += "<td>" + ti.category + "</td>";
    output += "<td>" + ti.desc + "</td>";
    
    for ( int i = 0; i < linkList.size(); ++i )
    {
        output += "<td><a href='" + linkList.at(i).at(1) +"'>" + linkList.at(i).at(0) + "</a></td>";
    }
    
    output += "</tr>";
    
    return output;
}

QString HTML::nsaTableEnd()
{
    return "</table><br>";
}

QString HTML::nsaPageEnd()
{
    return "</html>";
}

QString HTML::timeString( int sec )
{
    int hours;
    int minutes;
    int days( sec / 86400 );
 
    QString d = "d";
    QString h = "h";
    QString m = "m";
    
    if ( days > 0 )
    {
        sec = ( sec - ( days * 86400 ) );
        hours = ( sec / 3600 );
        sec = ( sec - ( hours * 3600 ) );
        minutes = ( sec / 60 );
        
        if ( days == 1 ) d = "d";
        if ( hours == 1 ) h = "h";
        if ( minutes == 1 ) m = "m";
        
        return QString( QString::number( days ) + " " + d + "  " + QString::number( hours ) + " " + h + "  " +
                        QString::number( minutes ) + " " + m );
    }
    else 
    {
        hours = ( sec / 3600 );
        
        if ( hours > 0 )
        {
            sec = ( sec - ( hours * 3600 ) );
            minutes = ( sec / 60 );        
            
            if ( hours == 1 ) h = "h";
            if ( minutes == 1 ) m = "m";
            
            return QString( QString::number( hours ) + " " + h + "  " + QString::number( minutes ) + " " + m );
        }
        else
        {
            minutes = ( sec / 60 );   
            
            if ( minutes == 1 ) m = "m";
            
            return QString( QString::number( minutes ) + " " + m );
        }
    }
}

int HTML::secDays( int sec )
{
    uint now = QDateTime::currentDateTime().toTime_t();
    uint notNow = now - sec;
    QDateTime tmp = QDateTime::fromTime_t( notNow );
    return tmp.daysTo( QDateTime::currentDateTime() );
}

QString HTML::BgColor( QString color)
{
    QString bg;
    
    switch (color.at(0).toAscii()) {
        case 69: bg = Settings::ratingEcol();
            break;
        case 68: bg = Settings::ratingDcol();
            break;
        case 67: bg = Settings::ratingCcol();
            break;
        case 66: bg = Settings::ratingBcol();
            break;
        case 65: bg = Settings::ratingAcol();
            break;
        default: bg = Settings::ratingFcol();
            break;
    }
    return bg;
}
