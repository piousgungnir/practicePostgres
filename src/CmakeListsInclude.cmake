
set(WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

include_directories("${CMAKE_CURRENT_LIST_DIR}")

# файлы только заголовочные, у которых нет пары-исходника
set(HEADERS_ONLY
    "domain/Answer"
    "domain/CoordinatePoint"
    "domain/Group"
    "domain/modes_v3"
    "domain/Timestamp"
    "domain/POIProtocol"
    "domain/azimuth/IAzimuthConsumer"
    "domain/azimuth/INorthDependent"
)
# файлы только исходники, у которых нет пары-заголовочника
set(SOURCES_ONLY )
# файлы классов исходные + заголовочные
set(CLASSES
    "domain/azimuth/Azimuth"
    "usecases/PdpReport"
)

foreach (class ${HEADERS_ONLY})
        list(APPEND HEADERS_INCLUDE "${WORKING_DIRECTORY}/${class}.h")
endforeach ()

foreach (class ${SOURCES_ONLY})
        list(APPEND SOURCES_INCLUDE "${WORKING_DIRECTORY}/${class}.cpp")
endforeach ()

foreach (class ${CLASSES})
        list(APPEND SOURCES_INCLUDE "${WORKING_DIRECTORY}/${class}.cpp")
        list(APPEND HEADERS_INCLUDE "${WORKING_DIRECTORY}/${class}.h")
endforeach ()
