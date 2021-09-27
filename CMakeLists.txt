cmake_minimum_required(VERSION 3.5)

project(RegistryEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

option(UPDATE_TRANSLATIONS "Update source translation (files translations/*.ts)." ON)

set(TS_FILES
    RegistryEditor_ru_RU.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    itemdialog.h
    itemdialog.cpp
    mainwindow.h)

set(PROJECT_FORMS
    mainwindow.ui
    itemdialog.ui)

set(PROJECT_FILES
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
    ${TS_FILES})

qt_add_executable(RegistryEditor
    MANUAL_FINALIZATION
    ${PROJECT_FILES})

if(UPDATE_TRANSLATIONS)
    message(STATUS"UPDATE_TRANSLATIONS option is set.")
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt_add_translation(QM_FILES ${TS_FILES})
endif()

target_link_libraries(RegistryEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RegistryEditor)
endif()
