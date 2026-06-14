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

use student
go

/*(4) 备份student数据库。*/
-- 备份数据库命令
declare @name varchar(250) 
set @name='D:\my_world\class\大三\大三上\主修课\数据挖掘与数据仓库\数据库备份\student'+ convert(varchar(50),getdate(),112)+'.bak'
BACKUP DATABASE student TO  DISK = @name
WITH NOFORMAT, NOINIT,  NAME = N'ConsumeDB-完整 数据库 备份', SKIP, NOREWIND, NOUNLOAD


/*(5) 删除表选修表。*/
drop table SC

/*(6) 恢复student数据库。*/
use master
restore database student
from disk=@name
with replace


/*(7) 删除student数据库。*/
drop database student

/*(8) 根据<数据表.doc>建立<学籍管理>数据库*/
use master
restore database 学籍管理
from disk=@name
with replace