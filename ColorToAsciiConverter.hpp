//
//  ColorToAsciiConverter.cpp
//  Camscii
//
//  Created by TJ Johnson on 7/17/18.
//  Copyright © 2018 TJ Johnson. All rights reserved.
//
//  Description: A utility object used to convert a single camera frame into ascii characters
//

#ifndef ColorToAsciiConverter_h
#define ColorToAsciiConverter_h

#include <cmath>
#include <future>
#include <thread>
#include <opencv2/opencv.hpp>

namespace camscii {
    class ColorToAsciiConverter{
    public:
        static ColorToAsciiConverter* getInst() {
            static ColorToAsciiConverter util;
            return &util;
        }
    
        cv::Mat getAsciiFrame(cv::Mat& frame, int height, int width, uint32_t gridSize, int threads=std::thread::hardware_concurrency()-1 ) {
            cv::Mat result = cv::Mat::zeros(height, width, CV_8UC3);

            std::vector<std::future<std::vector<std::pair<char,cv::Point>>>> threaddata;
            for ( int t = 0; t < threads; ++t ) {
                threaddata.push_back(
                std::async([&](int id) -> std::vector<std::pair<char, cv::Point>> {
                    std::vector<std::pair<char, cv::Point>> vecdata;
                    for ( int i = 0; i <= width; i += (gridSize) ) {
                        for ( int j = (id * gridSize); j <= height; j += (gridSize * threads) ){
                            cv::Vec3b avg = getAvgPixel(frame, j, j + gridSize - 1, i, i + gridSize - 1 );
                            uint8_t greyScale = toGreyScale(avg);
                            auto ascii = getCharacter( greyScale );
                            auto point = cv::Point(i,j);
                            
                            vecdata.push_back( std::make_pair( ascii, point ) );
                        }
                    }
                    return vecdata;
                }, t ) );
            }
            
            for ( auto& t : threaddata ) {
                auto data = t.get();
                for ( auto p : data ) {
                    putText(result,p.first,p.second);
                }
            }
            
            //Flip horizontally
            cv::flip(result,result,1);
            
            return result;
        }
        
    private:
        
        /**
         * Converts a pixel value to greyscale
         */
        uint8_t toGreyScale( cv::Vec3b& pixel ) {
            return pixel[0] * 0.21 +
                   pixel[1] * 0.72 +
                   pixel[2] * 0.07;
        }
        
        /**
         * Given a greyscale value from a converted pixel. will
         * return a corresponding character matchign that value
         */
        char getCharacter(uint8_t greyScale) {
            static const std::string allchars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`\'. ";
            return allchars[ ceil( (allchars.length() - 1) * ((double)greyScale) / 255 ) ];
        }
        
        /**
         * Given a frame and a bounding box. This function will compute a new pixel
         * with the average value of all pixels in the box.
         */
        cv::Vec3b getAvgPixel(cv::Mat& frame, int rowStart, int rowEnd, int colStart, int colEnd) {
            uint32_t r = 0,
                     g = 0,
                     b = 0,
                     inst = 0;
            
            for ( int i = rowStart; i <= rowEnd; ++i ) {
                for ( int j = colStart; j <= colEnd; ++j ) {
                    cv::Vec3b& pixel = frame.at<cv::Vec3b>(i,j);
                    r += pixel[0];
                    g += pixel[1];
                    b += pixel[2];
                    ++inst;
                }
            }
            
            cv::Vec3b result;
            result[0] = r / inst;
            result[1] = g / inst;
            result[2] = b / inst;
            return result;
        }
        
        /**
         * Given a frame, a character and a point in the frame: This function will
         * place a character at a point on the screen.
         */
        void putText(cv::Mat& frame,
                     char data,
                     cv::Point& point) {
            std::string str(1, data);
            cv::putText(frame,
                        str,
                        point,
                        CV_FONT_HERSHEY_PLAIN,
                        .4,
                        cv::Scalar(0xff,0xff,0xff));
        }
        
        ColorToAsciiConverter() {};
        ~ColorToAsciiConverter() {};
        ColorToAsciiConverter(const ColorToAsciiConverter&) = delete;
        ColorToAsciiConverter& operator = (const ColorToAsciiConverter&) = delete;
        
    };
}

#endif
