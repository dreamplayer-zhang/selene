if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

    set(SELENE_COMPILER_OPTIONS -Wall -Wextra -Wpedantic)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 8)
        # GCC 7 emits some spurious warning with -Wconversion enabled, so we don't enable it then.
        set(SELENE_COMPILER_OPTIONS ${SELENE_COMPILER_OPTIONS} -Wconversion)
    endif()

    if (SELENE_WARNINGS_AS_ERRORS)
        set(SELENE_COMPILER_OPTIONS ${SELENE_COMPILER_OPTIONS} -Werror)
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(SELENE_IMG_COMPILER_OPTIONS -Wno-clobbered)
    endif()

    set(SELENE_COMPILER_DEFINITIONS "")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

    set(SELENE_COMPILER_OPTIONS /MP /permissive- /wd4324 /wd4611)

    if (NOT "${CMAKE_GENERATOR}" MATCHES "NMake")
        string(REGEX REPLACE " /W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
        string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(SELENE_COMPILER_OPTIONS ${SELENE_COMPILER_OPTIONS} /W4)
    endif()

    if (SELENE_WARNINGS_AS_ERRORS)
        set(SELENE_COMPILER_OPTIONS ${SELENE_COMPILER_OPTIONS} /WX)
    endif()

    set(SELENE_COMPILER_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

endif()
