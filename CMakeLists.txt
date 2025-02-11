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
set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}" "$ENV{HOME}/Qt/6.8.2/gcc_64")

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia Test)

# Collect source files dynamically
file(GLOB PROJECT_SOURCES "*.cpp" "*.h" "*.ui")

# Exclude test files from the main build
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "qTestTheQuteCardGame.cpp")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "qTestTheQuteCardGame.h")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "gTestTheQuteCardGame.cpp")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "gTestTheQuteCardGame.h")

# Add resources
qt_add_resources(RESOURCES resources.qrc)

# Create main executable for the game
qt_add_executable(aQuteCardGame ${PROJECT_SOURCES} ${RESOURCES})

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
if(NOT ANDROID)
  # Enable testing
  enable_testing()

  # Collect source files dynamically for the test project
  file(GLOB TEST_PROJECT_SOURCES ${PROJECT_SOURCES})
  list(FILTER TEST_PROJECT_SOURCES EXCLUDE REGEX "main.cpp")

  # QtTest Unit Tests
  file(GLOB Q_TEST_SOURCES "qTestTheQuteCardGame.cpp" "qTestTheQuteCardGame.h")

  # Add the test executable
  add_executable(qTestTheQuteCardGame ${Q_TEST_SOURCES} ${TEST_PROJECT_SOURCES})

  # Ensure output directory is set for tests
  set_target_properties(qTestTheQuteCardGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                        ${CMAKE_BINARY_DIR}/bin)

  # Link Qt libraries for the test executable
  target_link_libraries(qTestTheQuteCardGame PRIVATE Qt6::Core Qt6::Widgets
                                                     Qt6::Multimedia Qt6::Test)

  # Add test to CTest
  add_test(NAME RunTests COMMAND qTestTheQuteCardGame)

  # --------------------------------------------------------------
  # GoogleTest Setup
  find_package(GTest REQUIRED)

  # Include directories for Google Test and Google Mock
  include_directories(${GTEST_INCLUDE_DIRS})

  # Google Test Sources
  file(GLOB G_TEST_SOURCES "gTestTheQuteCardGame.cpp" "gTestTheQuteCardGame.h")

  # Add Google Test executable
  add_executable(gTestTheQuteCardGame ${G_TEST_SOURCES} ${TEST_PROJECT_SOURCES})

  # Link Qt and Google Test libraries
  target_link_libraries(
    gTestTheQuteCardGame PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia
                                 GTest::GTest GTest::Main)
endif()
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
