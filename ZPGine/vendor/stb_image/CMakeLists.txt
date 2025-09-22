cmake_minimum_required(VERSION 3.30)
project(Stb_Image)

add_library(Stb_Image STATIC 
    stb_image.h 
    stb_image_write.h
    stb_image_impl.cpp
)


