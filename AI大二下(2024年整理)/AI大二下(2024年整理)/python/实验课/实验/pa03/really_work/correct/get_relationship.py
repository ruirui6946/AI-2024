import pymysql
db = pymysql.connect(host="localhost", user="root", password="0000", db="crawl3", charset="utf8")
# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 执行 SQL 查询语句
sql = "SELECT people_name,basic_relationship FROM result;"
cursor.execute(sql)

# 获取查询结果
results = cursor.fetchall()
# relationship = {}
# for row in results:
#     id=0
#     for i in range(len(row)):
#         if row[i] == 'r':
#             id = i
#             break 
#     row = row[5:i]
    

with open('correct_relationship.txt','w+',encoding='utf-8') as fp:
    for row in results:
        name = str(row[0])
        rela = str(row[1])
        relas = rela.split('\n')
        for rela in relas:
            id=0
            for i in range(len(rela)):
                if rela[i] == 'r':
                    id = i
                    break 
            rela = rela[5:id]
            fp.write(name+' '+rela+'\n')
# 关闭数据库连接
db.close()