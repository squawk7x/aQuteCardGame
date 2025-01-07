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
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Multimedia)

# Collect source files dynamically
file(GLOB PROJECT_SOURCES "*.cpp" "*.h" "*.ui")

# Add resources
qt_add_resources(RESOURCES resources.qrc)

# Build executable
qt_add_executable(aQuteCardGame ${PROJECT_SOURCES} ${RESOURCES})
qt_finalize_executable(aQuteCardGame)

# Link Qt libraries
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia)

# Install
include(GNUInstallDirs)
install(
  TARGETS aQuteCardGame
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

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
