clc;clear;close all

p=[4 -12 -14 5 9]
pder=polyder(p);
poly2sym(pder)

pint=polyint(p);
poly2sym(pint)