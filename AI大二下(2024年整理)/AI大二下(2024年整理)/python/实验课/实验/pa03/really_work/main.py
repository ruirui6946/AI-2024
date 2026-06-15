import jieba, codecs
import jieba.posseg as pseg
from pyecharts import options as opts
from pyecharts.charts import Graph
import time
import networkx as nx

class RelationExtractor:

	def __init__(self, fpStopWords, fpNameDicts):
		# 人名词典
		self.name_dicts = [line.strip().split(' ')[0] for line in open(fpNameDicts,'rt',encoding='utf-8').readlines()]
		# 停止词表
		self.stop_words = [line.strip() for line in open(fpStopWords,'rt',encoding='utf-8').readlines()]
		# 加载词典
		jieba.load_userdict(fpNameDicts)

	def extract(self, fpText):
		# 人物关系
		relationships = {}
		# 人名频次
		name_frequency = {}
		# 每个段落中的人名
		name_in_paragraph = []

        # 读取文本，统计人名出现的频次，以及每个段落中出现的人名
		with codecs.open(fpText, "r", "utf8") as f:
			for line in f.readlines():
				poss = pseg.cut(line)
				name_in_paragraph.append([])
				for w in poss:
					if w.flag != "nr" or len(w.word) < 2: # nr专门用于标记人名
						continue
					if (w.word in self.stop_words):# 停用词
						continue
					if (not w.word in self.name_dicts):# 不在人名字典中
						continue
					word = w.word
					name_in_paragraph[-1].append(word) # 添加这个名字
					if name_frequency.get(word) is None:
						name_frequency[word] = 0
						relationships[word] = {}
					name_frequency[word] += 1

		# 基于共现组织人物关系
		for paragraph in name_in_paragraph:# 每个段落
			for name1 in paragraph: # 每个段落中的名字
				for name2 in paragraph:
					if name1 == name2:
						continue
					if relationships[name1].get(name2) is None:
						relationships[name1][name2] = 1
					else:
						relationships[name1][name2] = relationships[name1][name2] + 1
        
		print('一共有'+str(len(name_frequency))+'个人物')

		# 返回节点和边
		return name_frequency, relationships

	def exportGephi(self, nodes, relationships):
		# 输出节点
		with codecs.open("./really_work/output/node.txt", "w", "gbk") as f:
			f.write("Id Label Weight\r\n")
			for name, freq in nodes.items():
				f.write(name + " " + str(freq) + "\r\n")
		# 输出边
		with codecs.open("./really_work/output/edge.txt", "w", "gbk") as f:
			f.write("Source Target Weight\r\n")
			for name, edges in relationships.items():
				for v, w in edges.items():
					if w > 0:
						f.write(name + " " + v + " " + str(w) + "\r\n")   

	def exportECharts(self, nodes, relationships):
		# 总频次，用于数据的归一化
		total = sum(list(map(lambda x:x[1], nodes.items())))

		# 输出节点
		nodes_data = []
		for name, freq in nodes.items():
			if freq > 300:# 比较重要的人物才画出来
				nodes_data.append(opts.GraphNode(
					name = name, 
					symbol_size = round(freq / total * 100, 2), 
					value = freq,
				)),

		# 输出边
		links_data = []
		for name, edges in relationships.items():
				for v, w in edges.items():
					# if w > 200:# 比较重要的关系才画出来
					links_data.append(opts.GraphLink(source = v, target = w, value = w))

		# 绘制Graph
		c = (
			Graph()
			.add(
				"",
				nodes_data,
				links_data,
				gravity = 0.2,
				repulsion = 8000,
				is_draggable = True,
				symbol = 'circle',
				linestyle_opts = opts.LineStyleOpts(
					curve = 0.3, width = 0.5, opacity = 0.7
				),
				edge_label = opts.LabelOpts(
					is_show = False, position = "middle", formatter = "{b}->{c}"
				),
			)
			.set_global_opts(
				title_opts = opts.TitleOpts(title="实验三人物关系抽取")
			)
			.render("./really_work/docs/实验三人物关系抽取.html")
		)

	def compute_core(self, relationships):
		# 创建一个无向图对象
		# 添加节点和边
		core_person = {}
		G = nx.Graph()
		for name1, item in relationships.items():
			for name2, dis in item.items():
				G.add_edge(name1, name2, weight=5000-dis)
		# 接近中心性 (Closeness Centrality)：衡量节点到其他所有节点的平均最短路径长度。
		# 接近中心性较高的节点表示在网络中距离其他节点较近
		closeness_centrality = nx.closeness_centrality(G)
		# 度中心性 (Degree Centrality)：一个节点的度是与其相连的边数。
		# 度中心性是一个节点的度相对于网络中所有其他节点的度的比例。
		# 简单地说，度中心性衡量了一个节点与其他节点之间的连通性
		degree_centrality = nx.degree_centrality(G)
		# 介数中心性 (Betweenness Centrality)：表示节点在网络中的所有最短路径上的出现频率。
		# 较高的介数中心性意味着节点在网络中扮演了“桥梁”角色，连接了不同的节点群组。
		betweenness_centrality = nx.betweenness_centrality(G)
		# 特征向量中心性 (Eigenvector Centrality)：度量节点在网络中的影响力。
		# 具有较高特征向量中心性的节点不仅本身连接了很多节点，而且它们的邻居节点也具有较高的连接性。
		eigenvector_centrality = nx.eigenvector_centrality(G)
		# PageRank：最初由 Google 的创始人设计用于网页排名，但也可以用于分析网络中节点的重要性。
		# PageRank 考虑了节点的链接数量和链接质量，给出了节点在网络中的相对重要性。
		pagerank = nx.pagerank(G)

		core_person = {}
		for name,a,b,c,d,e in zip(closeness_centrality.keys(),
								closeness_centrality.values(),
								degree_centrality.values(),
								betweenness_centrality.values(),
								eigenvector_centrality.values(),
								pagerank.values()):
			core_person[name]=a+b+c+d+e
		sort_core_person = sorted(core_person.items(), key=lambda x:x[1], reverse=True)
		sort_core_person = dict(sort_core_person)
		with codecs.open("./really_work/output/sorted.txt", "w", "utf8") as f:
			f.write('the number of people:'+str(len(sort_core_person))+'\n')
			id = 0
			for name,importance in sort_core_person.items():
				id += 1
				f.write(name+' '+str(importance)+'\n')
				if id == 200:
					break
		
if (__name__ == '__main__'):
	# 记录程序开始时间
	start_time = time.time()

	extractor = RelationExtractor(
		'./really_work/input/停用词词典.txt',
		'./really_work/input/人名词典.txt'
	)
	nodes, relationships = extractor.extract('./really_work/input/人物分析.txt')
	# extract函数耗时251.80秒

	extractor.exportGephi(nodes, relationships)
	extractor.exportECharts(nodes, relationships)

	extractor.compute_core(relationships)
	# 记录程序结束时间
	end_time = time.time()
	# 计算程序运行时间
	run_time = end_time - start_time
	print(f'程序运行时间为：{run_time:.2f} 秒')