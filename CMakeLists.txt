cmake_minimum_required( VERSION 2.8.3 )

if ( NOT DEFINED TOP_CMAKE )
    set ( TOP_CMAKE 1 )

project(CAMSCII)

include(CMakeParseArguments)

macro(debug_msg msg)
    if(DEFINED CAMSCII_CMAKE_DEBUG AND CAMSCII_CMAKE_DEBUG MATCHES 1)
        message( "DEBUG: ${msg}" )
    endif()
endmacro(debug_msg)

macro(camscii_executable target)
    set(options TEST GTEST)
    set(multiValueArgs INCLUDES CFLAGS DEFINES LIBDIRS LINKS SOURCES DEPENDS OUTDIR)
    cmake_parse_arguments(ARGUMENTS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    debug_msg( "target ${target}")
    debug_msg( "sources ${ARGUMENTS_SOURCES}" )
    add_executable(${target} ${ARGUMENTS_SOURCES})
    if(ARGUMENTS_OUTDIR)
        debug_msg( "setting output dir to ${ARGUMENTS_OUTDIR}" )
        set_property(TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${ARGUMENTS_OUTDIR})
    endif()
    if(ARGUMENTS_CFLAGS)
        debug_msg( "cflags ${ARGUMENTS_CFLAGS}")
        target_compile_options(${target} PRIVATE ${ARGUMENTS_CFLAGS})
    endif()
    if(ARGUMENTS_DEFINES)
        debug_msg( "defines ${ARGUMENTS_DEFINES}")
        target_compile_definitions(${target} PRIVATE ${ARGUMENTS_DEFINES})
    endif()
    if(ARGUMENTS_DEPENDS)
        debug_msg( "depends ${ARGUMENTS_DEPENDS}" )
        add_dependencies(${target} ${ARGUMENTS_DEPENDS})
    endif()
    if(ARGUMENTS_INCLUDES)
        debug_msg( "includes ${ARGUMENTS_INCLUDES}")
        target_include_directories(${target} PRIVATE ${ARGUMENTS_INCLUDES})
    endif()
    if(ARGUMENTS_LINKS)
        debug_msg( "links ${ARGUMENTS_LINKS}")
        if(ARGUMENTS_LIBDIRS)
            unset(my_ld_flags)
            foreach(dir IN LISTS ARGUMENTS_LIBDIRS)
                list(APPEND my_ld_flags "-L${dir}")
                list(APPEND my_ld_flags "-Wl,-rpath,${dir}")
            endforeach()
            list(REMOVE_DUPLICATES my_ld_flags)
            debug_msg( "my_ld_flags ${my_ld_flags}")
            target_link_libraries(${target} PRIVATE ${my_ld_flags} ${ARGUMENTS_LINKS})
        else()
            target_link_libraries(${target} PRIVATE ${ARGUMENTS_LINKS})
        endif()
    endif()
    if(ARGUMENTS_TEST)
        if(ARGUMENTS_OUTDIR)
            add_test(NAME ${target} WORKING_DIRECTORY ${ARGUMENTS_OUTDIR} COMMAND ${target})
        else()
            add_test(NAME ${target} COMMAND ${target})
        endif()
        add_dependencies(build_tests ${target})
    endif()
    if(ARGUMENTS_GTEST)
        if(ARGUMENTS_OUTDIR)
            add_test(NAME ${target} WORKING_DIRECTORY ${ARGUMENTS_OUTDIR} COMMAND ${target} --gtest_output=xml)
        else()
            add_test(NAME ${target} COMMAND ${target} --gtest_output=xml)
        endif()
        add_dependencies(build_tests ${target})
    endif()
endmacro(camscii_executable)

set(CAMSCII_CXX_FLAGS -Wall -w -g -O3 -std=c++1y )
set(CAMSCII_CXX_DEFINES "" )
set(CAMSCII_INCLUDES /usr/local/include )
set(CAMSCII_LIBDIRS /usr/local/lib )
set(OPENCV_LIBS opencv_aruco opencv_bgsegm opencv_bioinspired opencv_calib3d
                opencv_ccalib opencv_core opencv_datasets opencv_dnn
                opencv_dnn_objdetect opencv_dpm opencv_face opencv_features2d
                opencv_flann opencv_fuzzy opencv_hfs opencv_highgui opencv_img_hash
                opencv_imgcodecs opencv_imgproc opencv_line_descriptor opencv_ml
                opencv_objdetect opencv_optflow opencv_phase_unwrapping opencv_photo
                opencv_plot opencv_reg opencv_rgbd opencv_saliency opencv_shape
                opencv_stereo opencv_stitching opencv_structured_light opencv_superres
                opencv_surface_matching opencv_tracking opencv_video
                opencv_videoio opencv_videostab opencv_xfeatures2d opencv_ximgproc
                opencv_xobjdetect opencv_xphoto )

set(EXE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.cpp )

camscii_executable ( Camscii 
    CFLAGS ${CAMSCII_CXX_FLAGS}
    DEFINES ${CAMSCII_CXX_DEFINES}
    INCLUDES ${CAMSCII_INCLUDES} 
    LIBDIRS ${CAMSCII_LIBDIRS}
    LINKS boost_program_options
          ${OPENCV_LIBS}
    OUTDIR ${CMAKE_CURRENT_LIST_DIR}
    SOURCES ${EXE_SOURCES}
)
endif()

