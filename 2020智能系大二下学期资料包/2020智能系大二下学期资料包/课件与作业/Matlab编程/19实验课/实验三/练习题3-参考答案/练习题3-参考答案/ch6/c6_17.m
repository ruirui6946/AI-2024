clc;clear;
subplot('position',[0.1,0.15,0.3,0.65])   %     left,bottom,width,height
hist(randn(1,1000),20);                    %画直方图
xlabel('直方图')

subplot('position',[0.45,0.52,0.25,0.28])
[xp,yp,zp]=peaks;
contour(xp,yp,zp,15,'k')                 %画15条等高线，黑色
hold on
pcolor(xp,yp,zp)                         %
shading interp
hold off
axis off
text(-1.2,-4,'伪彩色图')

subplot('position',[0.72,0.5,0.25,0.3])
sphere(25);                                % 画球
axis equal,axis([-0.75,0.75,-0.75,0.75,-0.75,0.75])
light('Position',[1 3 2]);
light('Position',[-3 -1 3]);
axis off
text(-0.8,-0.7,-1,'三维图')

subplot('position',[0.45,0.15,0.5,0.25])
t=0:pi/15:pi;
y=sin(4*t).*sin(t)/2;
plot(t,y,'-bs','LineWidth',2,...          %...同一表达式换行
        'MarkerSize',5)
axis([0,3.14,-0.5,0.5])
xlabel('带标记点的线图')

subplot('position',[0.1,0.9,0.8,0.1])
text(0.25,0.2,'多窗口绘图示例')
axis off;
