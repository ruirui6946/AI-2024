clc;clear;

[c,hc]=contour(peaks(30));
child=get(hc,'children')
set(child(4),'linewidth',2)
set(child(6),'linestyle',':')
set(child(10),'marker','*')
