%define date 20180903
%define libname %mklibname Qt5Feedback 0
%define devel %mklibname -d Qt5Feedback
%global qtversion %(rpm -q --qf '%{VERSION}' qmake5)

Summary:	Qt Tactile Feedback Add-on Module
Name:		qt5-qtfeedback
Version:	0.0.0
Release:	%{?date:0.%{date}.}5
Source:		https://github.com/qt/qtfeedback/archive/master.tar.gz
Patch0:		qtfeedback-add-evdev-plugin.patch
BuildRequires:	cmake(Qt5Core)
BuildRequires:	cmake(Qt5Gui)
BuildRequires:	cmake(Qt5Qml)
BuildRequires:	cmake(Qt5Multimedia)
BuildRequires:	qmake5
BuildRequires:	perl
License:	LGPLv3

%description
Qt Tactile Feedback Add-on Module

%package -n %{devel}
Summary:	Development files for the Qt Tactile Feedback library
Group:		Development/C and C++
Requires:	%{name} = %{EVRD}
Requires:	%{libname} = %{EVRD}

%description -n %{devel}
Development files for the Qt Tactile Feedback library

%prep
%autosetup -p1 -n qtfeedback-master
perl %{_libdir}/qt5/bin/syncqt.pl -version %{qtversion}
qmake-qt5

%build
%make_build

%install
%make_build install INSTALL_ROOT="%{buildroot}"
# This file is bogus and breaks things by just being there
rm -f %{buildroot}%{_libdir}/cmake/Qt5Feedback/Qt5Feedback_.cmake

%dependinglibpackage Qt5Feedback 0

%files
%{_libdir}/qt5/plugins/feedback
%{_libdir}/qt5/qml/QtFeedback

%files -n %{devel}
%{_includedir}/qt5/QtFeedback
%{_libdir}/cmake/Qt5Feedback
%{_libdir}/libQt5Feedback.prl
%{_libdir}/libQt5Feedback.so
%{_libdir}/pkgconfig/Qt5Feedback.pc
%{_libdir}/qt5/mkspecs/modules/qt_lib_feedback.pri
%{_libdir}/qt5/mkspecs/modules/qt_lib_feedback_private.pri

