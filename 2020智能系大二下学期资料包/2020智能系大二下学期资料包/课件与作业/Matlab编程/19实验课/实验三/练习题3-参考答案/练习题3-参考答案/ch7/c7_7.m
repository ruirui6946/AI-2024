clc;clear;close all;

syms x s;
f=x^5+3*x^4+4*x^3+2*x^2+3*x+6;
F=subs(f,x,(s-1)/(s+1))