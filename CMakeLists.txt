cmake_minimum_required(VERSION 3.5)

project(PROMETHEEMethodsApp VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  ortools
  GIT_REPOSITORY https://github.com/google/or-tools.git
  GIT_TAG        main
)
set(BUILD_DEPS ON)
set(BUILD_SAMPLES OFF)
set(BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(ortools)

#https://stackoverflow.com/questions/72250704/cant-build-or-tools-fetched-with-fetchcontent-with-msvc-in-qt-creator
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
FetchContent_MakeAvailable(absl)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        newproblemdialog.h newproblemdialog.cpp newproblemdialog.ui
        helpdialog.h helpdialog.cpp helpdialog.ui
        Actions.h Actions.cpp
        Drawing.h Drawing.cpp drawing.ui
        prometheetable.h prometheetable.cpp PrometheeTable.ui
        prometheev.h prometheev.cpp prometheev.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PROMETHEEMethodsApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PROMETHEEMethodsApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PROMETHEEMethodsApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PROMETHEEMethodsApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(PROMETHEEMethodsApp PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(PROMETHEEMethodsApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PROMETHEEMethodsApp PRIVATE ortools::ortools)

set_target_properties(PROMETHEEMethodsApp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS PROMETHEEMethodsApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PROMETHEEMethodsApp)
endif()
