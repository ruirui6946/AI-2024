clc;clear;


figure(1);
surf(peaks(30));
colormap(hot);

fg1 = getframe(gcf);

figure(2);
surf(peaks(30));
colormap(cool);

fg2 = getframe(gcf);

close all;
figure(3)
subplot(1,2,1)
imshow(fg1.cdata);

subplot(1,2,2)
imshow(fg2.cdata);

