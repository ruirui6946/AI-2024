clc;clear;

[x,y]=meshgrid([-2:.2:2]);
 z=x.*exp(-x.^2-y.^2); 


subplot(2,2,1),   plot3(x,y,z)
title('plot3 (x,y,z)')

 subplot(2,2,2),   mesh(x,y,z)
 title('mesh (x,y,z)') 

subplot(2,2,3),   surf(x,y,z)
 title('surf (x,y,z)') 
 
 subplot(2,2,4),   surf(x,y,z), shading interp  %
 title('surf (x,y,z), shading interp')
