#!/bin/bash

set -e
#set -x

HEAD="\e[01;38;05;232;48;05;180m"
NORM="\e[32;46m"

function TITLE() {
    echo " 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    printf "\e[01;38;05;232;48;05;180m%-80s\e[32;46m\n" "$1"
    echo " 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    local str=$(printf "%-80s" "$1")
    echo "\e[01;38;05;232;48;05;173m${str}\e[32;46m"
    echo " 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo -en "\e[01;38;05;232;48;05;173m"
    printf   "%-70s" "$1"
    echo -e  "\e[32;46m"
    echo " END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
}

PROGRAMS="faac flac lame mac oggenc opusenc sox wavpack wvunpack"
export "RELEASE_DATE=$(date +%Y.%m.%d_%H.%M.%S)"
export "RELEASE_VERSION=${GITHUB_REF#refs/*/}"


TITLE "Set env"
echo " * APPIMAGE_NAME: ${APPIMAGE_NAME}"
echo " * PROGRAMS:      ${PROGRAMS}"
export



TITLE "Install packages"
echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
apt-get -y -qq update >/dev/null
apt-get -y -qq install locales >/dev/null
sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
apt-get -y -qq install build-essential pkg-config cmake  qtbase5-dev qttools5-dev-tools qttools5-dev libuchardet-dev libtag1-dev zlib1g-dev >/dev/null
apt-get -y -qq install flac vorbis-tools wavpack lame faac opus-tools sox >/dev/null
apt-get -y -qq install desktop-file-utils >/dev/null




TITLE "Build Flacon"
set -x
cmake -E make_directory build
cmake -E make_directory build/app
cmake -DCMAKE_INSTALL_PREFIX=build/app/usr -DAPPIMAGE_BUNDLE=Yes -DCMAKE_BUILD_TYPE=$BUILD_TYPE -B build $GITHUB_WORKSPACE
make -C build -j 8
make -C build install

echo "$HEAD Build AppImage dir                                   ${NORM}"
export PATH=~/tools/appimage/usr/bin:$PATH
export LD_LIBRARY_PATH=~/tools/appimage/usr/lib

echo "= Create symlinks ============="
ln -sf usr/bin/flacon AppRun
ln -sf usr/share/icons/hicolor/128x128/apps/flacon.png .
ln -sf usr/share/applications/flacon.desktop .
echo "= Add programs ================"
for prog in ${PROGRAMS}; do
    echo " * Copy ${prog}";
    src=$(which "$prog");
    dest=${src#/};
    cp -f "${src}" "usr/bin/";
done

echo "= Build image ================="
linuxdeploy --verbosity=3  --plugin qt --appdir "${{github.workspace}}/build/app"