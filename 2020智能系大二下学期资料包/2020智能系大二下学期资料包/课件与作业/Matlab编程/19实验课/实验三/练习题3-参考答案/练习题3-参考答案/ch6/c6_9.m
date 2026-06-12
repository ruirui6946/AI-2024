clc;clear;

[x,y,z]=sphere(30);
 mesh(x,y,z) 
 pause;
 
mesh(x,y,z),hidden off
pause;

surf(x,y,z)
pause;

z(18:30,1:5)=NaN*ones(13,5);
surf(x,y,z)
