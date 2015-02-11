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
Summary:    SSH Pam access control
Version:    0.3.5
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
SSH Pam control application asking for permission for every incoming ssh connection


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
mkdir -p %{buildroot}/usr/lib/systemd/user/post-user-session.target.wants
ln -s ../ssh-pam-access-control.service %{buildroot}/usr/lib/systemd/user/post-user-session.target.wants/ssh-pam-access-control.service
# << install post

%post
# >> post
if ! grep "ssh-pam-access-control" /etc/pam.d/sshd
then
  echo "session required pam_exec.so quiet stdout /usr/bin/ssh-pam-access-control" >> /etc/pam.d/sshd
fi
systemctl-user restart ngfd.service
systemctl-user restart ssh-pam-access-control.service
# << post

# >> pre
%pre
systemctl-user stop ssh-pam-access-control.service
if /sbin/pidof ssh-pam-access-control > /dev/null; then
killall ssh-pam-access-control
fi
if /sbin/pidof ssh-pam-access-daemon > /dev/null; then
killall ssh-pam-access-daemon
fi
# << pre

%preun
# >> preun
sed -i "/ssh-pam-access-control/d" /etc/pam.d/sshd
systemctl-user stop ssh-pam-access-control.service
if /sbin/pidof ssh-pam-access-control > /dev/null; then
killall ssh-pam-access-control
fi
if /sbin/pidof ssh-pam-access-daemon > /dev/null; then
killall ssh-pam-access-daemon
fi
# << preun

%files
%defattr(-,root,root,-)
/usr/lib/systemd/user/*.service
/usr/lib/systemd/user/post-user-session.target.wants/*.service
%config /etc/profiled/*.ini
%config %{_datadir}/ngfd/events.d/*.ini
%config %{_datadir}/lipstick/notificationcategories/*.conf
%{_bindir}/
%{_datadir}/ssh-pam-access-daemon
%{_datadir}/jolla-settings/entries
# >> files
# << files
