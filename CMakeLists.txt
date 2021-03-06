#
#  CMakeLists.txt
#
#  Copyright 2019 死体
#
#  This file is part of GS2Emu.
#
#  GS2Emu is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  GS2Emu is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with GS2Emu.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8.5)
project(ListServer C CXX)

set(CMAKE_DEBUG_POSTFIX _d)

set(BIN_DIR "bin" CACHE STRING "Binary output directory")


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/lib )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/lib )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/${BIN_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

link_directories(${PROJECT_SOURCE_DIR}/lib)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

# Lowercase project name for binaries and packaging
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# Version number in format X.YY.ZZ
set(VER_X 3)
set(VER_Y 0)
set(VER_Z 0)
set(VER_EXTRA " GIT" CACHE STRING "Extra version")
set(VER_FULL "${VER_X}.${VER_Y}.${VER_Z}${VER_EXTRA}")

STRING(REGEX REPLACE " " "-" VER_CPACK ${VER_FULL})

# Generate version header from the above
configure_file(
	${PROJECT_SOURCE_DIR}/server/include/IConfig.h.in
	${PROJECT_BINARY_DIR}/server/include/IConfig.h
)

option(NOSTATIC "Don't compile as a static runtime." OFF)
if(NOSTATIC)
	message("Don't compile as a static runtime.")
	set(BUILD_SHARED_LIBS TRUE CACHE BOOL "-" FORCE)
else()
	message("Compile as static runtime.")
	add_definitions(-DSTATICLIB)
	set(BUILD_SHARED_LIBS FALSE CACHE BOOL "-" FORCE)
endif()

option(NOMYSQL "Don't compile with MySQL support." OFF)
if(NOMYSQL)
	message("Don't compile with MySQL support.")
	add_definitions(-DNO_MYSQL)
else()
	message("Compile with MySQL support.")
endif()

if(APPLE)
	# Set variables for generating the Info.plist file
	set(MACOSX_BUNDLE_BUNDLE_VERSION "${VER_FULL}")
	set(MACOSX_BUNDLE_EXECUTABLE ${PROJECT_NAME})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.GraalReborn.ListServer")
	set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "Application")
	set(MACOSX_BUNDLE_ICON_FILE "carton")
	set(MACOSX_BUNDLE_NAME ${PROJECT_NAME})
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VER_FULL}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=gnu++17")
elseif(WIN32)
	# Visual C++ Compiler options
	if(MSVC)
		# Suppress secure string function warnings
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)

		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			add_compile_options("/std:c++latest")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
		endif()

		# Enable parallel compilation
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

		# Enable static linkage of the Microsoft Visual C/C++ Runtime
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MTd")
		set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(
			CMAKE_CXX_FLAGS_RELWITHDEBINFO
			"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MTd"
		)
		set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
		cmake_policy(SET CMP0043 NEW)
	endif()

	if( MINGW )
		add_definitions(-D__STDC_FORMAT_MACROS)
		add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
	endif()

	# Prevent Windows.h from clashing with the Standard Template Library so we
	# can use std::min/std::max (see https://support.microsoft.com/kb/143208)
	add_definitions(-DNOMINMAX)

elseif(AROS)
elseif(AMIGA)
else()
	if(NOT NOSTATIC)
	#	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	endif()

	add_definitions(-D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
endif()

# Prefer static linkage
if(NOT NOSTATIC)
	set(SUFFIXES_ORIG ${CMAKE_FIND_LIBRARY_SUFFIXES})
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

if(NOT NOMYSQL)
	find_package(MySQL)
	if(NOT MYSQL_FOUND)
		add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/mysqlclient EXCLUDE_FROM_ALL)
	endif()
endif()

find_package(Threads REQUIRED)

# Packaging
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
	set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME} ${VER_FULL}")
	set(
			CPACK_DMG_DS_STORE_SETUP_SCRIPT
			${PROJECT_SOURCE_DIR}/resources/packaging/osx/DMGSetup.scpt
	)
	set(
			CPACK_DMG_BACKGROUND_IMAGE
			${PROJECT_SOURCE_DIR}/resources/packaging/osx/DMGBackground.tif
	)
elseif(WIN32)
	set(CPACK_GENERATOR ZIP)
elseif(AMIGA)
	set(CPACK_GENERATOR LHA)
else()
	set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_PACKAGE_NAME ${PROJECT_NAME_LOWER})
set(CPACK_PACKAGE_VENDOR "Graal Reborn Team")
set(CPACK_PACKAGE_CONTACT "graal.in")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VER_CPACK}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VER_X})
set(CPACK_PACKAGE_VERSION_MINOR ${VER_Y})
set(CPACK_PACKAGE_VERSION_PATCH ${VER_Z}${VER_EXTRA})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graal Online v1.411 to v5 compatible listserver")
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VER_CPACK}-src")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.bzr/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/gs2lib)

# Restore library suffixes
if(NOT NOSTATIC)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ${SUFFIXES_ORIG})
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/bin)
add_subdirectory(${PROJECT_SOURCE_DIR}/server)
