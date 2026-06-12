clc;clear;close all;

syms x f;

y=dsolve('D2f=sin(x)*(1-2/x^2)/x+2*cos(x)/x^2','f(0)=2','Df(0)=0')

ezplot(y,[-50,50])