# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       ssh-pam-access-control

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    My SailfishOS Application
Version:    0.1
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Source100:  ssh-pam-access-control.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
Short description of my SailfishOS Application


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

%post
# >> post
if ! grep "ssh-pam-access-control" /etc/pam.d/sshd
then
#  echo "session optional pam_exec.so stdout seteuid /usr/bin/ssh-pam-access-control" >> /etc/pam.d/sshd
fi
# << post

%preun
# >> preun
#sed -i "/ssh-pam-access-control/d" /etc/pam.d/sshd
# << preun

%files
%defattr(-,root,root,-)
%{_bindir}/
%{_datadir}/ssh-pam-access-daemon
# >> files
# << files
