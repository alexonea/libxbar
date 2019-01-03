set(SOURCES
	XNode.cc
	XValue.cc
	XVerb.cc
	XNoun.cc
	XParser.cc
	XParserState.cc
	WordNet.cc
)

add_library(xbar ${SOURCES})
target_link_libraries(xbar WN)
