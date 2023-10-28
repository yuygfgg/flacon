#!/bin/bash

set -e
set -x

PROGRAMS="alacenc faac flac lame mac oggenc opusenc sox ttaenc wavpack wvunpack"
export "RELEASE_DATE=$(date +%Y.%m.%d_%H.%M.%S)"
export "RELEASE_VERSION=${GITHUB_REF#refs/*/}"
echo "░░ Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
echo "░░ * APPIMAGE_NAME: ${APPIMAGE_NAME}"
echo "░░ * PROGRAMS:      ${PROGRAMS}"
echo "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
export
echo "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"


echo "░░ Install packages ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
echo 'debconf debconf/frontend select Noninteractive' | sudo debconf-set-selections
sudo apt-get -y update
sudo apt-get -y install locales
sudo sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
sudo apt-get -y install build-essential pkg-config cmake  qtbase5-dev qttools5-dev-tools qttools5-dev libuchardet-dev libtag1-dev
sudo apt-get -y install flac vorbis-tools wavpack lame faac opus-tools sox
sudo apt-get -y install desktop-file-utils

echo "░░ Build Flacon ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
cmake -E make_directory build
cmake -E make_directory build/app
cmake -DCMAKE_INSTALL_PREFIX=build/app/usr -DAPPIMAGE_BUNDLE=Yes -DCMAKE_BUILD_TYPE=$BUILD_TYPE $GITHUB_WORKSPACE -B build
make -C build -j 8
make -C build install
