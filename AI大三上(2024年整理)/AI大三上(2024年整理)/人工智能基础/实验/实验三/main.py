from sklearn import datasets
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def normalize(X, axis=-1, p=2):
    """对数据集 X 进行正则化。

    Parameters:
    -----------
    X : numpy array
        待正则化的数据集.
    axis : int, optional (默认为 -1)
        指定正则化的轴.
    p : int, optional (默认为 2)
        指定正则化的阶数.

    Returns:
    --------
    numpy array
        正则化后的数据集.
    """
    lp_norm = np.atleast_1d(np.linalg.norm(X, p, axis))
    lp_norm[lp_norm == 0] = 1
    return X / np.expand_dims(lp_norm, axis)

def euclidean_distance(one_sample, X):
    """计算一个样本与数据集中所有样本的欧氏距离的平方。

    Parameters:
    -----------
    one_sample : numpy array
        待计算欧氏距离的样本.
    X : numpy array
        数据集.

    Returns:
    --------
    numpy array
        包含欧氏距离平方的数组.
    """
    one_sample = one_sample.reshape(1, -1)
    X = X.reshape(X.shape[0], -1)
    distances = np.power(np.tile(one_sample, (X.shape[0], 1)) - X, 2).sum(axis=1)
    return distances

class KMeans:
    """Kmeans聚类算法.

    Parameters:
    -----------
    k : int, optional (默认为 2)
        聚类的数目.
    max_iterations : int, optional (默认为 500)
        最大迭代次数. 
    varepsilon : float, optional (默认为 0.0001)
        阈值.
        判断是否收敛, 如果上一次的所有k个聚类中心与本次的所有k个聚类中心的差都小于varepsilon, 
        则说明算法已经收敛
    """
    def __init__(self, k=2, max_iterations=500, varepsilon=0.0001):
        self.k = k
        self.max_iterations = max_iterations
        self.varepsilon = varepsilon

    def init_random_centroids(self, X):
        """从所有样本中随机选取self.k样本作为初始的聚类中心。

        Parameters:
        -----------
        X : numpy array
            数据集.

        Returns:
        --------
        numpy array
            初始的聚类中心.
        """
        n_samples, n_features = np.shape(X)
        centroids = np.zeros((self.k, n_features))
        for i in range(self.k):
            centroid = X[np.random.choice(range(n_samples))]
            centroids[i] = centroid
        return centroids

    def closest_centroid(self, sample, centroids):
        """返回距离该样本最近的一个中心索引，范围[0, self.k)。

        Parameters:
        -----------
        sample : numpy array
            待分类的样本.
        centroids : numpy array
            当前的聚类中心.

        Returns:
        --------
        int
            距离最近的聚类中心的索引.
        """
        distances = euclidean_distance(sample, centroids)
        closest_i = np.argmin(distances)
        return closest_i

    def create_clusters(self, centroids, X):
        """将所有样本进行归类，归类规则就是将该样本归类到与其最近的中心。

        Parameters:
        -----------
        centroids : numpy array
            当前的聚类中心.
        X : numpy array
            数据集.

        Returns:
        --------
        list
            包含每个聚类中心对应的样本索引列表的列表.
        """
        n_samples = np.shape(X)[0]
        clusters = [[] for _ in range(self.k)]
        for sample_i, sample in enumerate(X):
            centroid_i = self.closest_centroid(sample, centroids)
            clusters[centroid_i].append(sample_i)
        return clusters

    def update_centroids(self, clusters, X):
        """对中心进行更新。

        Parameters:
        -----------
        clusters : list
            包含每个聚类中心对应的样本索引列表的列表.
        X : numpy array
            数据集.

        Returns:
        --------
        numpy array
            更新后的聚类中心.
        """
        n_features = np.shape(X)[1]
        centroids = np.zeros((self.k, n_features))
        for i, cluster in enumerate(clusters):
            centroid = np.mean(X[cluster], axis=0)
            centroids[i] = centroid
        return centroids

    def get_cluster_labels(self, clusters, X):
        """将所有样本进行归类，其所在的类别的索引就是其类别标签。

        Parameters:
        -----------
        clusters : list
            包含每个聚类中心对应的样本索引列表的列表.
        X : numpy array
            数据集.

        Returns:
        --------
        numpy array
            包含每个样本所属类别的标签.
        """
        y_pred = np.zeros(np.shape(X)[0])
        for cluster_i, cluster in enumerate(clusters):
            for sample_i in cluster:
                y_pred[sample_i] = cluster_i
        return y_pred

    def predict(self, X):
        """对整个数据集X进行Kmeans聚类，返回其聚类的标签。

        Parameters:
        -----------
        X : numpy array
            数据集.

        Returns:
        --------
        numpy array
            包含每个样本所属类别的标签.
        """
        centroids = self.init_random_centroids(X)

        for _ in range(self.max_iterations):
            clusters = self.create_clusters(centroids, X)
            former_centroids = centroids

            centroids = self.update_centroids(clusters, X)

            diff = centroids - former_centroids
            if np.all(diff < self.varepsilon):
                break

        return self.get_cluster_labels(clusters, X)

def main():
    # 加载数据集
    X, y = datasets.make_blobs(n_samples=10000, 
                               n_features=3, 
                               centers=[[0,0,0], [1,1,1], [2,2,2], [3,3,3]], 
                               cluster_std=[0.1, 0.2, 0.1, 0.2], 
                               random_state=5)

    # 聚类
    clf = KMeans(k=4, varepsilon=1e-5)
    y_pred = clf.predict(X)

    # 可视化
    fig = plt.figure(figsize=(12, 8))
    ax = Axes3D(fig, rect=[0, 0, 1, 1], elev=30, azim=20)
    for i in range(4):
        plt.scatter(X[y_pred==i][:, 0], X[y_pred==i][:, 1], X[y_pred==i][:, 2])# 画出聚类后得到的数据分布
        # plt.scatter(X[y==i][:, 0], X[y==i][:, 1], X[y==i][:, 2])# 画出数据最初的分布
    plt.show()

if __name__ == "__main__":
    main()