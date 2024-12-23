cmake_minimum_required(VERSION 3.5)

project(aQuteCardGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Static Compilation
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
	main.cpp
	table.cpp
	table.h
	card.cpp
	card.h
	choosers.cpp
	choosers.h
	blind.cpp
	blind.h
	stack.cpp
	stack.h
	monitor.cpp
	monitor.h
	played.cpp
	played.h
	playable.cpp
	playable.h
	cardvec.cpp
	cardvec.h
	handdeck.cpp
	handdeck.h
	player.cpp
	player.h
	game.cpp
	game.h
	drawn.cpp
	drawn.h
	got.cpp
	got.h
	table.ui

	README.md
)

# # Use Clang as the compiler
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

# # Add the MemorySanitizer flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -g")

# # If you're using clang, you may need to add these extra flags for MemorySanitizer
# if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-memory-track-origins -fno-omit-frame-pointer")
# endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(aQuteCardGame
		MANUAL_FINALIZATION
		${PROJECT_SOURCES}
                resources.qrc
	)
else()
	if(ANDROID)
		add_library(aQuteCardGame SHARED
			${PROJECT_SOURCES}
		)
		# Define properties for Android with Qt 5 after find_package() calls as:
		# set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
	else()
		add_executable(aQuteCardGame
			${PROJECT_SOURCES}
		)
	endif()
endif()

target_link_libraries(aQuteCardGame PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)


# Enable AddressSanitizer
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
# 	target_compile_options(aQuteCardGame PRIVATE -fsanitize=address)
# 	target_link_options(aQuteCardGame PRIVATE -fsanitize=address)
# endif()


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
	set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.aQuteCardGame)
endif()

set_target_properties(aQuteCardGame PROPERTIES
	${BUNDLE_ID_OPTION}
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS aQuteCardGame
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(aQuteCardGame)
endif()
