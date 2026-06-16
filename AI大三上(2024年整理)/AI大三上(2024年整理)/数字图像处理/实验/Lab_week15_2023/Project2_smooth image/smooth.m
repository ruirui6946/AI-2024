% Read the image
% originalImage = imread('ManWithNoise.jpg'); 
originalImage = imread('TruckWithNoise.bmp'); 
% Convert the image to double for processing
originalImage = double(originalImage);

% Set the size of the filter window (e.g., 3x3, 5x5)
filterSize = 4;

% Apply mean filter
meanFiltered = imfilter(originalImage, fspecial('average', [filterSize filterSize]));

% Display original and filtered images
figure;
imshow(uint8(meanFiltered));
title('Mean Filtered Image');