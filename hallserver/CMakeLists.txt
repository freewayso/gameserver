SET(APP hallserver)

INCLUDE_DIRECTORIES( . network protocol )
INCLUDE_DIRECTORIES( /usr/include/mysql )
INCLUDE_DIRECTORIES( ${SERVER_SOURCE_DIR}/share )


LINK_DIRECTORIES( /usr/lib64/mysql )


FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/util SRC_LIST)
#AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
					 
ADD_EXECUTABLE(${APP} ${SRC_LIST})
TARGET_LINK_LIBRARIES(${APP} pb common epoll util pthread mysqlclient profiler protobuf z scew expat curl hiredis)

IF(NOT DEFINED USE_ASAN)
	TARGET_LINK_LIBRARIES(${APP} tcmalloc)
	set_target_properties(${APP} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
	cotire(${APP})
ENDIF()
