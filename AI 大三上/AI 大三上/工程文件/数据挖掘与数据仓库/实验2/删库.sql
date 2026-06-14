/*drop database student*/


/*(6) 恢复student数据库。*/
declare @name varchar(250) 
set @name='D:\my_world\class\大三\大三上\主修课\数据挖掘与数据仓库\数据库备份\student'+ convert(varchar(50),getdate(),112)+'.bak'

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