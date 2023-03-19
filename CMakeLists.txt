cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(cafe VERSION 0.1.0 LANGUAGES C)

include_directories(. external/ external/lua/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(MENU_FOLDER menu/)
set(MENU coffee latte tea mocha)

include_directories(. external/)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(LIBRARIES ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

if (WIN32)
    set(LIBRARIES ${LIBRARIES} gdi32 winmm)
else()
    set(LIBRARIES ${LIBRARIES} dl m pthread)
endif()

set(STATIC_LIBRARIES ${LIBRARIES})
set(SHARED_LIBRARIES ${LIBRARIES})

foreach(el ${MENU})
    include_directories("${MENU_FOLDER}/${el}" "${MENU_FOLDER}/${el}/external")
    add_library(${el}Lib SHARED "${MENU_FOLDER}/${el}/${el}.c")
    add_library(${el}LibStatic STATIC "${MENU_FOLDER}/${el}/${el}.c")
    target_link_libraries(${el}LibStatic ${LIBRARIES})
    if (NOT WIN32)
        set_target_properties(${el}Lib PROPERTIES OUTPUT_NAME ${el})
        set_target_properties(${el}LibStatic PROPERTIES OUTPUT_NAME ${el})
    endif()
    list(APPEND SHARED_LIBRARIES ${el}Lib)
    list(APPEND STATIC_LIBRARIES ${el}LibStatic)
endforeach(el)

file(GLOB cafe_sources cafe.c impl.c)

add_library(cafeLib SHARED ${cafe_sources})
add_library(cafeLibStatic STATIC ${cafe_sources})

target_link_libraries(cafeLib ${SHARED_LIBRARIES})
target_link_libraries(cafeLibStatic ${STATIC_LIBRARIES})

set_target_properties(cafeLib PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(cafeLibStatic PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_executable(game main.c)
target_link_libraries(game cafeLibStatic)