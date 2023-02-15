cmake_minimum_required(VERSION 3.8)

project(ssr-pdp-report LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include("${PROJECT_SOURCE_DIR}/cmake/packages.cmake")

set(HEADERS_INCLUDE )
set(SOURCES_INCLUDE )
include("${CMAKE_CURRENT_LIST_DIR}/src/CmakeListsInclude.cmake")
include_directories("${PROJECT_SOURCE_DIR}/src")#хороший ли путь?

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(libpqxx REQUIRED)

set(ssr_pdp_report_lib ${HEADERS_INCLUDE} ${SOURCES_INCLUDE} src/database.hpp)

add_library(libssrpdpreport STATIC ${ssr_pdp_report_lib})

set_target_properties(libssrpdpreport PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(libssrpdpreport PROPERTIES CXX_STANDARD 17)

target_include_directories(libssrpdpreport SYSTEM PUBLIC ${QT_INCLUDES})

#target_link_libraries(sandbox_rti_lib_obj
#    PRIVATE
#)

add_executable(${PROJECT_NAME} "src/main.cpp")


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    libssrpdpreport
        libpqxx::pqxx
)



set(SSR_PDP_REPORT_UNIT_TESTS OFF)
if (SSR_PDP_REPORT_UNIT_TESTS)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif ()
