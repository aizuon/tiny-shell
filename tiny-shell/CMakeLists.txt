set(BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${BASE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${BASE_DIR}/*.hpp")
list(FILTER HEADER_FILES EXCLUDE REGEX "${BASE_DIR}/pch.hpp$")

add_executable(tiny-shell ${SOURCE_FILES} ${HEADER_FILES})

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(tiny-shell PRIVATE Boost::program_options)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(tiny-shell PRIVATE magic_enum::magic_enum)

target_include_directories(tiny-shell PUBLIC "${BASE_DIR}")

target_precompile_headers(tiny-shell PRIVATE "${BASE_DIR}/pch.hpp")
