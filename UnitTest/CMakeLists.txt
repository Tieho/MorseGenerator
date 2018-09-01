INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include)

SET(APP_SRCS
	QtTestingQtApp.cpp
	)

ADD_EXECUTABLE(MorseGeneratorTest ${APP_SRCS} ${APP_MOCS})

TARGET_LINK_LIBRARIES(MorseGeneratorTest
	Qt5::Core
	Qt5::Multimedia
	MorseGeneratorLib
	)

qt5_use_modules(MorseGeneratorTest Core Multimedia)
