% Read the image
originalImage = imread('Flower.jpg'); 

% Convert the image to double for processing
originalImage = double(originalImage);

% Create a Laplacian kernel
laplacianKernel = [0 -1 0; -1 4 -1; 0 -1 0];

% Apply Laplacian filter to the image And Add it to original image
edge = imfilter(originalImage, laplacianKernel);
sharpened = edge + originalImage;

% Clip values to the valid intensity range
sharpened = max(0, min(sharpened, 255));
edge = max(0, min(edge, 255));

% Convert back to uint8 for display
sharpened = uint8(sharpened);
edge = uint8(edge);

% Display original and sharpened images
figure;
subplot(1, 3, 1);
imshow(uint8(originalImage));
title('Original Image');

subplot(1, 3, 2);
imshow(edge);
title('Edge Image');

subplot(1, 3, 3);
imshow(sharpened);
title('Sharpened Image');
