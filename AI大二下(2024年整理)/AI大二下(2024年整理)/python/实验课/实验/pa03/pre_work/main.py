import pymysql
db = pymysql.connect(host="localhost", user="root", password="0000", db="crawl3", charset="utf8")
# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 执行 SQL 查询语句
sql = "SELECT basic_experience FROM result;"
cursor.execute(sql)

# 获取查询结果
results = cursor.fetchall()
with open('人物分析.txt','w+',encoding='utf-8') as fp:
    for row in results:
        row=str(row)
        row=row[2:-3]
        row=row+'\n'
        fp.write(row)
# 关闭数据库连接
db.close()