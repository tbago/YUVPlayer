project(base)

set(BASE_LIBRARY_NAME "base")

aux_source_directory("." BASE_SRC)

add_library(${BASE_LIBRARY_NAME} STATIC ${BASE_SRC})