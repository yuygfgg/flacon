#!/bin/bash

set -e
set -x

echo ${PROGRAMS}

uname -a
echo "░░ Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
export "RELEASE_DATE=$(date +%Y.%m.%d_%H.%M.%S)"
export "RELEASE_VERSION=${GITHUB_REF#refs/*/}"

if [[ "${GITHUB_REF_TYPE}"  = "tag" ]]; then
  export APPIMAGE_NAME="flacon-${GITHUB_REF_NAME:1}-x86_64.AppImage"
else 
  [[ "${GITHUB_REF_TYPE}" != "tag" ]] && export APPIMAGE_NAME="flacon-$(date +%Y.%m.%d_%H.%M.%S)-x86_64.AppImage"
fi 

echo "░░ Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
pwd
echo "░░ Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
ls -l 
echo "░░ Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
echo "==============================="
export
echo "-------------------------------"
cat $GITHUB_ENV
echo "==============================="

echo "░░ Install packages Set env ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
echo 'debconf debconf/frontend select Noninteractive' | sudo debconf-set-selections
sudo apt-get -y update
sudo apt-get -y install locales
sudo sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
sudo apt-get -y install build-essential pkg-config cmake  qtbase5-dev qttools5-dev-tools qttools5-dev libuchardet-dev libtag1-dev
sudo apt-get -y install flac vorbis-tools wavpack lame faac opus-tools sox
sudo apt-get -y install desktop-file-utils

echo "░░ Build Flacon ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
cmake -E make_directory ${{github.workspace}}/build
cmake -E make_directory ${{github.workspace}}/build/app
cmake -DCMAKE_INSTALL_PREFIX=${{github.workspace}}/build/app/usr -DAPPIMAGE_BUNDLE=Yes -DCMAKE_BUILD_TYPE=$BUILD_TYPE $GITHUB_WORKSPACE -B ${{github.workspace}}/build
make -C ${{github.workspace}}/build -j 8
make -C ${{github.workspace}}/build install
