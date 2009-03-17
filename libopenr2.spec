Name:           libopenr2
Version:        1.2.0
Release:        1%{?dist}
Summary:        MFC/R2 call setup library
Packager:       Alexandre Cavalcante Alencar <alexandre.alencar@gmail.com>

Group:          System Environment/Libraries
License:        LGPL
URL:            http://www.libopenr2.org/
Source0:        http://openr2.googlecode.com/files/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:	chrpath

%description
OpenR2 is a library that implements the MFC/R2 signalling over E1 lines using
the Zapata Telephony interface. The MF R2 tones required for the signaling are
generated by code borrowed from the LGPL library SpanDSP written by Steve Underwood, 
the user has the option to provide a MF interface to use his own MF R2 tone generation
and detection so the library will use them when needed, that's why this library does not 
depend directly on spandsp, libteletone or zaptel for tone generation and detection
(depends on zaptel for ABCD bits and general media transmission though)


%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q -n openr2


%build
%configure --libdir=%{_libdir} --sysconfdir=%{_sysconfdir}/openr2 --disable-static --disable-rpath
sed -i 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' libtool
sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
chrpath --delete $RPM_BUILD_ROOT%{_bindir}/r2test
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%clean
rm -rf $RPM_BUILD_ROOT


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING COPYING.LESSER NEWS README TODO
%doc doc/*.alaw
%doc doc/README.asterisk
%doc %dir doc/asterisk
%{_bindir}/r2test
%{_libdir}/*.so.*
%{_mandir}/man5/r2test.conf.5.gz
%{_mandir}/man8/r2test.8.gz
%config(noreplace) %{_sysconfdir}/openr2/r2test.conf
%config(noreplace) %{_sysconfdir}/openr2/r2proto.conf


%files devel
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING COPYING.LESSER NEWS README TODO
%{_includedir}/openr2.h
%{_includedir}/openr2/*.h
%{_libdir}/*.so


%changelog
* Mon Mar 16 2009 Alexandre Alencar <alexandre.alencar@gmail.com>
- Updated package version number
- Updated to OpenR2 source package 1.2.0
- Fixed some typos

* Mon Feb 02 2009 Alexandre Alencar <alexandre.alencar@gmail.com>
- Updated package version number
- Add compilance with ANATEL (Brazilian telecomunications agency)

* Mon Jan 12 2009 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update package version

* Sat Nov 29 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update package revision number
- Add missing files
- Small fixes on spec file

* Sat Nov 29 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to latest SVN release
- Changed version number
- Added sample docs

* Thu Oct 30 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to latest SVN release

* Thu Oct 09 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to latest SVN release
- Added r2test and r2test.conf man pages
- Build r2test by default
- Add r2test.conf and r2proto.conf files

* Mon Sep 15 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to latest SVN release
- Added r2hwcompat.h

* Thu Jul 10 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to latest SVN release
- Removed rpath and libdir perl fix script as we don't need it

* Tue Jun 24 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to lastest SVN release and changed release number to SVN

* Tue May 13 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to lastest SVN release and add r2engine.h header
- Removed SpanDSP dependency to reflect upstream

* Thu May 08 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Updated URL's, License to LGPL as upstream and conditional
- requirement of SpanDSP library and headers

* Tue Apr 29 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Update to 1.1 version with Brazilian R2D support

* Thu Apr 24 2008 Alexandre Alencar <alexandre.alencar@gmail.com>
- Initial release
