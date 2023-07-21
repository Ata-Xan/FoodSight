#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
#include <vector>
#include <opencv2/core/version.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
// ===================First stage feature extraction using SURF algorithm++++++++++++++++++++++++++++++++++++
// int main() {
//     // Set directory paths
//     std::string image_folder_path = "../foods_image_by_type";
//     std::string output_folder_path = "../keypoints_by_food_type";

// // Create SURF detector and extractor
//     cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create();
    
//     // Loop through each file in the images folder
//     for (const auto &entry : fs::directory_iterator(image_folder_path)) {
//         std::string image_path = entry.path().string();
        
//         // Read image
//         cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
        
//         // Check if the image was loaded successfully
//         if (img.empty()) {
//             std::cerr << "Error loading image: " << image_path << std::endl;
//             continue;
//         }
        
//         // Detect SURF keypoints and descriptors
//         std::vector<cv::KeyPoint> keypoints;
//         cv::Mat descriptors;
//         surf->detectAndCompute(img, cv::noArray(), keypoints, descriptors);
        
//         // Save the descriptors
//         std::string descriptor_filename = output_folder_path + "/" + entry.path().stem().string() + ".xml";
//         cv::FileStorage fileStorage(descriptor_filename, cv::FileStorage::WRITE);
//         fileStorage << "descriptors" << descriptors;
//         fileStorage.release();
//     }
    
//     return 0;
// }


///////////////////////////////////////////////// hsv trackbar ////////////////////////////////////////
// void on_trackbar(int, void*) {}

// cv::Mat computeLBP(cv::Mat& src) {
//     cv::Mat lbp = cv::Mat::zeros(src.size(), CV_8UC1);
//     for (int i = 1; i < src.rows - 1; i++) {
//         for (int j = 1; j < src.cols - 1; j++) {
//             unsigned char center = src.at<unsigned char>(i, j);
//             unsigned char code = 0;
//             code |= (src.at<unsigned char>(i-1, j-1) >= center) << 7;
//             code |= (src.at<unsigned char>(i-1, j  ) >= center) << 6;
//             code |= (src.at<unsigned char>(i-1, j+1) >= center) << 5;
//             code |= (src.at<unsigned char>(i  , j+1) >= center) << 4;
//             code |= (src.at<unsigned char>(i+1, j+1) >= center) << 3;
//             code |= (src.at<unsigned char>(i+1, j  ) >= center) << 2;
//             code |= (src.at<unsigned char>(i+1, j-1) >= center) << 1;
//             code |= (src.at<unsigned char>(i  , j-1) >= center) << 0;
//             lbp.at<unsigned char>(i, j) = code;
//         }
//     }
//     return lbp;
// }

// int main() {
//     // Load the image
//     cv::Mat image = cv::imread("../Food_leftover_dataset_1/tray1/food_image.jpg_1.jpg");
//     if (image.empty()) {
//         std::cerr << "Error loading image!" << std::endl;
//         return -1;
//     }

//     // Convert the image to grayscale for LBP
//     cv::Mat gray;
//     cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

//     // Compute LBP
//     cv::Mat lbp = computeLBP(gray);

//     // Convert the image to the HSV color space for color segmentation
//     cv::Mat hsv;
//     cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

//     // Create window
//     cv::namedWindow("Segmentation", cv::WINDOW_NORMAL);

//     // Create trackbars for HSV segmentation
//     int hMin = 0, hMax = 180, sMin = 0, sMax = 255, vMin = 0, vMax = 255;
//     cv::createTrackbar("Hue min", "Segmentation", &hMin, 180, on_trackbar);
//     cv::createTrackbar("Hue max", "Segmentation", &hMax, 180, on_trackbar);
//     cv::createTrackbar("Saturation min", "Segmentation", &sMin, 255, on_trackbar);
//     cv::createTrackbar("Saturation max", "Segmentation", &sMax, 255, on_trackbar);
//     cv::createTrackbar("Value min", "Segmentation", &vMin, 255, on_trackbar);
//     cv::createTrackbar("Value max", "Segmentation", &vMax, 255, on_trackbar);

//     // Loop for real-time segmentation adjustment
//     while (true) {
//         // Threshold the image in HSV space
//         cv::Mat mask;
//         cv::inRange(hsv, cv::Scalar(hMin, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask);

//         // Combine the original image and the mask
//         cv::Mat result;
//         cv::bitwise_and(image, image, result, mask);

//         // Here you might add additional logic to use LBP texture information to refine segmentation.
//         // For example, you can use the LBP values to help differentiate between different textures.
//         // As stated previously, you need to define a strategy for combining the texture information
//         // with the color information to segment the types of pasta.

//         // Display the result
//         cv::imshow("Segmentation", result);
//         // cv::imshow("LBP", lbp);

//         // Break the loop if 'Esc' key is pressed
//         char key = static_cast<char>(cv::waitKey(30));
//         if (key == 27) {
//             break;
//         }
//     }

//     return 0;
// }


// ####################################################################### the main project ########################################
struct ColorRange {
    std::string name;
    cv::Scalar lowerb;
    cv::Scalar upperb;
    cv::Vec4b color;
};

struct ObjectMatch {
    std::string name;
    float percentage;
};
void annotateImageWithFoods(const std::string* foodsNames, const std::vector<ColorRange>& colorRanges, cv::Mat& testImage) {
    int margin = 10;
    int yOffset = 0;

    for (int i = 0; i < 10; ++i) { 
        std::string foodName = foodsNames[i];

        if (foodName.empty()) {
            break; 
        }

        // Remove trailing underscore or numbers
        size_t underscorePos = foodName.find('_');
        if (underscorePos != std::string::npos) {
            foodName = foodName.substr(0, underscorePos);
        }
        size_t digitPos = foodName.find_last_of("0123456789");
        if (digitPos != std::string::npos) {
            foodName = foodName.substr(0, digitPos);
        }

        
        const ColorRange* colorRange = nullptr;
        for (const auto& cr : colorRanges) {
            if (cr.name == foodName) {
                colorRange = &cr;
                break;
            }
        }

        if (colorRange) {
            
            int baseline = 0;
            cv::Size textSize = cv::getTextSize(foodName, cv::FONT_HERSHEY_SIMPLEX, 1.0, 2, &baseline);

            
            cv::Rect textBackgroundRect(margin, yOffset, textSize.width + 2 * margin, textSize.height + 2 * margin);

            
            cv::rectangle(testImage, textBackgroundRect, colorRange->color, cv::FILLED);

            
            cv::putText(testImage, foodName, cv::Point(margin, yOffset + textSize.height + margin),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

            
            yOffset += textBackgroundRect.height + margin;
        }
    }
}
void colorizeSpecificObjects(cv::Mat& bigImg, cv::Mat& mask, const std::vector<ColorRange>& topMatches, int minPixelCount) {
    // Extract sub image
    cv::Mat subImg;
    bigImg.copyTo(subImg, mask);

    // Convert the image to HSV color space
    cv::Mat hsv;
    cv::cvtColor(subImg, hsv, cv::COLOR_BGR2HSV);
    bool bounded = false;
    int textHeight = 30;
    int margin = 10;
    int yOffset = 0;
    for (const auto& cr : topMatches) {
        // Threshold the HSV image to get only colors in range
        cv::Mat colorMask;
        cv::inRange(hsv, cr.lowerb, cr.upperb, colorMask);

        // Find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(colorMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Check the pixel count, if it's more than minPixelCount, colorize the objects
        if (cv::countNonZero(colorMask) > minPixelCount) {
            std::vector<cv::Point> all_contours;
            // Loop through the contours and colorize each object separately
            for (size_t i = 0; i < contours.size(); i++) {
                all_contours.insert(all_contours.end(), contours[i].begin(), contours[i].end());

                // Create a temporary overlay image for this contour
                cv::Mat overlay(subImg.size(), CV_8UC4, cv::Scalar(0, 0, 0, 0));

                // Fill the contour on the overlay image with the desired color and transparency
                cv::drawContours(overlay, contours, static_cast<int>(i), cv::Scalar(cr.color[0], cr.color[1], cr.color[2], cr.color[3]), cv::FILLED);

                // Convert overlay and bigImg to RGBA color space
                cv::cvtColor(overlay, overlay, cv::COLOR_BGR2BGRA);
                cv::cvtColor(bigImg, bigImg, cv::COLOR_BGR2BGRA);

                // Blend the overlay image with the original image
                cv::addWeighted(overlay, 1.0, bigImg, 1.0, 0, bigImg);
            }

            // Get bounding rect of all contours
            cv::Rect br = cv::boundingRect(all_contours);

            // Draw bounding box
            if (!bounded) {
                cv::rectangle(bigImg, br, cv::Scalar(cr.color[0], cr.color[1], cr.color[2], 255), 2);
                bounded = true;
            }

           
            
        }
    }
}

void processCircle(cv::Mat& testImage, const cv::Point& center, int radius, const std::string& savedFeaturesFolderPath, const std::vector<ColorRange>& colorRanges, int minPixelCount, std::string* &foodsNames) {
    static int index = 0;
    // Create a mask for the current circle
    cv::Mat mask = cv::Mat::zeros(testImage.size(), CV_8UC1);
    cv::circle(mask, center, radius, cv::Scalar(255), -1);

   
    cv::Mat subImg;
    testImage.copyTo(subImg, mask);

    // Initialize SURF detector
    cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create();

    
    std::vector<cv::KeyPoint> testKeypoints;
    cv::Mat testDescriptors;
    surf->detectAndCompute(subImg, cv::noArray(), testKeypoints, testDescriptors);

    std::vector<ObjectMatch> matches;
    std::vector<ColorRange> finalMatched;
   
    for (const auto& entry : fs::directory_iterator(savedFeaturesFolderPath)) {

        std::string featuresPath = entry.path().string();

        // Load saved descriptors
        cv::Mat savedDescriptors;
        cv::FileStorage fileStorage(featuresPath, cv::FileStorage::READ);
        fileStorage["descriptors"] >> savedDescriptors;
        fileStorage.release();

        
        cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(5), new cv::flann::SearchParams(50));

        
        std::vector<std::vector<cv::DMatch>> knnMatches;
        matcher.knnMatch(testDescriptors, savedDescriptors, knnMatches, 2);

        
        const float ratioThresh = 0.7f;
        std::vector<cv::DMatch> goodMatches;
        for (size_t i = 0; i < knnMatches.size(); i++) {
            if (knnMatches[i][0].distance < ratioThresh * knnMatches[i][1].distance) {
                goodMatches.push_back(knnMatches[i][0]);
            }
        }

        // Check if more than 10% of the features matched
        float matchPercentage = static_cast<float>(goodMatches.size()) / static_cast<float>(savedDescriptors.rows) * 100.0f;
        if (matchPercentage > 10.0f) {
            std::cout << "Match found with: " << entry.path().stem().string() << ", Match Percentage: " << matchPercentage << "%" << std::endl;
            matches.push_back({ entry.path().stem().string(), matchPercentage });
        }
    }

    // Sort the matches by matching percentage
    std::sort(matches.begin(), matches.end(), [](const ObjectMatch& a, const ObjectMatch& b) {
        return a.percentage > b.percentage;
    });

    // Check the first match for pasta or rice
    std::vector<ColorRange> topMatches;
    if (!matches.empty() && (matches[0].name.find("Pasta") != std::string::npos || matches[0].name.find("Rice") != std::string::npos)) {
        std::cout << matches[0].name << std::endl;
        for (const auto& cr : colorRanges) {
            if (cr.name == matches[0].name) {
                topMatches.push_back(cr);
                foodsNames[index] = cr.name;
                index++;
                break;
            }
        }

    }
    else {
        for (int i = 0; i < std::min(2, static_cast<int>(matches.size())); ++i) {
            for (const auto& cr : colorRanges) {
                if (cr.name == matches[i].name) {
                    topMatches.push_back(cr);
                    foodsNames[index] = cr.name;
                    std::cout<<cr.name<<std::endl;
                    index++;
                    break;
                }
            }
        }
    }

    for (const auto& match : topMatches)
        std::cout << "Name: " << match.name << std::endl;

    if (!topMatches.empty()) {
        colorizeSpecificObjects(testImage, mask, topMatches, minPixelCount);
    }

    

}

int main() {
    std::string* foodsNames = nullptr;
    foodsNames = new std::string[10];
    
    int size = 10;
    std::string savedFeaturesFolderPath = "../keypoints_by_food_type";
    std::string testImagePath = "../Food_leftover_dataset/tray3/food_image.jpg";

    std::vector<ColorRange> colorRanges = {
        {"Pasta Pesto", cv::Scalar(14, 108, 0), cv::Scalar(48, 254, 255), cv::Vec4b(0, 0, 255, 128)},
        {"Basil Potatoes", cv::Scalar(18, 46, 0), cv::Scalar(29, 255, 255), cv::Vec4b(0, 255, 0, 128)},
        {"Basil Potatoes_1", cv::Scalar(18, 46, 0), cv::Scalar(29, 255, 255), cv::Vec4b(0, 255, 0, 128)},
        {"Fish Cutlet", cv::Scalar(5, 98, 160), cv::Scalar(15, 255, 255), cv::Vec4b(255, 0, 0, 128)},
        {"Fish Cutlet_1", cv::Scalar(5, 98, 160), cv::Scalar(15, 255, 255), cv::Vec4b(255, 0, 0, 128)},
        {"Pasta With Tomato Sauce", cv::Scalar(0, 113, 0), cv::Scalar(25, 255, 255), cv::Vec4b(0, 255, 255, 128)},
        {"Grilled Pork Cutlet", cv::Scalar(12, 72, 0), cv::Scalar(28, 255, 255), cv::Vec4b(255, 255, 0, 128)},
        {"Grilled Pork Cutlet_1", cv::Scalar(12, 72, 0), cv::Scalar(28, 255, 255), cv::Vec4b(255, 255, 0, 128)},
        {"Beans", cv::Scalar(0, 159, 0), cv::Scalar(8, 255, 255), cv::Vec4b(255, 0, 255, 128)},
        {"Beans_1", cv::Scalar(0, 159, 0), cv::Scalar(8, 255, 255), cv::Vec4b(255, 0, 255, 128)},
        {"Beans_2", cv::Scalar(0, 159, 0), cv::Scalar(8, 255, 255), cv::Vec4b(255, 0, 255, 128)},
        {"Salad", cv::Scalar(0, 121, 58), cv::Scalar(180, 255, 255), cv::Vec4b(128, 0, 255, 128)},
        {"Salad_1", cv::Scalar(0, 121, 58), cv::Scalar(180, 255, 255), cv::Vec4b(128, 0, 255, 128)},
        {"Salad_2", cv::Scalar(0, 121, 58), cv::Scalar(180, 255, 255), cv::Vec4b(128, 0, 255, 128)},
        {"Rabit", cv::Scalar(1, 72, 0), cv::Scalar(21, 255, 255), cv::Vec4b(255, 128, 0, 128)},
        {"bread", cv::Scalar(1, 72, 0), cv::Scalar(21, 255, 255), cv::Vec4b(255, 128, 0, 128)},
        {"Rabit_1", cv::Scalar(1, 72, 0), cv::Scalar(21, 255, 255), cv::Vec4b(255, 128, 0, 128)},
        {"Pilaw Rice With Peppers And Peas", cv::Scalar(16, 55, 0), cv::Scalar(46, 255, 255), cv::Vec4b(0, 128, 255, 128)},
        {"Pasta With Meat Sauce", cv::Scalar(0, 113, 0), cv::Scalar(23, 255, 255), cv::Vec4b(128, 128, 128, 128)},
        {"Pasta With Clams and Mussels", cv::Scalar(8, 114, 0), cv::Scalar(39, 255, 255), cv::Vec4b(128, 128, 0, 128)},
        {"Seafood Salad", cv::Scalar(5, 0, 68), cv::Scalar(14, 144, 255), cv::Vec4b(128, 0, 128, 128)},
    };

    // Load the test image
    cv::Mat testImage = cv::imread(testImagePath, cv::IMREAD_COLOR);

    // Convert the image to BGRA color space
    cv::cvtColor(testImage, testImage, cv::COLOR_BGR2BGRA);

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(testImage, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);
    cv::blur(gray, gray, cv::Size(3, 3));

    
    cv::Mat edges;
    cv::Canny(gray, edges, 300, 100);

    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
    cv::dilate(edges, edges, kernel);
    cv::erode(edges, edges, kernel);

    // Run Hough Circle Transforms
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(edges, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 2, 100, 50, 130, 800);

    int minPixelCount = 5000; // replace with your threshold

    
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
        processCircle(testImage, center, radius, savedFeaturesFolderPath, colorRanges, minPixelCount, foodsNames);
    }

     annotateImageWithFoods(foodsNames, colorRanges, testImage);

    cv::namedWindow("Hough Circle Transform", cv::WINDOW_NORMAL);
    cv::imshow("Hough Circle Transform", testImage);

    cv::waitKey(0);
    return 0;
}
// ####################################################################### the main project ########################################

// ####################################################################### Left_over Calculation ################################
// int main()
// {
//     // Load the images
//     cv::Mat image1 = cv::imread("../build_1/Food_leftover_dataset/tray2/plate_0_food_image.jpg", cv::IMREAD_GRAYSCALE);
//     cv::Mat image2 = cv::imread("../build_1/Food_leftover_dataset/tray2/plate_1_leftover3.jpg", cv::IMREAD_GRAYSCALE);

//     if (image1.empty() || image2.empty())
//     {
//         std::cout << "Failed to load images!" << std::endl;
//         return -1;
//     }

//     // Convert image2 to binary (black and white)
//     cv::Mat binaryImage1;
//     cv::Mat binaryImage2;
//     cv::threshold(image1, binaryImage1, 0, 255, cv::THRESH_BINARY);
//     cv::threshold(image2, binaryImage2, 0, 255, cv::THRESH_BINARY);

//     // Count the number of non-black pixels in image2
//     int nonBlackPixels_1 = cv::countNonZero(binaryImage1);
//     int nonBlackPixels_2 = cv::countNonZero(binaryImage2);

//     // Calculate the ratio
//     double ratio = static_cast<double>(nonBlackPixels_2) / static_cast<double>(nonBlackPixels_1);

//     // std::cout << "Number of non-black pixels: " << nonBlackPixels << std::endl;
//     std::cout << "Ratio: " << ratio << std::endl;

//     return 0;
// }

