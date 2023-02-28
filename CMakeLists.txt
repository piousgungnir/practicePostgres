cmake_minimum_required(VERSION 3.8)

project(ssr-pdp-report LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(HEADERS_INCLUDE )
set(SOURCES_INCLUDE )
include("${CMAKE_CURRENT_LIST_DIR}/src/CmakeListsInclude.cmake")
include_directories("${PROJECT_SOURCE_DIR}/src")#хороший ли путь?
include_directories("${PROJECT_SOURCE_DIR}/src/proto_cpp")
include_directories("${PROJECT_SOURCE_DIR}/src/proto_cpp/mvrl-k")


set(ssr_pdp_report_lib ${HEADERS_INCLUDE} ${SOURCES_INCLUDE})

add_library(libssrpdpreport STATIC ${ssr_pdp_report_lib})

set_target_properties(libssrpdpreport PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(libssrpdpreport PROPERTIES CXX_STANDARD 17)

#define TARGET_LINK_PACKAGES
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include("${PROJECT_SOURCE_DIR}/cmake/packages.cmake")

target_link_libraries(libssrpdpreport
    PRIVATE
    ${TARGET_LINK_PACKAGES}
)

add_executable(${PROJECT_NAME} "src/main.cpp")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    libssrpdpreport
    ${TARGET_LINK_PACKAGES}
)



set(SSR_PDP_REPORT_UNIT_TESTS OFF)
if (SSR_PDP_REPORT_UNIT_TESTS)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif ()
