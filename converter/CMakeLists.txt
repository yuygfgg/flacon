 # BEGIN_COMMON_COPYRIGHT_HEADER
 # (c)LGPL2+
 #
 # Flacon - audio File Encoder
 # https://github.com/flacon/flacon
 #
 # Copyright: 2012-2017
 #   Alexander Sokoloff <sokoloff.a@gmail.com>
 #
 # This library is free software; you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public
 # License as published by the Free Software Foundation; either
 # version 2.1 of the License, or (at your option) any later version.

 # This library is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # Lesser General Public License for more details.

 # You should have received a copy of the GNU Lesser General Public
 # License along with this library; if not, write to the Free Software
 # Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 #
 # END_COMMON_COPYRIGHT_HEADER

cmake_minimum_required(VERSION 3.16.0)
cmake_policy(SET CMP0028 NEW)
if (POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()


project(converter)

set(HEADERS
    convertertypes.h
    wavheader.h
    decoder.h
    converter.h
    splitter.h
    encoder.h
    worker.h
    discpipline.h
    cuecreator.h
    coverimage.h
    extprocess.h
    replaygain.h
    totalprogresscounter.h
)

set(SOURCES
    convertertypes.cpp
    wavheader.cpp
    decoder.cpp
    converter.cpp
    splitter.cpp
    encoder.cpp
    worker.cpp
    discpipline.cpp
    cuecreator.cpp
    coverimage.cpp
    extprocess.cpp
    replaygain.cpp
    totalprogresscounter.cpp
)



#*******************************************
# include("cmake/macos.cmake")
# add_homebrew_qt_prefix_path()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    Core
    Widgets
)

find_package(ZLIB REQUIRED)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
