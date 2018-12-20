set(SOURCES
	xbar-analyze.cpp
)

add_executable(xbar-analyze ${SOURCES})
target_link_libraries(xbar-analyze xbar)
