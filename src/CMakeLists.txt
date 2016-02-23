cmake_minimum_required(VERSION 2.6)

PROJECT(ribbit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra  -Werror -march=native -pthread"  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -W -Wall -Wextra  -Werror -march=native -pthread"  )


add_definitions( -D_GLIBCXX_DEBUG=1 )


find_package(PkgConfig)

ADD_EXECUTABLE(ribbit-demo sessions.cxx libmhd.cxx pages.cxx main.cxx)
target_link_libraries(ribbit-demo -pthread -lboost_system -lboost_thread -lmicrohttpd)


