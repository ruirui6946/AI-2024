import numpy as np

def load_data_set():
    """
    创建实验数据集

    Returns:
        tuple: 包含实验数据集和类别标签向量的元组
    """
    data_set = [
        ['my', 'dog', 'has', 'flea', 'problems', 'help', 'please'],
        ['maybe', 'not', 'take', 'him', 'to', 'dog', 'park', 'stupid'],
        ['my', 'dalmation', 'is', 'so', 'cute', 'I', 'love', 'him'],
        ['stop', 'posting', 'stupid', 'worthless', 'garbage'],
        ['mr', 'licks', 'ate', 'my', 'steak', 'how', 'to', 'stop', 'him'],
        ['quit', 'buying', 'worthless', 'dog', 'food', 'stupid']
    ]
    class_labels = [0, 1, 0, 1, 0, 1]  # 类别标签向量，1代表侮辱性词汇，0代表非侮辱性词汇
    return data_set, class_labels

def create_vocab_list(data_set):
    """
    创建词汇表

    Args:
        data_set (list): 包含文档的列表

    Returns:
        list: 词汇表
    """
    vocab_set = set()
    for doc in data_set:
        vocab_set |= set(doc)
    return list(vocab_set)

def set_of_words_to_vec(vocab_list, input_set):
    """
    将词条转换为词条向量

    Args:
        vocab_list (list): 词汇表
        input_set (list): 输入的词条列表

    Returns:
        list: 词条向量
    """
    return_vec = [0] * len(vocab_list)
    for word in input_set:
        if word in vocab_list:
            return_vec[vocab_list.index(word)] = 1
        else:
            print(f"{word} is not in my Vocabulary!")
    return return_vec

def get_train_mat(data_set):
    """
    生成训练集向量列表

    Args:
        data_set (list): 包含文档的列表

    Returns:
        list: 训练集向量列表
    """
    train_mat = []
    vocab_list = create_vocab_list(data_set)
    for input_set in data_set:
        return_vec = set_of_words_to_vec(vocab_list, input_set)
        train_mat.append(return_vec)
    return train_mat

def train_naive_bayes(train_mat, class_labels):
    """
    朴素贝叶斯分类器训练函数

    Args:
        train_mat (list): 训练集向量列表
        class_labels (list): 类别标签向量

    Returns:
        tuple: 包含属于非侮辱类、侮辱类和文档属于侮辱类的概率的对数的元组
    """
    num_docs = len(train_mat)          # 计算训练的文档数目
    num_words = len(train_mat[0])       # 计算每篇文档的词条数
    p_abusive = sum(class_labels) / num_docs    # 文档属于侮辱类的概率
    p0_num, p1_num = np.ones(num_words), np.ones(num_words)
    p0_denom, p1_denom = 2, 2

    for i in range(num_docs):
        if class_labels[i] == 1:
            p1_num += train_mat[i]
            p1_denom += sum(train_mat[i])
        else:
            p0_num += train_mat[i]
            p0_denom += sum(train_mat[i])

    p1_v = np.log(p1_num / p1_denom)
    p0_v = np.log(p0_num / p0_denom)

    return p0_v, p1_v, p_abusive

def classify_naive_bayes(vec_to_classify, p0_v, p1_v, p_abusive):
    """
    朴素贝叶斯分类函数

    Args:
        vec_to_classify (list): 待分类的文档向量
        p0_v (numpy.ndarray): 属于非侮辱类的条件概率的对数
        p1_v (numpy.ndarray): 属于侮辱类的条件概率的对数
        p_abusive (float): 文档属于侮辱类的概率

    Returns:
        int: 分类结果，1表示属于侮辱类，0表示属于非侮辱类
    """
    p1 = sum(vec_to_classify * p1_v) + np.log(p_abusive)
    p0 = sum(vec_to_classify * p0_v) + np.log(1 - p_abusive)
    # 用对数可以解决小数相乘带来的下溢问题
    return 1 if p1 > p0 else 0

def test_naive_bayes(test_vector):
    """
    朴素贝叶斯测试函数

    Args:
        test_vector (list): 待测试的文档向量
    """
    # 加载数据集和类别标签向量
    data_set, class_labels = load_data_set()
    
    # 创建词汇表
    vocab_list = create_vocab_list(data_set)
    
    # 生成训练集向量列表
    training_matrix = get_train_mat(data_set)
    
    # 训练朴素贝叶斯分类器
    p0_vector, p1_vector, p_ab = train_naive_bayes(training_matrix, class_labels)
    
    # 将测试样本向量化
    test_vectorized = set_of_words_to_vec(vocab_list, test_vector)
    
    # 进行分类
    classification_result = classify_naive_bayes(test_vectorized, p0_vector, p1_vector, p_ab)
    
    # 输出分类结果
    category = "侮辱类" if classification_result == 1 else "非侮辱类"
    print(f"{test_vector} 属于 {category}")

# 测试用例1
test_vec1 = ['love', 'my', 'dalmation']
test_naive_bayes(test_vec1)

# 测试用例2
test_vec2 = ['stupid', 'garbage']
test_naive_bayes(test_vec2)

# 测试用例3: 包含所有正面词汇的例子
test_vec3 = ['my', 'dog', 'is', 'cute', 'I', 'love', 'him']
test_naive_bayes(test_vec3)

# 测试用例4: 包含所有负面词汇的例子
test_vec4 = ['stupid', 'worthless', 'garbage', 'posting']
test_naive_bayes(test_vec4)

# 测试用例5: 包含正面和负面词汇的例子
test_vec5 = ['my', 'dog', 'is', 'stupid', 'I', 'love', 'him']
test_naive_bayes(test_vec5)

# 测试用例6: 包含重复词汇的例子
test_vec6 = ['my', 'dog', 'dog', 'problems', 'help', 'please']
test_naive_bayes(test_vec6)

# +--------------+-----------+-------------------+
# | 测试用例编号 | 实际类别  | 分类结果         |
# +--------------+-----------+-------------------+
# | 1            | 非侮辱类  | 非侮辱类          |
# | 2            | 侮辱类    | 侮辱类            |
# | 3            | 非侮辱类  | 非侮辱类          |
# | 4            | 侮辱类    | 侮辱类            |
# | 5            |          | 非侮辱类           |
# | 6            | 非侮辱类  | 非侮辱类          |
# +--------------+-----------+-------------------+

