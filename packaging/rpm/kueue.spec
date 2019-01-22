#
# spec file for package kueue (Version 5.6.7)
#
# Copyright 2011 Stefan Bogner <sbogner@suse.com>
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.
#

# norootforbuild

BuildRequires:      libqt4-devel cmake update-desktop-files libQtWebKit-devel xdg-utils

%if 0%{?sles_version} == 0
  BuildRequires:      phonon-devel
%endif

Name:               kueue
Summary:            Queue-Manager
Version:            5.6.7
Release:            1
License:            GPL
Group:              System/GUI/Qt
Source0:            kueue-%{version}.tar.gz
BuildRoot:          %{_tmppath}/%{name}-%{version}-build
AutoReqProv:        yes

Requires:           libqt4 libqt4-sql-sqlite libQtWebKit4

%if 0%{?sles_version} == 0
  Requires:           libphonon4
%endif

%description
Manage your Unity queue using kueue

Authors:
-------
    Stefan Bogner <sbogner@suse.com>

%prep
  %setup -n kueue-%{version}

%build
  %if 0%{?sles_version} != 0
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DUSE_PHONON=no
  %else
    cmake -DCMAKE_INSTALL_PREFIX=/usr
  %endif
  %{__make}

%install
  %{makeinstall}

%clean
  rm -rf $RPM_BUILD_ROOT

%files 
%defattr(-,root,root)
/usr/bin/kueue
%dir /usr/share/icons/hicolor
%dir /usr/share/icons/hicolor/32x32
%dir /usr/share/icons/hicolor/32x32/apps
/usr/share/icons/hicolor/32x32/apps/kueue.png
/usr/share/applications/kueue.desktop

%changelog
