cmake_minimum_required(VERSION 3.16)

project(DiagramPP VERSION 0.1 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")


# Qt automatski alati
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Pronađi Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Core sources and headers
set(CORE_HEADERS
    include/cpp_class_item.h
    include/help.h
    include/history.h
    include/import_project.h
    include/main_window.h
    include/new_project.h
    include/project.h
)

set(CORE_SOURCES
    src/core/main.cpp
    src/core/cpp_class_item.cpp
    src/core/help.cpp
    src/core/history.cpp
    src/core/import_project.cpp
    src/core/main_window.cpp
    src/core/new_project.cpp
    src/core/project.cpp
)

set(UI_SOURCES
    src/ui/main_window.ui
    src/ui/project.ui
)

# Glavni projekat izvori
set(PROJECT_SOURCES
    ${CORE_HEADERS}
    ${CORE_SOURCES}
    ${UI_SOURCES}
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)


source_group("include" FILES ${CORE_HEADERS})
source_group("src/core" FILES ${CORE_SOURCES})
source_group("src/ui" FILES ${UI_SOURCES})
# Qt executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiagramPP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(DiagramPP ${PROJECT_SOURCES})
endif()

# Linkovanje Qt biblioteka
target_link_libraries(DiagramPP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# iOS/macOS bundle opcije
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DiagramPP)
endif()
set_target_properties(DiagramPP PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install target
include(GNUInstallDirs)
install(TARGETS DiagramPP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(DiagramPP PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}

)

# Finalizacija Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DiagramPP)
endif()

