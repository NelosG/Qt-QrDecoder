cmake_minimum_required(VERSION 3.26)
project(QrDecoder)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (NOT CMAKE_PREFIX_PATH)
    message(FATAL_ERROR "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()


#qzxing download
configure_file(CMakeLists.txt.in qzxing-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qzxing-download)

if (result)
    message(FATAL_ERROR "CMake step for qzxing failed: ${result}")
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qzxing-download)

if (result)
    message(FATAL_ERROR "Build step for qzxing failed: ${result}")
endif ()


#qzxing config
set(QZXING_SORCES ${CMAKE_CURRENT_BINARY_DIR}/qzxing-src/src)

if (POLICY CMP0100)
    set(CMAKE_POLICY_DEFAULT_CMP0100 NEW)
endif ()

set(QZXING_USE_DECODER_QR_CODE ON)
set(QZXING_USE_DECODER_1D_BARCODES ON)
set(QZXING_USE_DECODER_DATA_MATRIX ON)
set(QZXING_USE_DECODER_AZTEC ON)
set(QZXING_USE_DECODER_PDF17 ON)
set(QZXING_USE_DECODER_1D_BARCODES ON)


add_subdirectory(
        ${QZXING_SORCES}
        ${CMAKE_CURRENT_BINARY_DIR}/qzxing-build
)


#QT
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

set(QT Qt${QT_VERSION})
set(REQUIRED_LIBS_QUALIFIED ${QT}::Core ${QT}::Gui ${QT}::Widgets)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output-${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output-${CMAKE_BUILD_TYPE})


if (CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    if (WIN32)
        SET(GUI_TYPE WIN32)
    elseif (APPLE)
        SET(GUI_TYPE MACOSX_BUNDLE)
    endif ()
endif ()


#SRC
file(
        GLOB
        SRC
        src/*
)


set(SOURCES main.cpp ${SRC} resourses/icon.rc)


add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${QZXING_SORCES})
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} qzxing)