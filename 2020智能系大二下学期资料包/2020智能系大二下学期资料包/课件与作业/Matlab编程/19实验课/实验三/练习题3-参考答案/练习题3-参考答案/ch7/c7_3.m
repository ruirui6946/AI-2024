clc;clear;close all;

syms t;

ezplot(sin(3*t)*cos(t),sin(3*t)*sin(t),[0,2*pi])

%{
ezplot(fun)
ezplot(fun,[xmin,xmax])

ezplot(fun2)
ezplot(fun2,[xymin,xymax])
ezplot(fun2,[xmiymin,yman,xmax,x])

ezplot(funx,funy)
ezplot(funx,funy,[tmin,tmax])
%}