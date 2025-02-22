cmake_minimum_required(VERSION 3.5)

project(
  aQuteCardGame
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Environment-based Qt path for portability
set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}" "$ENV{HOME}/Qt/6.8.2/gcc_64")

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia Test)

# Collect source files dynamically
file(GLOB PROJECT_SOURCES "*.cpp" "*.h" "*.ui" "*.log")

# Exclude test files from the main build
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "qTestTheQuteCardGame.cpp")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "qTestTheQuteCardGame.h")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "gTestTheQuteCardGame.cpp")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "gTestTheQuteCardGame.h")

# Add resources
qt_add_resources(RESOURCES resources.qrc)

# Create main executable for the game
qt_add_executable(aQuteCardGame MANUAL_FINALIZATION ${PROJECT_SOURCES}
                  ${RESOURCES})

# Link Qt libraries for the main executable
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia
                                            Qt6::Test)

# Finalize main executable
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

# Handle Android deployment (if applicable)
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

# --------------------------------------------------------------
# Enable testing
enable_testing()

# GoogleTest & GoogleMock Setup
set(GTEST_DIR /usr/share/googletest)
set(GMOCK_DIR /usr/share/googletest/googlemock)

# Include GoogleTest and GoogleMock
include_directories(${GTEST_DIR}/googletest/include ${GMOCK_DIR}/include)

# Specify the binary directory for GoogleTest
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/googletest-build)

# Add the test executable
add_executable(runTests tst_cardtest.cpp card.cpp)

# Link the Qt6 and GoogleTest libraries to your test executable
target_link_libraries(runTests Qt6::Widgets gtest gtest_main gmock gmock_main)

# Register the tests
add_test(NAME CardTests COMMAND runTests)
