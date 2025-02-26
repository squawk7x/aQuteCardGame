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

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Ensure Qt 6 is being used
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia Test)

# Include the src directory for header files include_directories(src)

# # Alle Quell- und Headerdateien aus dem src Ordner einbinden
file(GLOB PROJECT_SOURCES src/*.cpp src/*.h src/*.ui)

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

# Add the test executable Add test executable
add_executable(runTests tests/tst_card.cpp)

# Link GoogleTest properly
target_link_libraries(runTests gtest gtest_main)

# Register the tests
add_test(NAME CardTests COMMAND runTests)
