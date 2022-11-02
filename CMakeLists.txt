cmake_minimum_required(VERSION 3.17)
project(saper)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH C:/Qt/5.15.2/msvc2019_64)
set(CMAKE_INSTALL_PREFIX ./)
set(CMAKE_CXX_STANDARD 14)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Gui Sql Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui Sql Svg REQUIRED)

add_executable(saper src/main.cpp src/game_field.cpp src/game_field.h src/game_objects/field_cell.cpp src/game_objects/field_cell.h src/game_over.cpp src/game_over.h)
target_link_libraries(saper Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Svg)

install(TARGETS saper DESTINATION bin)

install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Sql> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Svg> DESTINATION bin)

install(FILES $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers)
install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms)
install(FILES $<TARGET_FILE:Qt5::QWindowsDirect2DIntegrationPlugin> DESTINATION bin/platforms)

install(FILES db/database.db DESTINATION bin)
install(DIRECTORY assets/ DESTINATION bin/assets)
install(DIRECTORY docs/ DESTINATION bin/docs)