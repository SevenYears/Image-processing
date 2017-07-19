cmake_minimum_required(VERSION 2.8)
PROJECT(ImageProcessing) 
 
FIND_PACKAGE( opencv REQUIRED )   
INCLUDE_DIRECTORIES( ${ShowImage_SOURCE_DIR} )

ADD_EXECUTABLE(ImageProcessing main.cpp)  
TARGET_LINK_LIBRARIES (ImageProcessing ${OpenCV_LIBS} ) 