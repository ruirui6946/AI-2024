clc;clear;close all;

x0=0:pi/5:4*pi;  
y0=sin(x0).*exp(-x0/10);
x=0:pi/20:4*pi;
y=interp1(x0,y0,x,'spline'); %%　　y=spline(x0,y0,x); 
plot(x0,y0,'or',x,y,'b')

%{
插值方法：spline cubic linear
二维插值：z1=interp2(x,y,z,x1,y1,'method')
%}