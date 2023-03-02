#Запрос нужных для проекта библиотек
find_package(Threads REQUIRED)

set(TARGET_LINK_PACKAGES
    ${CMAKE_THREAD_LIBS_INIT}
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set (PQXX /usr/include/pqxx)
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

set(NANOMSG_DEPEND ON)
if (NANOMSG_DEPEND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PC_NANOMSQ libnanomsg)

    if (NOT PC_NANOMSQ_FOUND)
        pkg_check_modules(PC_NANOMSQ REQUIRED nanomsg)
    endif()


    if (PC_NANOMSQ_FOUND)
        list(APPEND TARGET_LINK_PACKAGES ${PC_NANOMSQ_LIBRARIES})
        include_directories(${PC_NANOMSQ_INCLUDE_DIRS})
    else()
        list(APPEND TARGET_LINK_PACKAGES ${PC_NANOMSQ_LIBRARIES} ${PQXX_LIB} ${PQ_LIB})
    endif()
endif()
