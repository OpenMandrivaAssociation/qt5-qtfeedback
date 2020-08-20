%define date 20180903
%define libname %mklibname Qt5Feedback 0
%define devel %mklibname -d Qt5Feedback

Summary:	Qt Tactile Feedback Add-on Module
Name:		qt5-qtfeedback
Version:	0.0.0
Release:	%{?date:0.%{date}.}1
Source:		https://github.com/qt/qtfeedback/archive/master.tar.gz
Patch0:		qtfeedback-compile.patch
BuildRequires:	cmake(Qt5Core)
BuildRequires:	cmake(Qt5Gui)
BuildRequires:	cmake(Qt5Qml)
BuildRequires:	qmake5
License:	LGPLv3

%description
Qt Tactile Feedback Add-on Module

%package -n %{devel}
Summary:	Development files for the Qt Tactile Feedback library
Group:		Development/C and C++
Requires:	%{libname} = %{EVRD}

%description -n %{devel}
Development files for the Qt Tactile Feedback library

%prep
%autosetup -p1 -n qtfeedback-master
qmake-qt5

%build
%make_build

%install
%make_build install INSTALL_ROOT="%{buildroot}"

%libpackage Qt5Feedback 0

%files
%{_includedir}/qt5/QtFeedback
%{_libdir}/qt5/plugins/feedback
%{_libdir}/qt5/qml/QtFeedback

%files -n %{devel}
%{_libdir}/cmake/Qt5Feedback
%{_libdir}/libQt5Feedback.prl
%{_libdir}/libQt5Feedback.so
%{_libdir}/pkgconfig/Qt5Feedback.pc
%{_libdir}/qt5/mkspecs/modules/qt_lib_feedback.pri
%{_libdir}/qt5/mkspecs/modules/qt_lib_feedback_private.pri

