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