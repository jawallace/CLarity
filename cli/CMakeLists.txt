file(GLOB CLARITY_CLI_SOURCES "*.cc")

add_executable(clarity_cli ${CLARITY_CLI_SOURCES})
target_link_libraries(clarity_cli clarity)

