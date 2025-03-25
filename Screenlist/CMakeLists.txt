cmake_minimum_required(VERSION 3.16)

project(ScreenList VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Test)
find_package(Qt6 REQUIRED COMPONENTS Gui)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ScreenList
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        sl_error.cpp sl_error.h sl_generator.cpp sl_generator.h sl_interface.cpp sl_interface.h sl_options.cpp sl_options.h sl_profile.cpp sl_profile.h sl_video_file.cpp sl_video_file.h
        GeneratorThread.cpp GeneratorThread.h resources.qrc
        about.h
        Settings.h Settings.cpp
        ProfileList.h ProfileList.cpp
        VideoItem.h VideoItem.cpp
        VdeoListModel.h VdeoListModel.cpp
        VdeoListModel.cpp VdeoListModel.h VideoItem.cpp VideoItem.h
        VdeoListModel.cpp VdeoListModel.h VideoItem.cpp VideoItem.h
        TODO.txt

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ScreenList APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ScreenList SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ScreenList
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ScreenList PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ScreenList PRIVATE Qt6::Multimedia)
target_link_libraries(ScreenList PRIVATE Qt6::Test)
target_link_libraries(ScreenList PRIVATE Qt6::Gui)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ScreenList)
endif()
set_target_properties(ScreenList PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ScreenList
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScreenList)
endif()
