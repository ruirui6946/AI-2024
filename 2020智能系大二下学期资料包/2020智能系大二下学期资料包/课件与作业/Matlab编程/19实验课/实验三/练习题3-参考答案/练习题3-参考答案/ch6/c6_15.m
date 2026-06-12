clc;clear;

surf(peaks)
shading interp
 lighting phong      %flat  gouraud   none
 light('Position',[-3 -2 1]);
 pause;
light('Position',[1 0 1]);
