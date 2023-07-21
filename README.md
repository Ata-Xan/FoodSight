# FoodSight
# Food Recognition and Leftover Estimation System

## Description

This project is a computer vision system designed to recognize food items and estimate leftovers in a canteen setting. The system uses a combination of techniques including Hough Circle Transform for localization, Speeded-Up Robust Features (SURF) algorithm for food recognition, and Hue, Saturation, and Value (HSV) color space for segmentation.

## Features

- Food item recognition and localization in tray images
- Segmentation of each food item
- Estimation of the quantity of leftovers

## Installation


## Prerequisites
(you can use the docker file as well to install all the prerequisites)
Before you begin, ensure you have met the following requirements:

* You have installed the latest version of [CMake](https://cmake.org/download/).
* You have a Windows/Linux/Mac machine with a recent version of C++ compiler.
* You have installed [OpenCV](https://opencv.org/releases/).
* You have installed [openCV_Contrib] https://github.com/opencv/opencv_contrib.git 

## Installing <FoodSight>

To install <FoodSight>, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/Ata-Xan/FoodSight.git
    ```

2. Navigate to the project directory:

    ```bash
    cd FoodSight
    ```

3. Create a new build directory and navigate into it:

    ```bash
    mkdir build && cd build
    ```

4. Run CMake to configure the project:

    ```bash
    cmake ..
    ```

5. Compile the project:

    ```bash
    make
    ```