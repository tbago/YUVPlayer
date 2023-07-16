project(media_base)

set(MEDIA_BASE_LIBRARY_NAME "mediabase")

aux_source_directory("." MEDIA_BASE_SRC)

add_library(${MEDIA_BASE_LIBRARY_NAME} STATIC ${MEDIA_BASE_SRC})

target_include_directories(${MEDIA_BASE_LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})