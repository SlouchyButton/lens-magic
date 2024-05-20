Name:           lens-magic
Version:        0.1.0
Release:        %autorelease
Summary:        App for photo development and tweaking with support for RAW images

License:        MPL-2.0 AND BSD-2-Clause
URL:            https://github.com/SlouchyButton/lens-magic
Source0:        https://github.com/SlouchyButton/lens-magic/archive/refs/tags/%{version}.tar.gz

BuildRequires:  meson
BuildRequires:  gcc
BuildRequires:  LibRaw-devel
BuildRequires:  libepoxy-devel
BuildRequires:  mesa-libGL-devel
BuildRequires:  libadwaita-devel
BuildRequires:  gtk4-devel
BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib

%description
This project aims to fill in the gap on Linux for 
easy to use app for RAW development and image tweaking. 
Goal of this app is to be easy as possible, 
follow GNOME style and be fast.

%prep
%autosetup


%build
%meson
%meson_build


%install
%meson_install


%check
desktop-file-validate %{buildroot}%{_datadir}/applications/com.alesnezbeda.lens-magic.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/com.alesnezbeda.lens-magic.metainfo.xml


%files
%license LICENSE 
%doc README.md
%{_bindir}/lens-magic
%{_datadir}/applications/com.alesnezbeda.lens-magic.desktop
%{_metainfodir}/com.alesnezbeda.lens-magic.metainfo.xml
%{_datadir}/glib-2.0/schemas/com.alesnezbeda.lens-magic.gschema.xml
%{_datadir}/icons/hicolor/scalable/apps/com.alesnezbeda.lens-magic.svg
%{_datadir}/icons/hicolor/symbolic/apps/com.alesnezbeda.lens-magic-symbolic.svg

%changelog
%autochangelog

