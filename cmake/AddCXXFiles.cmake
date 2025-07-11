function(add_cxx_files TARGET)

    # Gather private headers from src/
    file(GLOB_RECURSE HEADER_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
    )

    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
        PREFIX "Header Files"
        FILES ${HEADER_FILES}
    )

    target_sources("${TARGET}" PRIVATE ${HEADER_FILES})

    # Gather source files from src/
    file(GLOB_RECURSE SOURCE_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )

    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
        PREFIX "Source Files"
        FILES ${SOURCE_FILES}
    )

    target_sources("${TARGET}" PRIVATE ${SOURCE_FILES})
endfunction()