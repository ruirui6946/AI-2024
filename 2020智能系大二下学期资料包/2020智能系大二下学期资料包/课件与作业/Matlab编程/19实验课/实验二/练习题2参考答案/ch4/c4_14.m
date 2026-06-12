clc;clear;close all;

dy=@(x,y)-3*y+2*x;
[x,y]=ode23(dy,[1,3],2);
plot(x,y)