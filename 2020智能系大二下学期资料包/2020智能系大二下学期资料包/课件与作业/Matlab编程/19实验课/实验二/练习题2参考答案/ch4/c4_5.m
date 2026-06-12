clc;clear;close all;

p1=[3 13 6 8];
p2=[1 4];
p=deconv(p1,p2)
p=poly2sym(p)

%如果有余数

[p3 r]=deconv(p1,p2)