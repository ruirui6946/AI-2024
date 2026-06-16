% Read the image
originalImage = imread('Cashew.jpg'); 

% Convert the image to grayscale if it's not already
grayImage = rgb2gray(originalImage);

% Perform histogram equalization
enhancedImage = histeq(grayImage);

% Display original and enhanced images
figure;
imshow(enhancedImage);
title('Contrast Enhanced Image');
