cmake_minimum_required(VERSION 3.16)

project(DiagramPP VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)


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
    include/model/elements/composition/cpp_class.h
    include/model/elements/composition/cpp_struct.h
    include/model/elements/composition/composition.h
    include/model/elements/constructor/constructor.h
    include/model/elements/constructor/default_constructor.h
    include/model/elements/constructor/copy_constructor.h
    include/model/base/uml_class_diagram_node.h
    include/view/cpp_class_view.h
    include/validator.h
    include/commandManager/command_manager.h
    include/commandManager/command.h
    include/commandManager/composite_command.h
    include/commandManager/add_node_command.h
    include/commandManager/remove_node_command.h
    include/commandManager/add_branch_command.h
    include/commandManager/remove_branch_command.h
    include/project_generator.h
    include/model/base/branches.h
    include/model/base/branches.h
    include/view/member_dialog_base.h
    include/view/field_dialog.h
    include/view/method_dialog.h
    include/methodeditor.h
    include/widget/class_element_text.h
    include/serializers/diagram_json_serializer.h
    include/serializers/node_serializers.h
    include/serializers/composition_element_serializers.h
    include/serializers/utils/utils.h
    include/view/arrow.h
    include/view/line.h
    include/view/connection.h
    include/recoveyLog.h
    include/generate_project_dialog.h
    include/dialog_factory.h
    include/node_type.h
    include/shared_node_ptr.h
    include/connection_handler.h
    include/graph/graph.h
    include/serializers/serializer.h
)

set(CORE_SOURCES
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
    src/core/model/elements/composition/cpp_class.cpp
    src/core/model/elements/composition/cpp_struct.cpp
    src/core/model/elements/composition/composition.cpp
    src/core/model/elements/constructor/constructor.cpp
    src/core/model/elements/constructor/default_constructor.cpp
    src/core/model/elements/constructor/copy_constructor.cpp
    src/core/model/base/uml_class_diagram_node.cpp
    src/core/view/cpp_class_view.cpp
    src/commandManager/command_manager.cpp
    src/commandManager/composite_command.cpp
    src/commandManager/add_node_command.cpp
    src/commandManager/remove_node_command.cpp
    src/commandManager/add_branch_command.cpp
    src/commandManager/remove_branch_command.cpp
    src/core/project_generator.cpp
    src/core/model/base/branchs.cpp
    src/commandManager/command_manager.cpp
    src/commandManager/add_node_command.cpp
    src/core/view/member_dialog_base.cpp
    src/core/view/field_dialog.cpp
    src/core/view/method_dialog.cpp
    src/core/methodeditor.cpp
    src/core/widget/class_element_text.cpp
    src/serializers/diagram_json_serializer.cpp
    src/serializers/node_serializers.cpp
    src/serializers/composition_element_serializers.cpp
    src/serializers/utils/utils.cpp
    src/core/view/arrow.cpp
    src/core/view/line.cpp
    src/core/view/connection.cpp
    src/core/recoveryLog.cpp
    src/core/generate_project_dialog.cpp
    src/core/dialog_factory.cpp
    src/core/connection_handler.cpp
    src/serializers/serializer.cpp
)

set(UI_SOURCES
    src/ui/main_window.ui
    src/ui/Board.ui
    src/ui/methodeditor.ui
)

# Glavni projekat izvori
set(PROJECT_SOURCES
    ${CORE_HEADERS}
    ${CORE_SOURCES}
    ${UI_SOURCES}
)


set(TEST_SOURCES
    test/test_main.cpp
    test/test_diagram_graph.cpp
    test/test_field.cpp
    test/test_method.cpp
    test/test_validator.cpp
    test/test_default_constructor.cpp
    test/test_destructor.cpp
    test/test_copy_constructor.cpp
    test/test_command_manager.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# Qt executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiagramPP
        MANUAL_FINALIZATION
        src/core/main.cpp
        ${PROJECT_SOURCES}

    )
    qt_add_executable(DiagramTest
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${TEST_SOURCES}
    )
else()
    add_executable(DiagramPP ${PROJECT_SOURCES})
    add_executable(DiagramTest ${TEST_SOURCES})
endif()

# Linkovanje Qt biblioteka
target_link_libraries(DiagramPP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(DiagramTest
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Catch2::Catch2WithMain
)


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
target_include_directories(DiagramTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Finalizacija Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DiagramPP)
    qt_finalize_executable(DiagramTest)
endif()



