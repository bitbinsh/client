cmake_minimum_required(VERSION 3.12)
project(client)

# So boost has some parenthesis that fire some errors that aren't errors. Lets turn those off.
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-parentheses")
endif()
add_subdirectory(third-party/uri)


set(CMAKE_CXX_STANDARD 17)

set(LIBRARIES curl sodium boost_program_options)

add_executable(client src/main.cpp src/DownloadClient.cpp src/DownloadClient.h src/actions.cpp src/actions.h)

include_directories(${CMAKE_SOURCE_DIR}/third-party/uri/include)

foreach(LIBRARY ${LIBRARIES})
    find_library(LIB${LIBRARY}_PATH "${LIBRARY}")
    message(STATUS "Using ${LIBRARY} ${LIB${LIBRARY}_PATH}")
    target_link_libraries(client PRIVATE "${LIB${LIBRARY}_PATH}")
endforeach(LIBRARY)

target_link_libraries(client PRIVATE network-uri)
