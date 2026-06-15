# Third Party
import sys
import numpy as np
import os
from matplotlib import pyplot as plt

# In House
from inspection import calc_entropy, DataInterface, MajorityVoteClassifier, write_error

def calc_mi(data, lbls, hdrs, attr):
    """
    Calculate the mutual information for a certain attribute

    :param data: The data left at the current root node, 即feature matrix
    :param lbls: label vector
    :param hdrs: attribution_names
    :param attr: Which attribute to split the data on,即要计算的某个特征
    """
    if attr not in hdrs:
        return None

    # Entropy at current level - weighted sum of split entropies
    mi = calc_entropy(lbls)    # 计算熵， H(Y)

    # Find the column we need
    idx         = attr == hdrs   # idx=[False, False, True, False, False]
    # 上面对hdrs数据进行逻辑比较，这将创建一个布尔类型的数组，其中True表示预attr相等的元素位置，而其他位置则为False
    feature     = data[:, idx]  # data[:, idx]是使用布尔索引对data数据进行切片操作，以选择与True对应位置相对应的列
    # feature是一个二维numpy数组，包含选定列的数据
    num_samples = len(feature)# len(feature)返回的是feature数组的第一维的长度，即行数

    # Find the number of unique values
    unique_vals = np.unique(feature)  # 统计此属性的各类出现的次数，比如这个属性有0和1两类，则unique_vals就是表示0和1的个数

    # For each value, subtract the weighted entropy from the entropy before splitting
    for val in unique_vals:
        # Find out how many samples have that value (percentage) and what is the entropy of the labels
        val_mask        = feature == val
        val_percentage  = len(np.extract(val_mask, feature)) / num_samples # 其实这里就相当于是np.extract(feature==val, feature), 提取出来的是feature == val位置上的feature的元素
        # Percentage of samples with that label * the entropy of the labels where the sample has that value

        # 把val_mask铺平，取出val_mask中为True的对应的lbls位置上的值
        mi -= val_percentage * calc_entropy(lbls[val_mask.ravel()])  # H(Y|X) = sum p_i H(Y|X=x_i)

    return mi

class DecisionTree:
    def __init__(self, max_depth):  # max_depth = 2
        self.max_depth = int(max_depth)
        self.mvc       = MajorityVoteClassifier()  # Majority_Vote_Classifier多数投票分类器
        self.tree      = None

    def fit(self, X, y, hdrs):  # 传入feature matrix, label, attribution_name(headers)
        self.tree = self.recurse_tree(X, y, hdrs)
        return self.tree

    def predict_batch(self, node, X_test, test_hdrs):
        # X_test: 即tsv文件中除了第一行attribution 的其他信息
        # test_hdrs: 除了最后一个label不要，其他都要，即hdrs是属性的名称
        # node: 根节点
        preds = []
        for sample in X_test: # 一个样本一个样本去判断
            sample_dict = create_sample_dict(sample, test_hdrs)  # 用字典的形式把此样本的属性和对应的值保存起来到sample_dict中
            preds.append(self.predict(node, sample_dict))
        return preds

    def predict(self, node, X_dict):
        ''' 
        node: 即构建好的决策树
        X_dict: 即字典的形式，把各个属性和其对应的值凑成key:value
        '''
        if node is None:
            return None

        # Return value 返回预测的类别
        ret = None

        # Base case, node is a leaf node
        if node.vote is not None:
            ret =  node.vote
        # Recursive Step:
        else:
            if node.attr in X_dict.keys():
                attr_val = X_dict[node.attr]
                if attr_val == 0:
                    ret = self.predict(node.right, X_dict)
                else:
                    ret = self.predict(node.left, X_dict)
        return ret

    def recurse_tree(self, X, y, hdrs, depth = 0):
        # 传入feature matrix, label, attribution_name(headers)
        # depth应该是记录的当前node的深度？？？

        # Create a node for the current branch
        curr = Node()

        # Criteria if we are stopping (is a leaf node)   这里的max_depth=2
        leaf = (depth >= self.max_depth or 
                X is None or np.prod(X.shape) == 0 or
                y is None or np.prod(y.shape) == 0 or
                len(np.unique(y)) == 1 or
                len(np.unique(X)) == 1)  # 只要有一个满足leaf就为True; 否则leaf为False
        '''
        depth >= self.max_depth：如果当前节点的深度超过了最大深度限制（由变量max_depth指定），则认为该节点为叶子节点。
        X is None or np.prod(X.shape) == 0：如果输入的特征数据X为空或者特征数据的形状为空（没有样本），则认为该节点为叶子节点。
        y is None or np.prod(y.shape) == 0：如果输入的目标数据y为空或者目标数据的形状为空（没有样本），则认为该节点为叶子节点。
        len(np.unique(y)) == 1：如果目标数据y中的唯一值数量为1，即所有样本都属于同一个类别，那么认为该节点为叶子节点。
        len(np.unique(X)) == 1：如果特征数据X中的唯一值数量为1，即所有样本都具有相同的特征值，那么认为该节点为叶子节点。
        如果满足上述任何一个条件，那么节点将被视为叶子节点，停止继续分裂。这些条件用于控制决策树的生长过程，以避免过度拟合和无意义的分裂。
        '''

        # Printing the split of 0's and 1's for this branch of the tree
        print(f"[{len(np.extract(y == 0, y))} 0/{len(np.extract(y == 1, y))} 1]")  # 输出label中0的个数和1的个数
        # np.extract(y==0, y)是提取出y中为0的元素，len(np.extract(y == 0, y))表示统计y中为0的元素的个数

        # Base Case (Leaf Node)
        if leaf:# 如果是叶节点
            self.mvc.fit(y)  # Majority_vote_classifier, 输入标签，训练mvc，统计得到最多的票的类别
            curr.vote = self.mvc.infer()  # mvc进行inference,得到当前leaf的类别 (即y=0 还是 y=1)
        
        # Recursive Step (Internal Node)
        else:
            attr_mi = {}  # 各个属性的mutual information,计算各个属性的mi,用字典的形式存储各个属性的信息增益
            # 信息增益就是互信息，信息增益大的特征具有更强的分类能力， g(Y,A) = H(Y) - H(Y|A)

            # Find the mutual information for each attribute
            for hdr in hdrs:
                attr_mi[hdr] = calc_mi(X, y, hdrs, hdr) # 输入feature matrix, label_vector, attribution_names, one_attribution

            # Find out which feature had the highest mutual information
            split_idx = np.argmax(np.array(list(attr_mi.values())))
            curr.attr = hdrs[split_idx] # Mark the split attribute for the current node

            # Mark the headers that are left after the split
            remaining_hdrs = np.r_[hdrs[:split_idx], hdrs[split_idx+1:]]  # 沿水平方向堆叠余下的属性

            # Take each value for this feature (0 or 1)
            # Left child always corresponds to 1, Right child always corresponds to 0
            left_idxs  = X[:, split_idx] == 1  # 所有样本的此属性如果是1，则应该走left分支；如果是0则应该走right分支
            right_idxs = X[:, split_idx] == 0

            # Recursion and printing for debugging
            pipes = "| " * (depth + 1)
            print(f"{pipes}{hdrs[split_idx]} = 0: ", end="")
            # 剔除X矩阵的当前列的信息
            curr.right = self.recurse_tree(np.c_[X[right_idxs, :split_idx], X[right_idxs, split_idx+1:]], y[right_idxs], remaining_hdrs, depth+1)
            print(f"{pipes}{hdrs[split_idx]} = 1: ", end="")
            curr.left = self.recurse_tree(np.c_[X[left_idxs, :split_idx], X[left_idxs, split_idx+1:]], y[left_idxs], remaining_hdrs, depth+1)

        return curr
        
class Node:
    """
    Here is an arbitrary Node class that will form the basis of your decision
    tree. 
    Note:
        - the attributes provided are not exhaustive: you may add and remove
        attributes as needed, and you may allow the Node to take in initial
        arguments as well
        - you may add any methods to the Node class if desired 
    """
    def __init__(self):
        self.left = None    # 左1右0  
        self.right = None
        self.attr = None   # attribution_name
        self.vote = None   # 如果是leaf, 则会有vote, 即y的类别; 如果为内部节点，则为None

def create_sample_dict(sample, hdrs):
    out = {}
    for idx, hdr in enumerate(hdrs):
        out[hdr] = sample[idx]
    return out

def write_preds(file, preds):
    # Write the test results to a file
    with open(file, "w+") as f:
        for pred in preds:
            f.write(f"{pred}\n")

def run_decision_tree(train_input, test_input, max_depth, train_out, test_out, metrics_out):
    """
    Run the decision tree algorithm

    :param train_input: The filepath to the train set
    :param test_input:  The filepath to the test set
    :param max_depth:   Maximum depth of the tree
    :param train_out:   Where to output the predicted labels of the train set
    :param test_out:    Where to output the predicted labels of the test set
    :param metrics_out: Where to output the train and test error metrics
    """

    # Read the training file
    data_interface = DataInterface()
    data_interface.read_tsv(train_input)
    train_data = data_interface.get_data()   # 拿到train_data的数据

    # headers
    hdrs = data_interface.get_headers() # 获取tsv文件的第一行的属性名称以及类别名称
    X = train_data[:, :-1]  # 获取feature matrix,不要最后的label列
    y = train_data[:, -1]   # 只要最后的label列
    hdrs = hdrs[:-1]        # 除了最后一个label不要，其他都要，即hdrs是属性的名称


    # Create a decision tree object
    dt = DecisionTree(max_depth)
    tree = dt.fit(X, y, hdrs) # 传入feature matrix, label, attribution_name(headers)  # 构造决策树

    # Write the predictions of the training set
    train_preds = dt.predict_batch(tree, train_data, hdrs)     # 用构造好的决策树来进行预测训练集中样本的类别
    write_preds(train_out, train_preds)  # 将预测的结果train_preds写入文件train_out中

    # Read the test data
    data_interface.read_tsv(test_input)
    test_data  = data_interface.get_data()
    test_hdrs  = data_interface.get_headers()

    # Write the predictions of the test set
    test_preds = dt.predict_batch(tree, test_data, test_hdrs)  # 用构造好的决策树来进行预测测试集中样本的类别
    write_preds(test_out, test_preds)   # 将预测的结果test_preds写入文件test_out中,为字符串

    # print('test_out:',type(test_out),'  test_data: ',type(test_data))

    # train_out为字符串，而y为numpy.ndarray
    # Write the metrics
    train_error, test_error = write_error(train_out, y, test_out, test_data[:, -1], metrics_out)

    return train_error, test_error

def str_to_int_array(str):
    """
    Quick and dirty string array to numeric array

    :param str: String list ex: "[1,2,3]"

    Returns: list: Example: [1,2,3]
    """
    arr  = []
    junk = True
    for char in str:
        if char == "]":
            return arr
        elif char == "[":
            junk = False
        else:
            if not junk and char.isnumeric():
                arr.append(int(char))
    return arr

def plot_max_depths(train_input, test_input, depths, out_path):
    # Array of collected training and testing errors
    train_errors = []
    test_errors  = []

    # Datset identifier for output files
    dataset_str = os.path.split(train_input)[-1][0:5]

    # Use all possible depths depending on the size of the dataset
    if not depths:  # depth = []
        # Not the cleanest way, but this is not industry/research code, ideally we would frontload some computation or reuse some computation
        data_interface = DataInterface()
        data_interface.read_tsv(train_input)
        train_data = data_interface.get_data()
        depths = list(range(train_data.shape[1] + 1)) # 属性有2个，标签y是一个，则depths = [0, 1, 2, 3]
    # Convert string array ex: "[1,2,3]" to a real python list
    else:
        depths = str_to_int_array(depths)

    for depth in depths:
        out_train = out_path + "/" + f"{dataset_str}_{depth}_train.txt"
        out_test  = out_path + "/" + f"{dataset_str}_{depth}_test.txt"
        out_met   = out_path + "/" + f"{dataset_str}_{depth}_metrics.txt"

        # Run for a given depth
        train_error, test_error = run_decision_tree(train_input, test_input, depth, out_train, out_test, out_met)

        # Add to the list to be plotted
        train_errors.append(train_error)
        test_errors.append(test_error)

    plt.title("Training and Test Error vs. Decision Tree Max Depth")
    plt.plot(depths, train_errors, "ko--", label = "Train Error")
    plt.plot(depths, test_errors, "rx-",   label = "Test Error")
    plt.xlabel("Max Depth of Decision Tree")
    plt.ylabel("Percent Error")
    plt.legend(loc = "upper right")
    plt.show()
    plt.savefig(f"{out_path}/{dataset_str}.png")
    

if __name__ == '__main__':
    
    # Base Homework Use Case
    if len(sys.argv) == 7:
        train_input = sys.argv[1]
        test_input  = sys.argv[2]
        max_depth   = sys.argv[3]
        train_out   = sys.argv[4]
        test_out    = sys.argv[5]
        metrics_out = sys.argv[6]
        run_decision_tree(train_input, test_input, max_depth, train_out, test_out, metrics_out)
    
    # Plotting over the max_depth hyperparameter
    elif len(sys.argv) == 5:
        # python decision_tree.py small_train.tsv small_test.tsv [1,2,3] out_path_test
        train_input = sys.argv[1]
        test_input  = sys.argv[2]
        max_depths  = sys.argv[3]  # string list like: [0,1,2,3]
        out_path    = sys.argv[4]  # the path to save variable output files
        plot_max_depths(train_input, test_input, max_depths, out_path)
    
    elif len(sys.argv) == 4:
        train_input = sys.argv[1]
        test_input  = sys.argv[2]
        out_path    = sys.argv[3]
        plot_max_depths(train_input, test_input, [], out_path)

    else:
        raise ValueError("python decision_tree.py <train input> <test input> <max depth> <train out> <test out> <metrics out>")

#  python decision_tree.py heart_train.tsv heart_test.tsv 4 heart_4_train.txt heart_4_test.txt heart_4_metrics.txt > heart_depth_4.txt
#  python decision_tree.py small_train.tsv small_test.tsv 2 small_2_train.txt small_2_test.txt small_2_metrics.txt > small_depth_2.txt
#  python decision_tree.py education_train.tsv education_test.tsv 3 edu_3_train.txt edu_3_test.txt edu_3_metrics.txt > edu_depth_3.txt
#  python decision_tree.py education_train.tsv education_test.tsv 4 edu_4_train.txt edu_4_test.txt edu_4_metrics.txt > edu_depth_4.txt