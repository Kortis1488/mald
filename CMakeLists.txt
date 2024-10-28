cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(Mald)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Укажите путь к папке с CMake файлом SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.1/lib/cmake/SFML")

# Найдите SFML
find_package(SFML 2.6 COMPONENTS graphics window system audio network REQUIRED)

# Укажите исходные файлы
add_executable(Mald src/main.cpp)

# Линкуем SFML к вашему проекту
target_link_libraries(Mald PRIVATE sfml-window sfml-audio sfml-system sfml-network sfml-graphics)

# Укажите пути к заголовочным файлам
target_include_directories(Mald PRIVATE ${SFML_INCLUDE_DIR})

# Путь к DLL файлам SFML
set(SFML_DLL_DIR "X:\\mald\\lib\\SFML-2.6.1\\bin")

# Копируем необходимые DLL файлы в выходной каталог
add_custom_command(TARGET Mald POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLL_DIR}/sfml-system-d-2.dll
        ${SFML_DLL_DIR}/sfml-window-d-2.dll
        ${SFML_DLL_DIR}/sfml-graphics-d-2.dll
        ${SFML_DLL_DIR}/sfml-audio-d-2.dll
        ${SFML_DLL_DIR}/sfml-network-d-2.dll
        $<TARGET_FILE_DIR:Mald>
)

