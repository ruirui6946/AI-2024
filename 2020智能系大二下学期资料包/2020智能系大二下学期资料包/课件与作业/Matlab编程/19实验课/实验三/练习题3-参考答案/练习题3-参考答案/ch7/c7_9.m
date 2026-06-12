clc;clear;close all;

syms x y;
f=x*exp(-x*y);
int(int(f,x),y)

% int(fun,v)
% int(fun,v,a,b)