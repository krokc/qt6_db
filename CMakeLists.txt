cmake_minimum_required(VERSION 4.1)
project(DB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/msvc2022_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
        HttpServer
        REQUIRED)

add_executable(DB
        main.cpp
        model/DatabaseManager.cpp
        model/Repositories.cpp
        controller/AppController.cpp
        view/LoginWindow.cpp
        view/RegistrationWindow.cpp
        view/CrudDialogs.cpp
        view/MainWindow.cpp
)
target_link_libraries(DB
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        Qt::Network
        Qt::HttpServer
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt6_bin_dir "${_qt6_qmake_location}" DIRECTORY)

    add_custom_command(TARGET DB POST_BUILD
            COMMAND "${_qt6_bin_dir}/windeployqt.exe"
            --no-translations
            --no-system-d3d-compiler
            "$<TARGET_FILE:DB>"
            COMMENT "Running windeployqt for DB"
    )
endif ()
