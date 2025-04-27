cmake_minimum_required(VERSION 3.16)

project(keyboard_cat)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat -Wformat-security -Werror -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)
find_package(GIF REQUIRED)
find_package(tomlplusplus REQUIRED)

if (APPLE)
    set(HANDLER_SOURCE "keyboard_cat/handler/detail/macos/handler.cpp")
else ()
    set(HANDLER_SOURCE "keyboard_cat/handler/detail/linux/handler.cpp")
endif ()

add_executable(${PROJECT_NAME}
    keyboard_cat/main.cpp
    keyboard_cat/ui/window.cpp
    keyboard_cat/ui/tray.cpp
    ${HANDLER_SOURCE}
    keyboard_cat/handler/handler.cpp
    keyboard_cat/renderer/renderer.cpp
    keyboard_cat/gif/gif.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    GIF::GIF
    tomlplusplus::tomlplusplus
)

target_compile_definitions(${PROJECT_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)

if (APPLE)
    set(input_root "${CMAKE_CURRENT_LIST_DIR}/keyboard_cat")
    macro(add_resource FILE)
        file(RELATIVE_PATH relpath "${input_root}" "${FILE}")
        get_filename_component(relpath "${relpath}" DIRECTORY)
        target_sources(${PROJECT_NAME} PRIVATE ${FILE})
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${relpath}")
    endmacro()
    add_resource("${CMAKE_CURRENT_LIST_DIR}/resources/hallow_cat.gif")
    add_resource("${CMAKE_CURRENT_LIST_DIR}/resources/icon128.bmp")

    find_library(IO_LIB ImageIO)
    find_library(CS_LIB CoreServices)
    find_library(CT_LIB CoreText)
    find_library(CG_LIB CoreGraphics)
    find_library(CF_LIB CoreFoundation)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CF_LIB} ${CT_LIB} ${IO_LIB} ${CS_LIB} ${CG_LIB})
else()
    macro(copy_helper filename)
        set(outname "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/${filename}")
        add_custom_command(POST_BUILD
            TARGET "${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/resources/${filename}" "${outname}"
            DEPENDS "${filename}"
        )
    endmacro()
    copy_helper("hallow_cat.gif")
    copy_helper("icon128.bmp")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.ravbug.sdl3-sample"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.ravbug.sdl3-sample"
)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION ./install COMPONENT Runtime
        RUNTIME DESTINATION ./install/bin COMPONENT Runtime
    )
endif()
