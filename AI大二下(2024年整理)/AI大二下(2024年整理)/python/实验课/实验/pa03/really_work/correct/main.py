import networkx as nx
import codecs
def correct_core_person(relationships):
    # 创建一个无向图对象
    # 添加节点和边
    core_person = {}
    G = nx.Graph()
    for name1, item in relationships.items():
        for name2, dis in item.items():
            G.add_edge(name1, name2, weight=dis)
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
    with codecs.open("./really_work/correct/core_person.txt", "w", "utf8") as f:
        f.write('the number of people:'+str(len(sort_core_person))+'\n')
        id = 0
        for name,importance in sort_core_person.items():
            name = name.replace('\n','').replace('\r','')
            id += 1
            f.write(name+' '+str(importance)+'\n')
            if id == 200:
                break

relation = {}
with open('./really_work/correct/correct_relationship.txt', 'r', encoding='utf8') as f:
    lines = f.readlines()
    for line in lines:
        line = line.split(' ')
        
        if (line[0] in relation) and (line[1] not in relation[line[0]]):
            relation[line[0]][line[1]] = 1
        elif line[0] not in relation:
            relation[line[0]]={line[1]:0}
        else:
            relation[line[0]][line[1]] += 1
        
        if (line[1] in relation) and (line[0] not in relation[line[1]]):
            relation[line[1]][line[0]] = 1
        elif line[1] not in relation:
            relation[line[1]]={line[0]:0}
        else:
            relation[line[1]][line[0]] += 1

correct_core_person(relation)
print('over')