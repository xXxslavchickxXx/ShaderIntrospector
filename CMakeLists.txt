cmake_minimum_required(VERSION 3.20)

# Задаем версию стандарта
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Название проекта
project(shaderIntrospector VERSION 1.0 LANGUAGES CXX)

# Аналогично указываю дирректории заголовков
set(INTROSPECTOR_LIB_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/attributeReflector/attributeReflector.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/uniformReflector/uniformReflector.h
    "include/uniformReflector/traits/traits.h"
    "include/uniformReflector/uniform/uniform_entry.h"
    "include/tools.h"
    "include/uniformBlockReflector/uniformBlockReflector.h"
    "include/uniformBlockReflector/uniformBlockData.h"
    "include/consoleTools.h"
    
    "include/attributeReflector/attributeHandle.h"
 "include/AGreflection.h"   "include/templates/template_iterator.h" "include/templates/opengl_iterator.h" "include/uniformBlockReflector/uniformBlockField.h" "include/uniformBlockReflector/uniformBlockView.h"
 "include/uniformBlockReflector/uniformBlockSequence.h")

# Для удобства задаю все спп файлы в одну переменную
set(INTROSPECTOR_LIB_CPP
	${CMAKE_CURRENT_SOURCE_DIR}/src/attributeReflector/attributeReflector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/uniformReflector/uniformReflector.cpp
    "src/uniformReflector/uniform/uniform_entry.cpp"
    "src/uniformBlockReflector/uniformBlockReflector.cpp"
   )

# Создаю саму либу
add_library(shaderIntrospector STATIC
    ${INTROSPECTOR_LIB_CPP}
    ${INTROSPECTOR_LIB_HEADERS}
)

# Подвязываю икнлуды
target_include_directories(shaderIntrospector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Привязываем доп. либы
find_package(GLEW REQUIRED)
target_link_libraries(shaderIntrospector PUBLIC GLEW::GLEW)

option(BUILD_INTROSPECTOR_EXAMPLE OFF)

if(BUILD_INTROSPECTOR_EXAMPLE)
	add_executable(example_intro example.cpp)

    # Для создания окна нужен глвф
    find_package(glfw3 REQUIRED)
    add_subdirectory(libs/shader)

    target_link_libraries(shaderIntrospector PUBLIC
        shaderLib
    )
    
    # Подвязываем то что нужно
    target_link_libraries(example_intro
        glfw
        shaderIntrospector
    )

    # Эта тема нужна для копирования ассетов в папку с исполняемым файлом
    add_custom_command(TARGET example_intro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:example_intro>/assets
    )
endif()