file(GLOB_RECURSE source "*.cpp" "*.h")

add_library(libAsio STATIC ${source})

if(NOT WIN32)
    target_compile_options(libAsio PRIVATE -std=c++11)
    find_package(Threads)
    target_link_libraries(libAsio ${CMAKE_THREAD_LIBS_INIT})
endif()

target_include_directories(libAsio
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        ${CMAKE_SOURCE_DIR}/3rd_party/asio/include
    PRIVATE
        /usr/local/include
)

if(NOT WIN32)
    target_link_directories(libAsio PUBLIC /usr/local/lib)
endif()