# Указываем минимальную требуемую версию CMake
cmake_minimum_required(VERSION 3.16.3)

# Задаем имя проекта
project(PicturesControl1)

# Указываем стандарт языка C++ (в данном случае C++17)
set(CMAKE_CXX_STANDARD 17)

# Говорим CMake, что хотим собрать исполняемый файл с именем 'MyCoolApp'
# из исходного файла 'main.cpp'
add_executable(PicturesControl1 main.cpp)
