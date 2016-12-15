#
#               kueue - keep track of your SR queue
#         (C) 2011 - 2012 Stefan Bogner <sbogner@suse.com>
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the
#
#    Free Software Foundation, Inc.
#    59 Temple Place - Suite 330
#    Boston, MA  02111-1307, USA
#
#    Have a lot of fun :-)

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( kueue )
SET( KUEUE_VERSION git )

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE( LibMagic REQUIRED )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(  ${LIBMAGIC_INCLUDE_DIR} 
	              ${QT_INCLUDES} 
                      ${CMAKE_CURRENT_BINARY_DIR} 
		      ${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS( ${QT_DEFINITIONS} )

SET( QT_USE_QTSQL TRUE )
SET( QT_USE_QTNETWORK TRUE )
SET( QT_USE_QTWEBKIT TRUE )
SET( QT_USE_XML TRUE )
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

#
# Configure defines for the preprocessor
#

IF( QT_PHONON_FOUND AND NOT APPLE )
    OPTION( USE_PHONON "Use Phonon?" ON )
    SET( QT_USE_PHONON TRUE )
ENDIF( QT_PHONON_FOUND AND NOT APPLE )

IF( QT_QTDBUS_FOUND AND NOT APPLE )
    OPTION( QT_HAS_DBUS "Enable DBUS Support" ON )
ENDIF( QT_QTDBUS_FOUND AND NOT APPLE )

IF( WIN32 )
    OPTION( IS_WIN32 "Windows?" ON )
ENDIF( WIN32 )

IF( APPLE )
    OPTION( IS_OSX "MacOSX?" ON )
    OPTION( QT_HAS_DBUS "Enable DBUS Support" OFF )
ENDIF( APPLE )

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h )

#
# Add the "qtsingleapplication" subdirectory with the static lib
#

ADD_SUBDIRECTORY( qtsingleapplication )
ADD_SUBDIRECTORY( simplecrypt )

SET( QtSingleApp_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qtsingleapplication )
SET( QT_SINGLEAPP_LIBRARY qtsingleapplication )

SET( SimpleCrypt_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/simplecrypt )
SET( SIMPLECRYPT_LIBRARY simplecrypt )

INCLUDE_DIRECTORIES( ${QtSingleApp_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${ArchiveExtract_INCLUDE_DIR} )

#
# Define the source files
#

SET( kueue_CPP  main.cpp
                kueueapp.cpp
		kueuethreads.cpp
                settings.cpp
                network.cpp

                data/database.cpp
		data/data.cpp
		data/datathread.cpp
		data/dircleaner.cpp

                unity/unitypage.cpp
                unity/popupwindow.cpp
                unity/webeditor.cpp
                unity/unitynetwork.cpp

                ui/kueuewindow.cpp
                ui/html.cpp
                ui/configdialog.cpp
                ui/detailwindow.cpp
                ui/tabwidget.cpp
                ui/systray.cpp
                ui/statusbar.cpp
		ui/unitywidgets.cpp
		ui/updatedialog.cpp
		ui/download/edittableview.cpp
		ui/download/downloadmanager.cpp
		ui/download/squeezelabel.cpp
		ui/download/autosaver.cpp
		ui/qprogressindicator.cpp
		ui/busywidget.cpp
		ui/threadprogress.cpp
                ui/waitwidget.cpp

                ui/browsers/unitybrowser.cpp 
                ui/browsers/qmonbrowser.cpp
                ui/browsers/queuebrowser.cpp
                ui/browsers/statsbrowser.cpp 
		ui/browsers/helpbrowser.cpp
		ui/browsers/browser.cpp
		ui/browsers/subownerbrowser.cpp

		ui/browsers/search/webviewsearch.cpp 
                ui/browsers/search/browsersearch.cpp
		ui/browsers/search/searchbar.cpp 
		ui/browsers/search/searchlineedit.cpp 
		ui/browsers/search/lineedit.cpp 
		ui/browsers/search/clearbutton.cpp 
		ui/browsers/search/searchbutton.cpp
               
	        archivers/archiveextract.cpp	
                archivers/Utility.cpp
                
		supportconfig/supportconfig.cpp

                nsa/nsa.cpp
		nsa/nsawindow.cpp
	        nsa/nsajob.cpp ) 

SET( kueue_QRC  kueue.qrc )

SET( kueue_UI   ui/detailwindow.ui
                ui/kueuewindow.ui
                ui/configdialog.ui
                ui/configdialog-basic.ui
		ui/passwordprompt.ui
		ui/download/downloads.ui
		ui/download/downloaditem.ui
		ui/browsers/search/searchbanner.ui
                ui/ssdialog.ui
	        ui/calendardialog.ui
                ui/browsers/helpbrowser.ui
		ui/updatedialog.ui 
                ui/notedialog.ui
                ui/closedialog.ui )

#
# Default Qt stuff...
#

QT4_WRAP_UI( kueue_UI_H ${kueue_UI} )
QT4_ADD_RESOURCES( kueue_QRC_SRCS ${kueue_QRC} )
QT4_AUTOMOC( ${kueue_CPP} )

#
# On Windows, set a resource file containing the icons and use WIN32
# for the executable to get rid of cmd.exe coming up along kueue
#

IF( WIN32 )
    SET( RES_FILE "ui/icons/app/kueue.rc" )
    SET( CMAKE_RC_COMPILER_INIT windres )
    ENABLE_LANGUAGE( RC )
    SET( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>" )
    ADD_EXECUTABLE( kueue WIN32 ${RES_FILE} ${kueue_CPP} ${kueue_QRC_SRCS} ${kueue_UI_H} )
    SET( WINDOWS_LINK ${QT_PHONON_LIBRARY} ) 
ENDIF( WIN32 )

#
# On Linux, install the icon and desktop file according to freedesktop.org specs
# Also, enable uninstall support (see cmake_uninstall.cmake.in)
#

IF( UNIX AND NOT APPLE )
    ADD_EXECUTABLE( kueue ${kueue_CPP} ${kueue_QRC_SRCS} ${kueue_UI_H} )
    SET( APP_ICON ${CMAKE_SOURCE_DIR}/ui/icons/app/kueue.png )
    SET( DESKTOP_FILE ${CMAKE_SOURCE_DIR}/kueue.desktop )
    SET( ICON_DIR /usr/share/icons/hicolor )
    SET( DESKTOP_DIR /usr/share/applications )
    SET( SHARE_FILE ${CMAKE_SOURCE_DIR}/productmenu-default.xml )
    SET( SHARE_DIR /usr/share/kueue/ )
    INSTALL( TARGETS kueue DESTINATION bin )
    INSTALL( FILES ${APP_ICON} DESTINATION ${ICON_DIR}/32x32/apps )
    INSTALL( FILES ${SHARE_FILE} DESTINATION ${SHARE_DIR} )
    INSTALL( PROGRAMS ${DESKTOP_FILE} DESTINATION ${DESKTOP_DIR} ) 
    CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
    ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
    SET( LINUX_LINK ${QT_PHONON_LIBRARY} ${QT_QTDBUS_LIBRARY} )
ENDIF( UNIX AND NOT APPLE )

#
# On Mac OSX, create an app bundle which can be used to build the installer
# and build a dmg installer right away
#

IF( APPLE )
    SET( OSX_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ui/icons/app/kueue.icns )
    SET( MACOSX_BUNDLE_BUNDLE_NAME kueue )
    ADD_CUSTOM_TARGET( OSX_BUNDLE_DIRS COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/kueue.app/Contents/Resources
                                       COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/kueue.app/Contents/MacOS )
    SET_SOURCE_FILES_PROPERTIES( ${OSX_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET( kueue_SRC ${kueue_CPP} ${OSX_ICON_FILE} )
    ADD_EXECUTABLE( kueue MACOSX_BUNDLE ${kueue_SRC} ${kueue_QRC_SRCS} ${kueue_UI_H} )
    ADD_DEPENDENCIES( kueue OSX_BUNDLE_DIRS )
    SET_TARGET_PROPERTIES( kueue PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/packaging/osx/kueue.plist )
    #ADD_CUSTOM_TARGET( OSX_MAKE_INSTALLER ALL 
    #                                      COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/packaging/osx/mkdmg_cmake ${KUEUE_VERSION} 
    #                                      DEPENDS kueue OSX_BUNDLE_DIRS )
ENDIF( APPLE )

#
# And finally, link kueue against a few Qt libraries
#

TARGET_LINK_LIBRARIES( kueue ${QT_LIBRARIES} 
                             ${QT_QTCORE_LIBRARY} 
                             ${QT_QTWEBKIT_LIBRARY} 
                             ${QT_QTSQL_LIBRARIES} 
                             ${QT_QTUITOOLS_LIBRARY} 
                             ${QT_QTNETWORK_LIBRARY} 
                             ${QT_QTXML_LIBRARY}
                             ${QT_QTGUI_LIBRARY} 
                             ${QT_SINGLEAPP_LIBRARY}
                             ${SIMPLECRYPT_LIBRARY} 
                             ${QT_QTTEST_LIBRARY} 
                             ${LIBMAGIC_LIBRARY}
			     ${LINUX_LINK}
                             ${WINDOWS_LINK} )
