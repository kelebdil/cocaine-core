FUNCTION(LOCATE_LIBRARY VARIABLE HEADER LIBRARY)
    IF(${VARIABLE}_INCLUDE_DIRS AND ${VARIABLE}_LIBRARY_DIRS)
        RETURN()
    ENDIF()

    FIND_PATH(${VARIABLE}_INCLUDE_DIRS NAMES ${HEADER} PATH_SUFFIXES ${ARGN})
    FIND_LIBRARY(${VARIABLE}_LIBRARIES NAMES ${LIBRARY} PATH_SUFFIXES ${ARGN})
    GET_FILENAME_COMPONENT(${VARIABLE}_LIBRARY_DIRS ${${VARIABLE}_LIBRARIES} PATH CACHE)
    
    STRING(TOLOWER ${VARIABLE} LIBRARY_NAME)

    IF(NOT ${VARIABLE}_INCLUDE_DIRS OR NOT ${VARIABLE}_LIBRARY_DIRS)
        MESSAGE(FATAL_ERROR "${LIBRARY_NAME} development files are required to build.")
    ELSE()
        MESSAGE(STATUS "Found ${LIBRARY_NAME}: ${${VARIABLE}_LIBRARIES}")
    ENDIF()
ENDFUNCTION()


function(add_clang_static_analysis target)
    get_target_property(SRCs ${target} SOURCES)
    add_library(${target}_analyze OBJECT EXCLUDE_FROM_ALL ${SRCs})
    set_target_properties(${target}_analyze PROPERTIES
                          COMPILE_OPTIONS "--analyze"
                          EXCLUDE_FROM_DEFAULT_BUILD true)
endfunction()