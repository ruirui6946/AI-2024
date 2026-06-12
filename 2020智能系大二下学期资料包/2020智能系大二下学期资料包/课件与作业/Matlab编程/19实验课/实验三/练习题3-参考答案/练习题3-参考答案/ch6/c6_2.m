clc;clear;

t=0:0.5:10;
y1=exp(-0.1*t);
 y2=exp(-0.2*t);
 y3=exp(-0.5*t);
 
 
 plot(t,y1,'-ob',t,y2,':*r',t,y3,'-.^g')
 
 %pause;
 
 title('y=e^{-at}','FontSize',12)
text(t(6),y1(6),'\leftarrowa=0.1','FontSize',11)
text(t(6),y2(6),'\leftarrowa=0.2','FontSize',11)
text(t(6),y3(6),'\leftarrowa=0.5','FontSize',11)

%pause;
legend('a=0.1','a=0.2','a=0.5')