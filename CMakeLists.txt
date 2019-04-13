CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(ep_mthread)

SET(CMAKE_CXX_COMPILER g++)

ADD_COMPILE_OPTIONS("-g")
SET(SRCS
	src/main.cpp
	src/Response.cpp
	src/Requestion.cpp
	src/Server.cpp
)

ADD_EXECUTABLE(ep_mthread ${SRCS})
TARGET_LINK_LIBRARIES(ep_mthread pthread)