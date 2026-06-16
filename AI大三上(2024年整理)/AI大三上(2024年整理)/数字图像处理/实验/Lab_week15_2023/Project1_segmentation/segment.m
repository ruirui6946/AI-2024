% Load the image
originalImage = imread('Block.png');

% Convert the image to grayscale if it's in RGB format
grayImage = im2gray(originalImage);

% Apply median filtering to reduce noise
filteredImage = medfilt2(grayImage);

% Enhance contrast
enhancedImage = imadjust(filteredImage);

% Use edge detection to find object boundaries
edgeImage = edge(enhancedImage, 'sobel');

% Dilate the edges to connect broken boundaries
dilatedEdges = imdilate(edgeImage, strel('square', 3));

% Fill the enclosed regions in the dilated image
filledImage = imfill(dilatedEdges, 'holes');

% Remove small objects (noise) from the filled image
cleanedImage = bwareaopen(filledImage, 100);

% Display the results
figure;
subplot(1, 3, 1)
imshow(originalImage)
title('Original Image');

subplot(1, 3, 2)
imshow(edgeImage)
title('Edge Detection');

subplot(1, 3, 3)
imshow(cleanedImage)
title('Segmented Objects');