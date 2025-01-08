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

# Environment-based Qt path for portability
set(CMAKE_PREFIX_PATH $ENV{Qt6_DIR} "~/Qt/6.8.1/gcc_64")

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia Test)

# Collect source files dynamically
file(GLOB PROJECT_SOURCES "*.cpp" "*.h" "*.ui")

# list(FILTER PROJECT_SOURCES EXCLUDE REGEX "testTheQuteCardGame.cpp")

# Add resources
qt_add_resources(RESOURCES resources.qrc)

# Create main executable for the game
qt_add_executable(aQuteCardGame main.cpp ${PROJECT_SOURCES} ${RESOURCES})

# Link Qt libraries for the main executable
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia
                                            Qt6::Test)

# Finalize
qt_finalize_executable(aQuteCardGame)

# --------------------------------------------------------------

# Install main executable
include(GNUInstallDirs)
install(
  TARGETS aQuteCardGame
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# --------------------------------------------------------------

# Add the test source files for the test executable
file(GLOB TEST_SOURCES "testTheQuteCardGame.cpp" "testTheQuteCardGame.h")

# Collect source files dynamically
file(GLOB TEST_PROJECT_SOURCES "*.cpp" "*.h" "*.ui")

# list(FILTER TEST_PROJECT_SOURCES EXCLUDE REGEX "main.cpp")

# Add the test executable
add_executable(testCases ${TEST_SOURCES} ${TEST_PROJECT_SOURCES})

# Link Qt libraries for the test executable
target_link_libraries(testCases PRIVATE Qt6::Widgets Qt6::Widgets
                                        Qt6::Multimedia Qt6::Test)

# Finalize
qt_finalize_executable(testCases)

# --------------------------------------------------------------

# Enable testing and add test executable
enable_testing()
add_test(NAME RunTests COMMAND testCases)

# --------------------------------------------------------------

# Handle Android deployment
if(ANDROID)
  set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")
  if(EXISTS "${ANDROID_PACKAGE_SOURCE_DIR}")
    qt_android_generate_deployment_settings(aQuteCardGame PACKAGE_SOURCE_DIR
                                            "${ANDROID_PACKAGE_SOURCE_DIR}")
  else()
    message(
      WARNING
        "Android package source directory not found: ${ANDROID_PACKAGE_SOURCE_DIR}"
    )
  endif()
endif()
