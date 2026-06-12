clc;clear;close all;

x=[0:0.1:1];
y=[-0.44 1.97 3.28 6.16 7.08 7.34 7.66 9.56 9.48 9.33 11.2 ];

f3=polyfit(x,y,3);
x3=[0:0.02:1];
y3=polyval(f3,x3);

f6=polyfit(x,y,11);
x6=[0:0.02:1];
y6=polyval(f6,x6);

plot(x,y,'o',x3,y3,'r',x6,y6,'b')

legend('xy','f3','f6');
zoom on