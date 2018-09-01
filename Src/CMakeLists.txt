SET(TARGET MorseGeneratorLib)

FIND_PACKAGE(Qt5Core)

INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/Include
	)

QT5_WRAP_CPP(MOC_SRCS
	../Include/MorseGenerator.h
	)

SET(APP_SRCS
	MorseGenerator.cpp
	WavExporter.cpp
	${MOC_SRCS}
	)

ADD_LIBRARY(${TARGET} ${APP_SRCS})

qt5_use_modules(${TARGET} Core)
