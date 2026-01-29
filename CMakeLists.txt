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
    include/Board.h
    include/graph/diagram_graph.h
    include/model/base/visibility.h
    include/model/base/reference.h
    include/model/elements/argument.h
    include/model/elements/class_element.h
    include/model/elements/type/type.h
    include/model/elements/type/regular_type.h
    include/model/elements/type/generic_type.h
    include/model/elements/type/namespace_type.h
    include/model/elements/type/pointer_type.h
    include/model/elements/field.h
    include/model/elements/method.h
    include/model/elements/destructor.h
    include/model/elements/cpp_enum_value.h
    include/model/elements/composition/cpp_enum.h
    include/model/elements/composition/cpp_class.h
    include/model/elements/composition/cpp_struct.h
    include/model/elements/composition/composition.h
    include/model/elements/constructor/constructor.h
    include/model/elements/constructor/default_constructor.h
    include/model/elements/constructor/copy_constructor.h
    include/model/base/uml_class_diagram_node.h
    include/view/cpp_class.h
    include/validator.h
    include/commandManager/command_manager.h
    include/commandManager/command.h
    include/commandManager/add_node_command.h
    include/commandManager/remove_node_command.h
    include/project_generator.h
    include/model/base/branches.h
    include/model/base/branches.h
    include/view/node_view.h
    include/view/member_dialog_base.h
    include/view/field_dialog.h
    include/view/method_dialog.h
    include/nodefactory.h
    include/methodeditor.h
    include/widget/class_element_text.h
)

set(CORE_SOURCES
    src/core/main.cpp
    src/core/cpp_class_item.cpp
    src/core/help.cpp
    src/core/history.cpp
    src/core/import_project.cpp
    src/core/main_window.cpp
    src/core/new_project.cpp
    src/core/Board.cpp
    src/core/validator.cpp
    src/core/graph/diagram_graph.cpp
    src/core/model/base/visibility.cpp
    src/core/model/base/reference.cpp
    src/core/model/elements/argument.cpp
    src/core/model/elements/class_element.cpp
    src/core/model/elements/type/regular_type.cpp
    src/core/model/elements/type/generic_type.cpp
    src/core/model/elements/type/namespace_type.cpp
    src/core/model/elements/type/pointer_type.cpp
    src/core/model/elements/field.cpp
    src/core/model/elements/method.cpp
    src/core/model/elements/destructor.cpp
    src/core/model/elements/cpp_enum_value.cpp
    src/core/model/elements/composition/cpp_enum.cpp
    src/core/model/elements/composition/cpp_class.cpp
    src/core/model/elements/composition/cpp_struct.cpp
    src/core/model/elements/composition/composition.cpp
    src/core/model/elements/constructor/constructor.cpp
    src/core/model/elements/constructor/default_constructor.cpp
    src/core/model/elements/constructor/copy_constructor.cpp
    src/core/model/base/uml_class_diagram_node.cpp
    src/core/view/cpp_class.cpp
    src/commandManager/command_manager.cpp
    src/commandManager/add_node_command.cpp
    src/commandManager/remove_node_command.cpp
    src/commandManager/command_manager.cpp
    src/core/project_generator.cpp
    src/core/model/base/branchs.cpp
    src/commandManager/command_manager.cpp
    src/commandManager/add_node_command.cpp
    src/core/view/node_view.cpp
    src/core/view/member_dialog_base.cpp
    src/core/view/field_dialog.cpp
    src/core/view/method_dialog.cpp
    src/core/nodefactory.cpp
    src/core/methodeditor.cpp
    src/core/widget/class_element_text.cpp
)

set(UI_SOURCES
    src/ui/main_window.ui
    src/ui/Board.ui
    src/ui/nodefactory.ui
    src/ui/methodeditor.ui
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

# Qt executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiagramPP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/image_paths.h
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

