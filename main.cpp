//
//  main.cpp
//  Camscii
//
//  Created by TJ Johnson on 7/12/18.
//  Copyright © 2018 TJ Johnson. All rights reserved.
//

#include <iostream>
#include <boost/program_options.hpp>
#include "OpenCvCam.hpp"
#include "ColorToAsciiConverter.hpp"

using camscii::OpenCvCam;
using camscii::ColorToAsciiConverter;
using namespace boost::program_options;

/**
 * Creates a recording of the camera given a recording path and fps. If path is
 * empty the function will take no action.
 */
void createRecording(OpenCvCam& cam, const std::string& path, double fps);

void mainLoop(OpenCvCam& cam);

/**
 * Operations here are simple. Open the camera and loop through collecting each
 * available frame. Convert each frame to a new frame containing only ascii characters.
 * Use the camera object and display that new frame to the user.
 */
int main(int argc, const char * argv[]) {
    std::string recordingPath;
    double recordingFps = 0;
    options_description opts("Video Conversion Options");
    
    try {
        
        opts.add_options()
        ("help,h", "print usage message")
        ("recording_path,r", value(&recordingPath)->default_value(""), "Video Recording Path")
        ("recording_fps", value(&recordingFps)->default_value(30), "Recording FPS");
        
        variables_map vm;
        store(parse_command_line(argc, argv, opts), vm);
        
        if (vm.count("help")) {
            std::cout << opts << std::endl;
            return 0;
        }
        
        notify(vm);
    } catch (boost::program_options::error& err){
        std::cerr << "ERROR: " << err.what() << std::endl << std::endl;
        std::cerr << opts << std::endl;
    }
    
    OpenCvCam cam("Video Feed");
    
    if ( cam.isReady() ){
        std::cout << "Camera ready to use" << std::endl;
    } else {
        std::cout << "Camera failed to open" << std::endl;
        return -1;
    }
    
    //Create a recording usign the provided options
    createRecording(cam, recordingPath, recordingFps);
    
    //Loops until ESC is hit
    mainLoop(cam);
    
    return 0;
}

void createRecording(OpenCvCam& cam, const std::string& path, double fps){
    if ( !path.empty() ) {
        char codec[] = "MPEG";
        cam.createWriter(path, codec, fps );
    }
}

void mainLoop(OpenCvCam& cam){
    cv::Mat frame;
    while (cam.getNextFrame(frame) ){
        if ( cam.shouldClose() ) {
            std::cout << "Stopping" << std::endl;
            break;
        } else {
            auto converter = ColorToAsciiConverter::getInst();
            
            auto asciiFrame = converter->getAsciiFrame(frame,
                                                       cam.getHeight(),
                                                       cam.getWidth(),
                                                       cam.getGridSize() );
            cam.showFrame(asciiFrame);
            cam.write(asciiFrame);
        }
    }
}
