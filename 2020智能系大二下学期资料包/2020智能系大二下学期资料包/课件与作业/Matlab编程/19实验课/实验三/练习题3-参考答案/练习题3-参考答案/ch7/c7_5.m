clc;clear;close all;

syms y t;
dsolve('D3y+3*D2y+3*Dy+y=exp(-t)*sin(t)')