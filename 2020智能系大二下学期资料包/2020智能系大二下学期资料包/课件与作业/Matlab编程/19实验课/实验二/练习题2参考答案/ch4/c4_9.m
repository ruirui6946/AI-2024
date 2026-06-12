clc;clear;close all

v = -2:0.2:2;
[x,y] = meshgrid(v);
z=10*(x.^3-y.^5).*exp(-x.^2-y.^2);
[px,py] = gradient(z,.2,.2);  %  [px,py,...]=gadient(f,h1,h2....)
contour(x,y,z,25)      %画出等高线  
pause;
hold on
quiver(x,y,px,py)  %  在点（x,y）处画向量（px，py）
hold off

% contour(Z)
% contour(Z,n)
% contour(Z,v)
% contour(X,Y,Z)
% contour(X,Y,Z,n)
% contour(X,Y,Z,v)

