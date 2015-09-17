cmake_minimum_required(VERSION 3.0)
project(maudio_editor)

if(NOT DEFINED MAUDIO_PARENT_PROJECT)
	set(MAUDIO_PARENT_PROJECT 0)
endif()

set(PORTABLE 1 CACHE BOOL "set portable install")
set(DEBIAN 0 CACHE BOOL "create Debian package")

if(PORTABLE EQUAL 1)
	message(STATUS "maudio_editor: using portable install")
	
	set(CMAKE_INSTALL_PREFIX "install")
	add_definitions("-DMAUDIO_PORTABLE=1")
else()
	message(STATUS "maudio_editor: using system install")
	
	add_definitions("-DMAUDIO_PORTABLE=0")
endif()

set(MAUDIO_EDITOR_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(MAUDIO_EDITOR_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(MAUDIO_EDITOR_UI_DIR "${PROJECT_SOURCE_DIR}/ui")
set(MAUDIO_EDITOR_RES_DIR "${PROJECT_SOURCE_DIR}/res")
set(MAUDIO_EDITOR_TEST_DIR "${PROJECT_SOURCE_DIR}/test")

include_directories(${MAUDIO_EDITOR_INCLUDE_DIR})
include_directories(${MAUDIO_EDITOR_SOURCE_DIR})
include_directories(${MAUDIO_EDITOR_UI_DIR})
include_directories(${MAUDIO_EDITOR_RES_DIR})
include_directories(${MAUDIO_EDITOR_TEST_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wl,--no-as-needed")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "./res/lib:.")

#qt5 stuff
find_package(Qt5Widgets REQUIRED)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

#set preprocessor variables for version and commit hash
if(EXISTS "${PROJECT_SOURCE_DIR}/.git/HEAD")
	execute_process(
		COMMAND git rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
		COMMAND git log -1 --format=%H
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
		COMMAND git describe --long --tags --dirty --always
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_VERSION_DESCRIPTION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
		COMMAND git describe --always
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	add_definitions("-DMAUDIO_EDITOR_GIT_BRANCH=\"${GIT_BRANCH}\"")
	add_definitions("-DMAUDIO_EDITOR_GIT_COMMIT=\"${GIT_COMMIT_HASH}\"")
	add_definitions("-DMAUDIO_EDITOR_GIT_DESCRIPTION=\"${GIT_VERSION_DESCRIPTION}\"")
	
	if(${GIT_VERSION} MATCHES "^v%d.%d.%d")
		add_definitions("-DMAUDIO_EDITOR_VERSION=\"${GIT_VERSION}\"")
	endif()
else()
	message(WARNING "Couldn't retrieve version information from git!")
endif()

set(MAUDIO_EDITOR_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
)

qt5_wrap_cpp(EDITOR_QT5_SRC
	${PROJECT_SOURCE_DIR}/include/mainwindow.hpp
)

qt5_wrap_ui(EDITOR_QT5_UI
	${PROJECT_SOURCE_DIR}/ui/mainwindow.ui
)

#example binary
add_executable(maudio_editor ${MAUDIO_EDITOR_SOURCE_FILES} ${EDITOR_QT5_SRC} ${EDITOR_QT5_UI})
target_link_libraries(maudio_editor ${Qt5Widgets_LIBRARIES})

if(${MAUDIO_PARENT_PROJECT} EQUAL 0)
	target_link_libraries(maudio_editor -lmaudio)
else()
	target_link_libraries(maudio_editor maudio)
endif()

#install lib to prefix
if(PORTABLE EQUAL 1)
	install(TARGETS maudio_editor RUNTIME DESTINATION ".")
else()
	install(TARGETS maudio_editor RUNTIME DESTINATION "lib")
endif()

#create packages
if(${MAUDIO_PARENT_PROJECT} EQUAL 0)
	if(PORTABLE EQUAL 1)
		message(STATUS "maudio_editor: using zip package generator")
		set(CPACK_GENERATOR "ZIP")
	elseif(${DEBIAN} EQUAL 1)
		message(STATUS "maudio_editor: using Debian package generator")
		set(CPACK_GENERATOR "DEB")
	endif()

	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Schwarz")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MAudio-Editor-Qt")
	set(CPACK_PACKAGE_VENDOR "Martin Schwarz")
	set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.txt")
	set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
	set(CPACK_PACKAGE_VERSION_MAJOR "0")
	set(CPACK_PACKAGE_VERSION_MINOR "0")
	set(CPACK_PACKAGE_VERSION_PATCH "0")
	set(CPACK_OUTPUT_FILE_PREFIX "packages")
	INCLUDE(CPack)
endif()





