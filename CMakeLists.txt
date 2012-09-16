cmake_minimum_required(VERSION 2.8)
project (libAeDaemon C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

include(EnsureOutOfSourceBuild)

EnsureOutOfSourceBuild("LibAeDaemon requires out-of-source build.")
find_package(Apr)
find_package(AprUtil)

add_definitions(${LIBAPR_DEFINITIONS} ${LIBAPR-UTIL_DEFINITIONS})
include_directories(${LIBAPR_INCLUDE_DIRS} ${LIBAPR-UTIL_INCLUDE_DIRS})

add_executable(aedaemon src/daemon.c)
target_link_libraries(aedaemon ${LIBAPR_LIBRARIES} ${LIBAPR-UTIL_LIBRARIES})

add_executable(aedaemon1 src/daemon1.c)
target_link_libraries(aedaemon1 ${LIBAPR_LIBRARIES} ${LIBAPR-UTIL_LIBRARIES})

add_executable(aprdaemon src/aprdaemon.c)
target_link_libraries(aprdaemon ${LIBAPR_LIBRARIES} ${LIBAPR-UTIL_LIBRARIES})

install(TARGETS aedaemon aedaemon1 aprdaemon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

# For future shared lib
#install(TARGETS mySharedLib DESTINATION /some/full/path)
install(FILES README DESTINATION /etc/libaedaemon)

# Install proper init script
if(LIBAEDAEMON_LINUX_DISTRIBUTION STREQUAL "ArchLinux")
	install(FILES scripts/initscript-arch.sh
		  DESTINATION /etc/rc.d
		  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
		  RENAME libaedaemon)
endif(LIBAEDAEMON_LINUX_DISTRIBUTION STREQUAL "ArchLinux")
