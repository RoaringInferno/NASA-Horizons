file(GLOB_RECURSE GLOBAL_SOURCES global/*.cpp)
file(GLOB_RECURSE GLOBAL_HEADERS global/*.hpp)

file(GLOB_RECURSE HORIZONPULL_SOURCES horizonpull/*.cpp)
file(GLOB_RECURSE HORIZONPULL_HEADERS horizonpull/*.hpp)
target_sources(horizonpull PRIVATE ${HORIZONPULL_SOURCES} ${HORIZONPULL_HEADERS} ${GLOBAL_SOURCES} ${GLOBAL_HEADERS})
target_include_directories(horizonpull PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/global)

file(GLOB_RECURSE HORIZONREAD_SOURCES horizonread/*.cpp)
file(GLOB_RECURSE HORIZONREAD_HEADERS horizonread/*.hpp)
target_sources(horizonread PRIVATE ${HORIZONREAD_SOURCES} ${HORIZONREAD_HEADERS} ${GLOBAL_SOURCES} ${GLOBAL_HEADERS})
target_include_directories(horizonread PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/global)

file(GLOB_RECURSE HORIZONS_SOURCES horizons/*.cpp)
file(GLOB_RECURSE HORIZONS_HEADERS horizons/*.hpp)
target_sources(horizons PRIVATE ${HORIZONS_SOURCES} ${HORIZONS_HEADERS})
target_include_directories(horizons PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/global)