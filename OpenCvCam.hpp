//
//  OpenCvCam.h
//  Camscii
//
//  Created by TJ Johnson on 7/12/18.
//  Copyright © 2018 TJ Johnson. All rights reserved.
//
//  Description: This class contains a reference to the openCv camera object.
//

#ifndef OpenCvCam_h
#define OpenCvCam_h

#include <string>

#include <opencv2/opencv.hpp>

namespace camscii {
    class OpenCvCam {
    public:
        OpenCvCam(std::string windowName){
            _cap = std::make_unique<cv::VideoCapture>(0);
            _width = _cap->get(cv::CAP_PROP_FRAME_WIDTH);
            _height = _cap->get(cv::CAP_PROP_FRAME_HEIGHT);
            _windowName = windowName;
            _gridSize = 5;
            cv::namedWindow(_windowName);
        }
        ~OpenCvCam(){}
        
        bool isReady() {
            return _cap->isOpened();
        }

        bool getNextFrame(cv::Mat& frame) {
            return _cap->read(frame);
        }
        
        void drawHorizontalLine (cv::Mat& frame, int row, int colStart, int colEnd) {
            for ( int i = colStart; i < colEnd; ++i ) {
                cv::Vec3b& pixel = frame.at<cv::Vec3b>(row,i);
                pixel[0] = pixel[1] = pixel[2] = 0xff;
            }
        }
        
        void drawVerticalLine (cv::Mat& frame, int col, int rowStart, int rowEnd ) {
            for ( int i = rowStart; i < rowEnd; ++i ) {
                cv::Vec3b& pixel = frame.at<cv::Vec3b>(i,col);
                pixel[0] = pixel[1] = pixel[2] = 0xff;
            }
        }
        
        void drawGrid(cv::Mat& frame){
            for ( int i = _gridSize; i <= _height; i+=_gridSize ) {
                drawHorizontalLine(frame, i, 0, _width);
            }
           
            for ( int j = _gridSize; j <= _width; j+=_gridSize ){
                drawVerticalLine(frame, j, 0, _height);
            }
        }
        
        void showFrame(cv::Mat& frame){
            cv::imshow(_windowName, frame);
        }
        
        bool shouldClose() {
            return cv::waitKey(10) == 27;
        }
        
        double getWidth() {
            return _width;
        }
        
        double getHeight() {
            return _height;
        }
        
        uint32_t getGridSize() {
            return _gridSize;
        }
        
        void createWriter(const std::string& filename,
                          char codec[4],
                          uint32_t fps ) {
            _writer = std::make_unique<cv::VideoWriter>(
                filename,
                CV_FOURCC(codec[0], codec[1], codec[2], codec[3]),
                fps,
                cv::Size(_width,_height)
            );
        }
        
        void write(cv::Mat& frame) {
            if ( _writer ) {
                _writer->write(frame);
            }
        }
        
    private:
        std::unique_ptr<cv::VideoCapture> _cap;
        double _width;
        double _height;
        uint32_t _gridSize;
        std::string _windowName;
        std::unique_ptr<cv::VideoWriter> _writer;
    };
}

#endif /* OpenCvCam_h */
