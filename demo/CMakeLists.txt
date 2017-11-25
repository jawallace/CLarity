set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

file(GLOB CLARITY_DEMO_SOURCES "src/*.cc" "include/*.h")
add_executable(clarity_demo ${CLARITY_DEMO_SOURCES} "../third_party/qdarkstylesheet/qdarkstyle/style.qrc")
target_include_directories(clarity_demo PRIVATE "include" ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(clarity_demo clarity Qt5::Core Qt5::Widgets)

