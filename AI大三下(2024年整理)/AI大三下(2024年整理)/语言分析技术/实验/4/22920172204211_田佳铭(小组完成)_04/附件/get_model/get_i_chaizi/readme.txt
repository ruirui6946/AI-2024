chaizi-jt.txt是拆字词典
get_i_chazi.py是制作常用汉字拆字倒映射的程序
i_chazi.txt是get_i_chazi.py的输出结果（暂时不放在这里，节省空间）
i_chazi_tool.py中定义了两个函数
（1）i_chaizi_get(filename)：根据filename读入倒拆字表
（2）iquery(x,y)：查询x的倒映射结果，y是倒映射表，例如iquery("夕口",i_chaizi)，结果是“名”

当然，要在句子中识别出拆字类型，需要找到连续的一些拆字结果，逐一倒映射并合成，例如科比：禾斗匕匕就需要找到禾斗和匕匕，变为科和比，再组合出科比