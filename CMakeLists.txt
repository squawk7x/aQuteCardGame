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

# Resources directory for icons
if(ANDROID)
    set(ANDROID_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/android/res)

    # Copy launcher icons for Android to the correct location in the APK
    file(GLOB ANDROID_LAUNCHER_ICONS
        ${ANDROID_RESOURCE_DIR}/drawable/ic_launcher.png
        ${ANDROID_RESOURCE_DIR}/drawable-mdpi/ic_launcher.png
        ${ANDROID_RESOURCE_DIR}/drawable-hdpi/ic_launcher.png
        ${ANDROID_RESOURCE_DIR}/drawable-xhdpi/ic_launcher.png
        ${ANDROID_RESOURCE_DIR}/drawable-xxhdpi/ic_launcher.png
        ${ANDROID_RESOURCE_DIR}/drawable-xxxhdpi/ic_launcher.png
    )

    # Copy icons to the build directory to be included in the APK
    foreach(icon ${ANDROID_LAUNCHER_ICONS})
        get_filename_component(icon_dir ${icon} DIRECTORY)
        file(RELATIVE_PATH icon_relative_path ${ANDROID_RESOURCE_DIR} ${icon_dir})
        file(COPY ${icon} DESTINATION ${CMAKE_BINARY_DIR}/res/${icon_relative_path})
    endforeach()

    # # Set Android-specific properties after the target is created
    # set_target_properties(aQuteCardGame PROPERTIES
    #     ANDROID_PACKAGE_SOURCE_DIR ${ANDROID_RESOURCE_DIR}
    #     ANDROID_STL c++_static
    # )
endif()
# For Qt6, use `qt_add_resources` for handling resources
qt_add_resources(RESOURCES resources.qrc)

# Create executable (or shared library for Android)
qt_add_executable(aQuteCardGame
    ${PROJECT_SOURCES}
    ${RESOURCES}  # Include the resources file
)

# Link Qt libraries to the project
target_link_libraries(aQuteCardGame PRIVATE Qt6::Widgets Qt6::Multimedia)

# Finalize the executable if using Qt 6
qt_finalize_executable(aQuteCardGame)

# Install the target
include(GNUInstallDirs)
install(TARGETS aQuteCardGame
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
