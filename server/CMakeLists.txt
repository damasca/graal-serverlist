#
#  server/src/CMakeLists.txt
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

include(CheckFunctionExists)
include(CheckSymbolExists)

set(
    SOURCES
	src/CFileSystem.cpp
	src/CMySQL.cpp
	src/codec.cpp
	src/CSocket.cpp
	src/FProfile.cpp
	src/main.cpp
	src/TPlayer.cpp
	src/TServer.cpp
	src/ListServer.cpp
	src/MySQLBackend.cpp
)

set(
    HEADERS
    ${PROJECT_BINARY_DIR}/server/include/IConfig.h
	include/CFileSystem.h
	include/TPlayer.h
	include/codec.h
	include/FProfile.h
	include/CMySQL.h
	include/TServer.h
	include/main.h
	include/CSocket.h
	include/ListServer.h
	include/PlayerProfile.h
	include/IDataBackend.h
	include/MySQLBackend.h
)

include_directories(
    # Include the CMake-generated version header from the build directory
    ${PROJECT_BINARY_DIR}/server/include
	include
)

# Set target names for the executables
if(APPLE OR WIN32)
    # OS X and Windows get a mixed-case binary name
    set(TARGET_NAME ${PROJECT_NAME})
elseif(EMSCRIPTEN)
    set(TARGET_NAME ${PROJECT_NAME_LOWER})
else()
    # Linux/other UNIX get a lower-case binary name
    set(TARGET_NAME ${PROJECT_NAME_LOWER})
endif()

check_symbol_exists(inet_pton "ws2tcpip.h" HAVE_INET_PTON)
if (NOT HAVE_INET_PTON)
	check_function_exists(inet_pton HAVE_INET_PTON)
endif()

if (HAVE_INET_PTON)
	add_definition(-DHAVE_INET_PTON)
endif()

if(NOT MYSQL_FOUND)
	include_directories(${PROJECT_SOURCE_DIR}/dependencies/mysqlclient/include)
	include_directories(${PROJECT_BINARY_DIR}/dependencies/mysqlclient/include)
endif()

include_directories(${PROJECT_SOURCE_DIR}/dependencies/gs2lib/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/mysql-modern-cpp/mysql+++)

if(APPLE)
	add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

    # Enable ARC (automatic reference counting) for OS X build
    set_property(
        TARGET ${TARGET_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc"
    )
elseif(WIN32)
    add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
	if(MSVC)
		set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
		set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "../bin")

		if(NOT MYSQL_FOUND)
			target_link_libraries(${TARGET_NAME} libmysql.lib)
		endif()
	endif()
elseif(EMSCRIPTEN)
	add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
else()
	add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
endif()

add_dependencies(${TARGET_NAME} gs2lib)
target_link_libraries(${TARGET_NAME} gs2lib)

if(MYSQL_FOUND)
	target_link_libraries(${TARGET_NAME} ${MYSQL_LIBRARY})
	else()
	add_dependencies(${TARGET_NAME} mariadbclient)
	target_link_libraries(${TARGET_NAME} mariadbclient)
endif()

target_link_libraries(${TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT})

if(WIN32)
	target_link_libraries(${TARGET_NAME} ws2_32 wsock32 iphlpapi)
endif()

file(GLOB TEXT
    "${PROJECT_NAME_LOWER}.wasm"
)

set(INSTALL_DEST "src")

install(FILES ${TEXT} DESTINATION ${INSTALL_DEST})

set(INSTALL_DEST src)

install(TARGETS ${TARGET_NAME} DESTINATION ${INSTALL_DEST})