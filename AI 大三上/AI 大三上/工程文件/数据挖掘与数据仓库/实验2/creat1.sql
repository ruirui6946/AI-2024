/*(1) 建立student数据库。*/
create database [student]
go

use student
go

/*(3) 在数据库中建立学籍表、课程表和选修表（设立主键）*/

/*创建第一个表*/
create table Course
(
    Cno int not null primary key,
    Cname char(20) not null,
    Cpno int,
    Ccredit int not null,
);

insert into Course(Cno,Cname,Cpno,Ccredit)
    values(1,'数据库',5,4)
insert into Course(Cno,Cname,Ccredit)
    values(2,'数学',2)
insert into Course(Cno,Cname,Cpno,Ccredit)
    values(3,'信息系统',1,4)
insert into Course(Cno,Cname,Cpno,Ccredit)
    values(4,'操作系统',6,3)
insert into Course(Cno,Cname,Cpno,Ccredit)
    values(5,'数据结构',7,4)
insert into Course(Cno,Cname,Ccredit)
    values(6,'数据处理',2)
insert into Course(Cno,Cname,Cpno,Ccredit)
    values(7,'PASCAL语言',6,4)

create table Student
(
    Sno char(10) not null primary key,
    Sname char(20),
    Ssex char(10),
    Sage int,
    Sdept nchar(2)
);

insert into Student(Sno,Sname,Ssex,Sage,Sdept)
    values('95001','李勇','男',20,'CS')
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
    values('95002','刘晨','女',19,'IS')
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
    values('95003','王敏','女',18,'MA')
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
    values('95004','张立','男',19,'IS')

create table SC
(
    Sno char(10),
	Cno int,
	Grade int,
	primary key (Sno,Cno),/*组合主码*/
	foreign key (Sno) references Student(Sno),
	foreign key (Cno) references Course(Cno)
);

insert into Sc(Sno,Cno,Grade)
    values('95001',1,92)
insert into Sc(Sno,Cno,Grade)
    values('95001',2,85)
insert into Sc(Sno,Cno,Grade)
    values('95001',3,88)
insert into Sc(Sno,Cno,Grade)
    values('95002',2,90)
insert into Sc(Sno,Cno,Grade)
    values('95002',3,80)