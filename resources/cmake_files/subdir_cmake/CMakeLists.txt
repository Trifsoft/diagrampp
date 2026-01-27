add_library(core
    
)

target_include_directories(core
    PUBLIC  
        ${CMAKE_SOURCE_DIR}/include
)

add_executable(app
    
)

target_link_libraries(app
    PRIVATE
        core
)
