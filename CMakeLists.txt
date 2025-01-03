cmake_minimum_required(VERSION 3.5)

project(
  aQuteCardGame
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.8.1/gcc_64/lib/cmake/Qt6" ${CMAKE_PREFIX_PATH})

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia)

# List of source files
set(PROJECT_SOURCES
    main.cpp
    table.cpp
    table.h
    card.cpp
    card.h
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
    choosers.h
    choosers.cpp
    README.md)

# For Qt6, use `qt_add_resources` for handling resources
qt_add_resources(RESOURCES resources.qrc)

qt_add_executable(aQuteCardGame ${PROJECT_SOURCES} ${RESOURCES}
                  MANUAL_FINALIZATION)

# Finalize the executable if using Qt 6
qt_finalize_executable(aQuteCardGame)

# Link Qt libraries to the project
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia)

# Install the target
include(GNUInstallDirs)
install(
  TARGETS aQuteCardGame
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set(APP_NAME "aQuteCardGame")

# Specify Android package source directory
if(ANDROID)
  set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")

  # Ensure the directory exists
  if(NOT EXISTS ${ANDROID_PACKAGE_SOURCE_DIR})
    message(
      FATAL_ERROR
        "Android package source directory does not exist: ${ANDROID_PACKAGE_SOURCE_DIR}"
    )
  endif()

  # Generate deployment settings for Android
  set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")
  qt_android_generate_deployment_settings(${APP_NAME} PACKAGE_SOURCE_DIR
                                          "${ANDROID_PACKAGE_SOURCE_DIR}")
endif()
