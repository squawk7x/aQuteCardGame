cmake_minimum_required(VERSION 3.5)

project(aQuteCardGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia)

# List of source files
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

# For Qt6, use `qt_add_resources` for handling resources
qt_add_resources(RESOURCES resources.qrc)

qt_add_executable(aQuteCardGame
    ${PROJECT_SOURCES}
    ${RESOURCES}
)

# Link Qt libraries to the project
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia)

# Android-specific settings
if(ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
    )
endif()


# Finalize the executable if using Qt 6
qt_finalize_executable(aQuteCardGame)

# Install the target
include(GNUInstallDirs)
install(TARGETS aQuteCardGame
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
