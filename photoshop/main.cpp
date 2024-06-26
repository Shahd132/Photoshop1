#include "Image_Class.h"
#include <iostream>
#include<string>
#include <fstream>
#include<algorithm>
#include<vector>
#include<cmath>
using namespace std;
void gray_scale(string filename) {//filter1
    Image image(filename);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k);
            }
            avg /= 3;
            // Set the pixel values to the average grayscale value
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = avg;
            }
        }
    }
    image.saveImage("CURRENT_VERSION.jpg");
}
//end
void filter2(string filename) {//filter2
    Image image(filename);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; ++k) {
                avg += image(i, j, k);
            }
            avg = avg / 3; // Calculate average RGB value

            // Convert to black and white
            unsigned int threshold = 128; // Threshold for converting to black or white
            unsigned int bw_value = (avg < threshold) ? 0
                                                      : 255; // If average is less than threshold, set to black, else white

            // Set all channels to the same black and white value
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = bw_value;
            }
        }
    }
    image.saveImage("CURRENT_VERSION.jpg");
}
//end filter2
void invert_colors(string filename)//filter3
{

    unsigned char *imageData;
    int width, height, channels;
    imageData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (imageData == nullptr) {
        cout << "Error loading image." << endl;
    }
    for (int i = 0; i < width * height * channels; i++)
    {
        imageData[i] = 255 - imageData[i];
    }
    stbi_write_jpg(filename.c_str(), width, height, channels, imageData, 100);
    stbi_image_free(imageData);
}
//end filter3
Image manualResizeImage(Image image, int new_width, int new_height) {//filter4
    int originalWidth = image.width;
    int originalHeight = image.height;

    Image resizedImage(new_width, new_height);

    // Calculate the scaling factors for width and height
    float widthRatio = static_cast<float>(originalWidth) / new_width;
    float heightRatio = static_cast<float>(originalHeight) / new_height;

    // Resize loop
    for (int i = 0; i < new_width; ++i) {
        for (int j = 0; j < new_height; ++j) {
            // Calculate the corresponding position in the original image
            int originalX = static_cast<int>(i * widthRatio);
            int originalY = static_cast<int>(j * heightRatio);

            // Get the pixel value from the original image and set it in the resized image
            for (int k = 0; k < 3; ++k) {
                unsigned char pixelValue = image.getPixel(originalX, originalY, k);
                resizedImage.setPixel(i, j, k, pixelValue);
            }
        }
    }

    return resizedImage;
}

// Function to merge two images by resizing to the largest dimensions
Image mergeResizeToMax( Image image1,  Image image2) {
    // Determine the maximum dimensions
    int max_width = std::max(image1.width, image2.width);
    int max_height = std::max(image1.height, image2.height);

    // Resize both images to the maximum dimensions
    Image resizedImage1 = manualResizeImage(image1, max_width, max_height);
    Image resizedImage2 = manualResizeImage(image2, max_width, max_height);

    Image mergedImage(max_width, max_height);
    for (int i = 0; i < max_width; ++i) {
        for (int j = 0; j < max_height; ++j) {
            // Blend pixel values using the alpha value
            for (int k = 0; k < 3; ++k) {
                unsigned char pixel1 = resizedImage1.getPixel(i, j, k);
                unsigned char pixel2 = resizedImage2.getPixel(i, j, k);
                unsigned char blendedPixel = static_cast<unsigned char>((0.5 * pixel1) + (0.5 * pixel2));
                mergedImage.setPixel(i, j, k, blendedPixel);
            }
        }
    }
    mergedImage.saveImage("CURRENT_VERSION.jpg");

    return mergedImage;
}

// Function to merge two images by overlapping the common area of smaller width and height
Image mergeOverlapCommonArea(Image image1,Image image2) {
    // Determine the common area dimensions
    int min_width = std::min(image1.width, image2.width);
    int min_height = std::min(image1.height, image2.height);
    Image mergedImage(min_width, min_height);
    for (int i = 0; i < min_width; ++i) {
        for (int j = 0; j < min_height; ++j) {
            // Blend pixel values using the alpha value
            for (int k = 0; k < 3; ++k) {
                unsigned char pixel1 = image1.getPixel(i, j, k);
                unsigned char pixel2 = image2.getPixel(i, j, k);
                unsigned char blendedPixel = static_cast<unsigned char>((0.5* pixel1) + (0.5 * pixel2));
                mergedImage.setPixel(i, j, k, blendedPixel);
            }
        }
    }
    mergedImage.saveImage("CURRENT_VERSION.jpg");

    return mergedImage;
}

void merge(string filename) {
    // Load two images
    Image image1(filename);
    string filename2,original;
    bool validFilename = false;
    while (!validFilename) {
        cout << "Enter the name of the image file (with extension): ";
        cin >> filename2;
        ifstream file(filename2);
        if (!file.is_open()) {
            cout << "File '" << filename2 << "' not found. Please enter a valid filename with valid extention\n." << endl;
        } else {
            validFilename = true;
        }
    }
    Image image2(filename2);

    // Prompt the user for their choice
    cout << "Choose merging option:\n";
    cout << "1-Maximuim image.\n";
    cout << "2-Merge the common area of the smaller width and height.\n";
    int option;
    cin >> option;

    Image mergedImage;
    if (option == 1) {
        mergedImage = mergeResizeToMax(image1, image2);
    } else if (option == 2) {
        mergedImage = mergeOverlapCommonArea(image1, image2);
    } else {
        cout << " Exit";

    }
}
//end filter4
void filter5(string filename) {//filter5
    // Load the original image
    Image img(filename);

    // Create a new image with the same dimensions as the original
    Image flippedImg(img.width, img.height); // Swap width and height

    cout << "1-Horizontal Flip , 2-Vertical Flip" << endl ;
    int choice ;
    cin >> choice ;
    if(choice==1){
        Image Horizontal_Flip(img.width, img.height);

        for (int i = 0; i < img.height; ++i) {
            for (int j = 0; j < img.width; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    // Swap pixel values horizontally
                    flippedImg(img.width - j - 1, i, k) = img(j, i, k); // Corrected indices
                }
            }
        }

        // Save the horizontally flipped image
        flippedImg.saveImage("CURRENT_VERSION.jpg");

    }
    else if(choice==2){
        Image Vertical_Flip(img.width, img.height);
        // Vertical Flip: Swap pixels from top to bottom
        for (int i = 0; i < img.height; ++i) {
            for (int j = 0; j < img.width; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    // Swap pixel values vertically
                    flippedImg(j, img.height - i - 1, k) = img(j, i, k); // Corrected indices
                }
            }
        }

        // Save the vertically flipped image
        flippedImg.saveImage("CURRENT_VERSION.jpg");

    }
}
//end filter5
enum Rotation { ROTATE_90, ROTATE_180, ROTATE_270 };//filter6
void rotateImage(Image &img, Rotation rotation) {
    if (rotation != ROTATE_90 && rotation != ROTATE_180 && rotation != ROTATE_270) {
        cout << "Invalid angle. Only 90, 180, or 270 degrees rotation is supported." << endl;
        return;
    }

    int width = img.width;
    int height = img.height;
    int channels = img.channels;

    // Calculate new dimensions based on rotation
    int newWidth, newHeight;
    if (rotation == ROTATE_90 || rotation == ROTATE_270) {
        newWidth = height;
        newHeight = width;
    } else {
        newWidth = width;
        newHeight = height;
    }

    // Create temporary buffer for rotated image
    unsigned char* rotatedImageData = new unsigned char[newWidth * newHeight * channels];

    // Perform rotation
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int newX, newY;
            if (rotation == ROTATE_90) {
                newX = newHeight - 1 - y;
                newY = x;
            } else if (rotation == ROTATE_180) {
                newX = newWidth - 1 - x;
                newY = newHeight - 1 - y;
            } else { // angle == 270
                newX = y;
                newY = newWidth - 1 - x;
            }

            for (int c = 0; c < channels; ++c) {
                rotatedImageData[(y * newWidth + x) * channels + c] = img.getPixel(newX, newY, c);
            }
        }
    }

    // Free old image data and update dimensions
    delete[] img.imageData;
    img.imageData = rotatedImageData;
    img.width = newWidth;
    img.height = newHeight;
}

void Rotation1(string filename)
{
    Image img(filename);

    // Ask user for rotation choice
    int rotationChoice;
    cout << "Choose rotation:\n1. 90 degrees\n2. 180 degrees\n3. 270 degrees\n";
    cin >> rotationChoice;
    Rotation rotation;
    while(true)
    {
        if (rotationChoice == 1) {
            rotation = ROTATE_90;
            break;
        }
        else if (rotationChoice == 2) {
            rotation = ROTATE_180;
            break;
        }
        else if (rotationChoice == 3) {
            rotation = ROTATE_270;
            break;
        }
        else {
            cout << "Invalid choice. Please choose 1, 2, or 3." << endl;
            continue;
        }
    }


    // Rotate image
    rotateImage(img, rotation);

    // Save rotated image// Change this to the desired output filename

    img.saveImage("CURRENT_VERSION.jpg");
}
//end filter6
void filter7(string filename) {//filter7
    // Load the image
    Image image(filename);

    // Prompt the user to choose darkening or lightening
    cout << "Do you want to lighten or darken the image?\n"<< "1-lighten\n" <<"2-darken"<<endl;
    int choice;
    cin >> choice;

    // Factor for darkness or brightness adjustment
    float factor;
    if (choice == 1) {
        cout << "Enter the percentage of lightening (0 to 100): ";
        cin >> factor;
        factor = 1.0 + (factor / 100.0); // Convert percentage to factor
    } else if (choice == 2) {
        cout << "Enter the percentage of darkening (0 to 100): ";
        cin >> factor;
        factor = 1.0 - (factor / 100.0); // Convert percentage to factor
    } else {
        cout << "Invalid choice. Please enter 'lighten' or 'darken'." <<endl;
        return;
    }

    // Adjust darkness or brightness of the image
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                // Apply darkness or brightness adjustment
                int newValue = static_cast<int>(image(i, j, k) * factor);

                // Ensure the value stays within [0, 255] range
                newValue = std::min(std::max(newValue, 0), 255);

                // Update pixel value
                image(i, j, k) = newValue;
            }
        }
    }

    // Save the modified image
    image.saveImage("CURRENT_VERSION.jpg");
}
//end filter7
void filter8(string filename) {//filter8
    // Load the original image
    Image img(filename);

    // Prompt user for cropping parameters
    int x, y, crop_width, crop_height;
    cout << "Enter the starting point (x, y) of the area to crop: ";
    cin >> x >> y;
    cout << "Enter the dimensions (width x height) of the area to crop: ";
    cin >> crop_width >> crop_height;

    // Create a new image for the cropped result
    Image croppedImg(crop_height, crop_width);

    // Iterate over the cropped region and copy pixels from the original image
    for (int i = 0; i < crop_width; ++i) {
        for (int j = 0; j < crop_height; ++j) {
            for (int k = 0; k < img.channels; ++k) {
                croppedImg(j, i, k) = img(y + j, x + i, k); // Copy pixels from original image to cropped image
            }
        }
    }

    croppedImg.saveImage("CURRENT_VERSION.jpg");
}
//end filter8
enum Frame {Fancy,Simple};//filter9
int colors[6][3] = {
        {255, 0, 0},   // Red
        {255, 255, 0}, // Yellow
        {0, 255, 0},   // Green
        {0, 255, 255}, // Cyan
        {0, 0, 255},   // Blue
        {255, 0, 255}  // Magenta
};

void addFrame(Image &img, int frameSize, int red, int green, int blue,Frame frame) {
    int width = img.width;
    int height = img.height;
    int channels = img.channels;

    // Iterate over the pixels in the border regions to add the frame
    switch (frame)
    {
        case Simple:
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    // Check if the pixel is in the border region
                    if (x < frameSize || x >= width - frameSize || y < frameSize || y >= height - frameSize) {
                        // Set pixel color to frame color
                        img(x, y, 0) = red;
                        img(x, y, 1) = green;
                        img(x, y, 2) = blue;
                    }
                }
            }
        case Fancy:
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    // Check if the pixel is in the border region
                    if (x < frameSize || x >= width - frameSize || y < frameSize || y >= height - frameSize) {
                        int colorIndex = (x + y) % 6;
                        img(x, y, 0) = colors[colorIndex][0]; // Red component
                        img(x, y, 1) = colors[colorIndex][1]; // Green component
                        img(x, y, 2) = colors[colorIndex][2];
                    }
                }
            }
    }
}

void frame (string filename) {
    Image img(filename);

    // Ask user for frame size and color
    int frameSize;
    int red, green, blue;
    cout << "Enter the frame size: ";
    cin >> frameSize;
    cout << "Enter the RGB values for the frame color (separated by spaces): ";
    cin >> red >> green >> blue;

    // Add a simple frame to the image
    while(true)
    {
        cout<<"1_Simple frame\n"<<"2_Fancy frame\n";
        string framechoice;
        cin>>framechoice;
        Frame frame;
        if(framechoice=="1")
        {
            frame=Simple;
            addFrame(img,frameSize,red,green,blue,frame);
            img.saveImage("CURRENT_VERSION.jpg");
            break;
        }
        else if(framechoice=="2")
        {
            frame=Fancy;
            addFrame(img,frameSize,red,green,blue,frame);
            img.saveImage("CURRENT_VERSION.jpg" );
            break;
        }
        else
        {
            cout<<"Invalid choice ,try again.\n";
            continue;
        }
    }
}
//end filter9
Image applySobel(Image image, int threshold = 128) {//filter10
    Image result(image.width, image.height);

    // Sobel operator kernels
    int sobelXKernel[3][3] = {{-1, 0, 1},
                              {-2, 0, 2},
                              {-1, 0, 1}};
    int sobelYKernel[3][3] = {{-1, -2, -1},
                              {0, 0, 0},
                              {1, 2, 1}};

    for (int i = 1; i < image.width - 1; ++i) {
        for (int j = 1; j < image.height - 1; ++j) {
            int gradientX = 0;
            int gradientY = 0;

            // Convolve image with Sobel kernels
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    gradientX += sobelXKernel[x + 1][y + 1] * image.getPixel(i + x, j + y, 0);
                    gradientY += sobelYKernel[x + 1][y + 1] * image.getPixel(i + x, j + y, 0);
                }
            }

            // Calculate gradient magnitude
            int gradientMagnitude = sqrt(gradientX * gradientX + gradientY * gradientY);


            if (gradientMagnitude >= threshold) {

                for (int k = 0; k < 3; ++k) {
                    result.setPixel(i, j, k, 255);
                }
            } else {

                for (int k = 0; k < 3; ++k) {
                    result.setPixel(i, j, k, 0);
                }
            }
        }
    }

    return result;
}

Image convertToBlackAndWhite(Image& image, unsigned int threshold = 128) {
    Image sobelImage = applySobel(image, threshold);
    Image result(image.width, image.height);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int grey = sobelImage.getPixel(i, j, 0);

            if (grey < threshold) {
                for (int k = 0; k < 3; ++k) {
                    result.setPixel(i, j, k, 255);
                }
            } else {
                for (int k = 0; k < 3; ++k) {
                    result.setPixel(i, j, k, 0);
                }
            }
        }
    }

    return result;
}

void filter10(string filename) {
    Image image(filename);
    Image ISobelImage = convertToBlackAndWhite(image);
    ISobelImage.saveImage("CURRENT_VERSION.jpg");
}
//end filter10
void filter11(string filename) {//filter11
    Image img(filename);

    int newWidth, newHeight;
    cout << "Enter the new width: ";
    cin >> newWidth;
    cout << "Enter the new height: ";
    cin >> newHeight;

    Image resizedImg(newWidth, newHeight);

    // Calculate the scaling factors for width and height
    float widthScale = static_cast<float>(img.width) / newWidth;
    float heightScale = static_cast<float>(img.height) / newHeight;

    // Loop through each pixel of the resized image
    for (int i = 0; i < newWidth; ++i) {
        for (int j = 0; j < newHeight; ++j) {
            // Calculate the corresponding pixel position in the original image
            int original_i = static_cast<int>(i * widthScale);
            int original_j = static_cast<int>(j * heightScale);

            // Copy pixel values from the original image to the resized image
            for (int k = 0; k < img.channels; ++k) {
                resizedImg(i, j, k) = img(original_i, original_j, k);
            }
        }
    }

    resizedImg.saveImage("CURRENT_VERSION.jpg");

}
//end filter11
// Function to apply a blur filter to the image
void applyBlurFilter(vector<unsigned char>& imageData, int width, int height, int channels, int blurLevel) {//filter12
    // Define the blur kernel based on the blur level
    int kernelSize = 2 * blurLevel + 1;
    float kernelSum = pow(kernelSize, 2);
    vector<vector<float>> blurKernel(kernelSize, vector<float>(kernelSize, 1.0 / kernelSum));

    // Create a temporary vector to store the blurred result
    vector<unsigned char> blurredImage(imageData.size());

    // Iterate over each pixel in the image (excluding border pixels)
    for (int y = blurLevel; y < height - blurLevel; ++y) {
        for (int x = blurLevel; x < width - blurLevel; ++x) {
            // Apply the blur kernel to the pixel and its neighbors
            for (int c = 0; c < channels; ++c) {
                float blurredValue = 0.0;
                for (int ky = -blurLevel; ky <= blurLevel; ++ky) {
                    for (int kx = -blurLevel; kx <= blurLevel; ++kx) {
                        int pixelX = x + kx;
                        int pixelY = y + ky;
                        int kernelX = kx + blurLevel; // Adjust kernel index to match blurKernel
                        int kernelY = ky + blurLevel;
                        int pixelIndex = (pixelY * width + pixelX) * channels + c;
                        blurredValue += imageData[pixelIndex] * blurKernel[kernelY][kernelX];
                    }
                }
                // Assign the blurred value to the corresponding pixel in the temporary vector
                int index = (y * width + x) * channels + c;
                blurredImage[index] = static_cast<unsigned char>(blurredValue);
            }
        }
    }

    // Copy the blurred image back to the original image data
    imageData = blurredImage;
}

void filter12(string filename) {
    // Load an image from file
    Image image(filename);
    unsigned char *imageData;
    int width, height, channels;
    imageData = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // Convert imageData to a vector
    vector<unsigned char> imageDataVec(imageData, imageData + width * height * channels);

    // Ask for blur level
    int blurLevel;
    cout << "Enter blur level (0 for no blur): ";
    cin >> blurLevel;

    // Apply blur filter
    applyBlurFilter(imageDataVec, width, height, channels, blurLevel);

    // Save the blurred image
    stbi_write_jpg(filename.c_str(), width, height, channels, imageDataVec.data(), 100);

    cout << "Blur filter applied and saved successfully." << endl;

    // Free memory allocated by stbi_load
    stbi_image_free(imageData);


}
//end filter12
void filter16(string filename)//bonus
{
    Image inputImage(filename);
    for (int y = 0; y < inputImage.height; y++) {
        for (int x = 0; x < inputImage.width; x++) {
            // Get original red and blue pixel values
            unsigned char &red = inputImage.getPixel(x, y, 0);
            unsigned char &blue = inputImage.getPixel(x, y, 2);

            // Increase red and blue channels to make purple
            red = min(255, red + 50); // Increase red, clamp to 255
            blue = min(255, blue + 50); // Increase blue, clamp to 255
        }
    }

    // Save the modified image
    inputImage.saveImage("CURRENT_VERSION.jpg");
}
void filter17(string filename) {//bonus
    Image image(filename);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 0) = 255;
            int temp = 255 - image(i, j, 2); // Store the calculated value in temp
            image(i, j, 2) = 255 - image(i, j, 1); // Use the calculated value from temp
            image(i, j, 1) = temp; // Assign the stored value from temp to the green channel
        }
    }
    image.saveImage("CURRENT_VERSION.jpg");
}

void filter13(string filename)//bonus
{
    // Create an Image object named "image" with the file "wano.jpg"
    Image image(filename);

    // Loop through each pixel in the image
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            // Get the original red, green, and blue pixel values at position (j, i)
            unsigned char &red = image.getPixel(j, i, 0);
            unsigned char &green = image.getPixel(j, i, 1);
            unsigned char &blue = image.getPixel(j, i, 2);

            // Adjust the pixel values to simulate natural sunlight effect
            red = min(255, red + 35);    // Increase red intensity
            green = min(255, green + 30);// Increase green intensity
            blue = min(255, blue - 33);  // Decrease blue intensity
        }
    }

    // Save the modified image with the filename "nutural_sunlight.jpg"
    image.saveImage("CURRENT_VERSION.jpg");
}
void loadNewImage(string& currentImage) {
    string filename;
    bool validFilename = false;

    while (!validFilename) {
        cout << "Enter the name of the image file (with extension): ";
        cin >> filename;
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "File '" << filename << "' not found. Please enter a valid filename with valid extension.\n";
        } else {
            // Check if the file extension is valid
            string ext = filename.substr(filename.find_last_of(".") + 1);
            if (ext != "jpg" && ext != "bmp" && ext != "png") {
                cout << "Invalid file extension. Please choose a file with '.jpg', '.bmp', or '.png' extension.\n";
            } else {
                validFilename = true;
                currentImage = filename;
            }
        }
    }
    cout << "New image '" << currentImage << "' loaded.\n";
}

void saveImage(string& currentImage) {
    Image image(currentImage);
    if (currentImage.empty()) {
        cout << "No image loaded. Please load an image first.\n";
        return;
    }
    string filename,original;
    bool validFilename = false;
    while (!validFilename) {
        cout << "Enter the name of the image file (with extension): ";
        cin >> filename;
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "File '" << filename << "' not found. Please enter a valid filename with valid extention\n." << endl;
        }
        else {
            // Check if the file extension is valid
            string ext = filename.substr(filename.find_last_of(".") + 1);
            if (ext != "jpg" && ext != "bmp" && ext != "png") {
                cout << "Invalid file extension. Please choose a file with '.jpg', '.bmp', or '.png' extension.\n";
            } else {
                validFilename = true;
            }
        }
    }
    image.saveImage(filename);

    // Perform the save operation
    // You need to implement the save functionality here
    cout << "Image saved as '" << filename << "'.\n";
}

int main()
{
    string filename,original;
    bool validFilename = false;
    while (!validFilename) {
        cout << "Enter the name of the image file (with extension): ";
        cin >> filename;
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "File '" << filename << "' not found. Please enter a valid filename with valid extention\n." << endl;
        }
        else {
            // Check if the file extension is valid
            string ext = filename.substr(filename.find_last_of(".") + 1);
            if (ext != "jpg" && ext != "bmp" && ext != "png") {
                cout << "Invalid file extension. Please choose a file with '.jpg', '.bmp', or '.png' extension.\n";
            } else {
                validFilename = true;
            }
        }
    }

    Image Temp_image(filename);
    Temp_image.saveImage("CURRENT_VERSION.jpg");
    original = filename;
    filename = "CURRENT_VERSION.jpg";

    while(true)
    {
        cout << "1_Load a new image\n"<<"2_Merge images\n"<<"3_Black and white\n"<<"4_Invert_color\n"<<"5_Gray_scale\n"<<"6_ dark and lighten image\n"<<"7_Rotation\n"<<"8_Blur_image\n"<<"9_Add_frame\n"<<"10_Purple_image\n"<<"11_Crop_image\n"<<"12_Resize_image\n"<<"13_Flip_image\n"<<"14_Edge_detection\n"<<"15_Infrared_image\n"<<"16_Sunlight_image\n"<<"17_Save_image\n"<<"18_Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin>>choice;
        if(choice==1)
        {
            loadNewImage(filename);

            Image Temp_image(filename);
            Temp_image.saveImage("CURRENT_VERSION.jpg");
            filename = "CURRENT_VERSION.jpg";
            original = filename;
        }
        if(choice==2)
        {
            merge(filename);
        }
        else if(choice==3)
        {
            filter2(filename);
        }
        else if(choice==4)
        {
            invert_colors(filename);
        }
        else if(choice==5)
        {
            gray_scale(filename);
        }
        else if(choice==6)
        {
            filter7(filename);
        }
        else if(choice==7)
        {
            Rotation1(filename);
        }
        else if(choice==8)
        {
            filter12(filename);
        }
        else if(choice==9)
        {
            frame(filename);
        }
        else if(choice==10)
        {
            filter16(filename);
        }
        else if(choice==11)
        {
            filter8(filename);
        }
        else if(choice==12)
        {
            filter11(filename);
        }
        else if(choice==13)
        {
            filter5(filename);
        }
        else if(choice==14)
        {
            filter10(filename);
        }

        else if(choice==15)
        {
            filter17(filename);
        }
        else if(choice==16)
        {
            filter13(filename);
        }
        else if(choice==17)
        {
            saveImage(filename);
        }
        else if(choice==18)
        {
            cout<<"Thanks for using our programme.";
            break;
        }
        else
        {
            cout<<"Invalid choice try again.\n";
        }
    }
    return 0;
}















