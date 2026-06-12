clear
clc
stud.name='Jim';
stud.age=19;
stud.score=randi([1,99],3,10);

stud(2).name='Henry';
stud(2).age=18;
stud(2).score=randi([1,99],3,10);

stud(3).name='Smith';
stud(3).age=20;
stud(3).score=randi([1,99],3,10);

studScore2=stud(2).score;
disp(studScore2);

studAvgScore2=mean(stud(2).score,2);
disp(studAvgScore2);

studAvgScore=ones(1,10);
for k=1:3
    studAvgScore=studAvgScore+mean(stud(k).score);
end
studAvgScore=studAvgScore./3;

studName=cell(1,3);
for k=1:3
    studName(k)={stud(k).name};
end
