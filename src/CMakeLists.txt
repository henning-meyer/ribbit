cmake_minimum_required(VERSION 3.5)

PROJECT(ribbit)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra  -Werror -march=native -pthread"  )
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -W -Wall -Wextra  -Werror -march=native -pthread"  )


add_compile_options(-std=c++14 -W -Wall -Wextra  -Werror -march=native -pthread -g -flto)
add_definitions( -D_GLIBCXX_DEBUG=1 )

link_libraries(-std=c++14 -W -Wall -Wextra  -Werror -march=native -pthread -g -flto)


find_package(PkgConfig)

set(NORMAL_SOURCES sessions.cxx libmhd.cxx pages.cxx main.cxx content.cxx html_file.cxx)
set(SPECIAL_SOURCES program_options.cxx)

FOREACH(source ${NORMAL_SOURCES})
# set_source_files_properties(${source} PROPERTIES COMPILE_FLAGS "-include precompiled.hxx")
ENDFOREACH()

ADD_EXECUTABLE(ribbit-demo ${NORMAL_SOURCES} ${SPECIAL_SOURCES})
target_link_libraries(ribbit-demo -pthread -lboost_system -lboost_thread -lboost_program_options -lmicrohttpd)

