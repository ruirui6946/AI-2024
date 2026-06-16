# import copy
# import heapq
# from collections import deque
# import time

# class My_PriorityQueue(object):
#     def __init__(self):
#         self._queue = []
#         self._index = 0

#     def push(self, item, priority):
#         """
#         队列由 (priority, index, item) 形式组成
#         priority 增加 "-" 号是因为 heappush 默认是最小堆，我们这里不加负号，因为我们的优先级就是距离，距离越小优先级越高
#         index 是为了当两个对象的优先级一致时，按照插入顺序排列
#         """
#         # heapq.heappush(self._queue, (-priority, self._index, item))
#         heapq.heappush(self._queue, (priority, self._index, item))
#         self._index += 1

#     def pop(self):
#         """
#         弹出优先级最高的对象
#         """
#         return heapq.heappop(self._queue)[-1]

#     def qsize(self):
#         return len(self._queue)

#     def empty(self):
#         return True if not self._queue else False
    
#     def exist_node(self, item):# 是否存在样子为item的样子的结点
#         for i in self._queue:
#             if item.shape == i[-1].shape:
#                 return True 
#         return False
    
#     def same_shape_node(self, item):
#         for i in self._queue:
#             if item.shape == i[-1].shape:
#                 return i
    
#     def delete_node(self, item):
#         for i in self._queue:
#             if item[-1].shape == i[-1].shape:
#                 del i 
#                 break

# class Node(object):
#     def __init__(self, shape, h, parent, direction):
#         self.shape = shape 
#         self.h = h 
#         self.g = self.count(shape)
#         self.parent = parent 
#         self.direction = direction 
    
#     def count(self, shape):# 评估方案1
#         global target
#         num = 0
#         for i in range(3):
#             for j in range(3):
#                 if shape[i][j] != target[i][j]:
#                     num += 1
#         return num 

#     # def count(self, shape):# 评估方案2
#     #     global target 
#     #     num = 0
#     #     for i in range(1, 9):
#     #         pos_i_1, pos_j_1 = -1, -1
#     #         for x in range(3):
#     #             for y in range(3):
#     #                 if shape[x][y] == str(i):
#     #                     pos_i_1, pos_j_1 = x, y
#     #         pos_i_2, pos_j_2 = -1, -1
#     #         for x in range(3):
#     #             for y in range(3):
#     #                 if target[x][y] == str(i):
#     #                     pos_i_2, pos_j_2 = x, y 
#     #         num += pow(pos_i_1 - pos_i_2, 2) + pow(pos_j_1 - pos_j_2, 2)
#     #     return num
    
#     def position(self):
#         for i in range(3):
#             for j in range(3):
#                 if self.shape[i][j] == ' ':
#                     return [i, j]

# def swap(t1, t2):
#     temp = t1 
#     t1 = t2 
#     t2 = temp 
#     return t1, t2

# def Print(node):# 打印
#     dq = deque()
#     while node is not None:
#         dq.append(node)
#         node = node.parent

#     while len(dq):
#         q_front = dq.pop()
#         print(q_front.direction)
#         for i in q_front.shape:
#             for j in i:
#                 print(j, end=' ')
#             print()

# target = [['2', '8', '3'],
#           ['1', '6', '4'],
#           ['7', ' ', '5']]
# origin = [['1', '6', '2'],
#           ['8', ' ', '3'],
#           ['7', '5', '4']]

# if __name__ == "__main__":
#     start_time = time.time()
#     node = Node(origin, 0, None, '')
#     pq = My_PriorityQueue()
#     pq.push(node, node.g + node.h)
#     visited_nodes = My_PriorityQueue()
#     flag = 0
#     num_step = 0
#     while pq.qsize() != 0:
#         num_step += 1
#         q_front = pq.pop()
#         q_front_copy = copy.deepcopy(q_front)
#         visited_nodes.push(q_front_copy, q_front_copy.g + q_front_copy.h)# 已访问
#         if q_front.shape == target:
#             Print(q_front_copy)
#             flag = 1
#             break
#         if pq.exist_node(q_front_copy):# 这个样子已经存在，则观察当前结点和已经走过的节点的g+h谁大谁小
#             visited_node = pq.same_shape_node(q_front_copy)
#             if visited_node[-1].h + visited_node[-1].g < q_front.h + q_front.g:# 之前那个点更优
#                 continue# 抛弃当前点
#             else:# 当前点更好，原来的点出队，当前点入队
#                 pq.delete_node(visited_node)
#                 pq.push(q_front_copy, q_front_copy.g + q_front_copy.h)
#         position = q_front_copy.position()
#         if position[1] < 2: # 右移
#             shape = copy.deepcopy(q_front_copy.shape)
#             shape[position[0]][position[1]], shape[position[0]][position[1] + 1] = shape[position[0]][position[1] + 1], shape[position[0]][position[1]]
#             node = Node(shape, q_front_copy.h + 1, q_front_copy, 'right')
#             pq.push(node, node.g + node.h)
#         if position[1] > 0: # 左移
#             shape = copy.deepcopy(q_front_copy.shape)
#             shape[position[0]][position[1]], shape[position[0]][position[1] - 1] = shape[position[0]][position[1] - 1], shape[position[0]][position[1]]
#             node = Node(shape, q_front_copy.h + 1, q_front_copy, 'left')
#             pq.push(node, node.g + node.h)
#         if position[0] < 2:# 下移
#             shape = copy.deepcopy(q_front_copy.shape)
#             shape[position[0] + 1][position[1]], shape[position[0]][position[1]] = shape[position[0]][position[1]], shape[position[0] + 1][position[1]]
#             node = Node(shape, q_front_copy.h + 1, q_front_copy, 'down')
#             pq.push(node, node.g + node.h)
#         if position[0] > 0:# 上移
#             shape = copy.deepcopy(q_front_copy.shape)
#             shape[position[0] - 1][position[1]], shape[position[0]][position[1]] = shape[position[0]][position[1]], shape[position[0] - 1][position[1]]
#             node = Node(shape, q_front_copy.h + 1, q_front_copy, 'up')
#             pq.push(node, node.g + node.h)
#     if flag == 0:
#         print('no solution')
#     end_time = time.time()
#     print("time spent is %.6f seconds" % (end_time-start_time))
#     print("number of steps is %d"%num_step)


# # 金哥
# import heapq
# import time
# closednum = 1
# # 定义八数码问题的目标状态
# goal_state = ((1, 2, 3),
#               (8, 0, 4),
#               (7, 6, 5))

# # 定义每个状态的移动方向（上、下、左、右）
# directions = [(0, -1), (0, 1), (-1, 0), (1, 0)]

# # 定义八数码问题的初始状态
# initial_state = ((2, 8, 3),
#                  (1, 6, 4),
#                  (7, 0, 5))

# # 计算当前状态与目标状态之间的曼哈顿距离
# def calculate_manhattan_distance(state):
#     distance = 0
#     for i in range(3):
#         for j in range(3):
#             num = state[i][j]
#             if num != 0:
#                 goal_pos = get_blank_position(goal_state,num)
#                 distance += abs(i - goal_pos[0]) + abs(j - goal_pos[1])
#     return distance

# # 计算当前状态与目标状态之间的错位距离
# def calculate_distance(state):
#     distance = 0
#     for i in range(3):
#         for j in range(3):
#             num = state[i][j]
#             goal_pos = get_blank_position(goal_state,num)
#             distance += 1 if goal_pos[0]!=i and goal_pos[1]!=j else 0
#     return distance

# # 获取空白格的位置
# def get_blank_position(state,num):
#     for i in range(3):
#         for j in range(3):
#             if state[i][j] == num:
#                 return i, j

# # 生成当前状态的邻居状态
# def generate_neighbors(state):
#     neighbors = []
#     i, j = get_blank_position(state,0)
#     for direction in directions:
#         new_i = i + direction[0]
#         new_j = j + direction[1]
#         if 0 <= new_i < 3 and 0 <= new_j < 3:
#             neighbor_state = list(map(list, state))  # 将元组转换为列表，元组不可变
#             neighbor_state[i][j], neighbor_state[new_i][new_j] = neighbor_state[new_i][new_j], neighbor_state[i][j]#交换两个元素
#             neighbors.append(tuple(map(tuple, neighbor_state)))  # 将列表转换回元组
#     return neighbors


# # A*算法求解八数码问题
# def solve_puzzle(initial_state):
#     global solution_count,closednum
#     open_list = []# 定义优先队列
#     start_node = (calculate_manhattan_distance(initial_state), 0, initial_state)# 初始化起始状态的代价函数值和步数
#     heapq.heappush(open_list, start_node)# 将起始状态添加到优先队列中
#     visited = set()# 定义已访问状态集合
#     parents = {}# 定义用于回溯路径的字典
#     while open_list:
#         # 从优先队列中取出代价最小的状态
#         current_node = heapq.heappop(open_list)
#         current_state = current_node[2]
#         # 检查是否达到目标状态
#         if current_state == goal_state:
#             solution_count += 1
#             # 回溯路径并输出每一步的状态
#             print("Solution", solution_count)
#             print("Number of steps:", current_node[1])
#             print("Search Time: %.6f seconds" % (time.time() - start_time))
#             print_solution(parents, current_state)
#             print("******************************")
#             break
#         # 将当前状态添加到已访问状态集合中
#         visited.add(tuple(map(tuple, current_state)))
#         # 生成当前状态的邻居状态
#         neighbors = generate_neighbors(current_state)
#         for neighbor_state in neighbors:
#             if tuple(map(tuple, neighbor_state)) not in visited:
#                 closednum += 1
#                 # 计算邻居状态的代价函数值和步数
#                 g = current_node[1] + 1
#                 h = calculate_manhattan_distance(neighbor_state)
#                 f = g + h
#                 # 将邻居状态添加到优先队列中
#                 heapq.heappush(open_list, (f, g, neighbor_state))
#                 # 记录邻居状态的父状态
#                 parents[tuple(map(tuple, neighbor_state))] = current_state

#     if solution_count == 0:
#         print("No solution found!")

# # 回溯路径并输出每一步的状态
# def print_solution(parents, state):
#     if state in parents:
#         print_solution(parents, parents[state])
#     print_state(state)

# # 输出当前状态的方格布局
# def print_state(state):
#     for row in state:
#         print(row)
#     print()

# if __name__ == "__main__":
#     # 定义解决方案数量
#     solution_count = 0
#     # 运行八数码问题的求解
#     print("******************************")
#     start_time = time.time()
#     solve_puzzle(initial_state)
#     end_time = time.time()
#     print("Total search time: %.6f seconds" % (end_time - start_time))
#     print("生成结点个数为 %d" % closednum)

import heapq

# Define initial and goal states
goal_state = (
    (1, 2, 3),
    (8, 0, 4),
    (7, 6, 5)
)

directions = [(0, -1), (0, 1), (-1, 0), (1, 0)]

initial_state = (
    (2, 8, 0),
    (1, 6, 3),
    (7, 5, 4)
)

# Define heuristic functions
def manhattan_distance(state):
    distance = 0
    for i in range(3):
        for j in range(3):
            num = state[i][j]
            if num != 0:
                goal_pos = get_position(goal_state, num)
                distance += abs(i - goal_pos[0]) + abs(j - goal_pos[1])
    return distance

def misplaced_tiles(state):
    distance = 0
    for i in range(3):
        for j in range(3):
            num = state[i][j]
            goal_pos = get_position(goal_state, num)
            distance += 1 if goal_pos[0] != i or goal_pos[1] != j else 0
    return distance

def get_position(state, num):
    for i in range(3):
        for j in range(3):
            if state[i][j] == num:
                return i, j

# Generate neighbor states
def generate_neighbors(state):
    neighbors = []
    i, j = get_position(state, 0)
    for direction in directions:
        new_i, new_j = i + direction[0], j + direction[1]
        if 0 <= new_i < 3 and 0 <= new_j < 3:
            neighbor_state = list(map(list, state))
            neighbor_state[i][j], neighbor_state[new_i][new_j] = neighbor_state[new_i][new_j], neighbor_state[i][j]
            neighbors.append(tuple(map(tuple, neighbor_state)))
    return neighbors

# A* algorithm for solving the puzzle
def solve_puzzle(initial_state):
    global solution_count, closed_node_count
    open_list = []  
    start_node = (misplaced_tiles(initial_state), 0, initial_state)
    heapq.heappush(open_list, start_node)
    visited = set()  
    parents = {}
    while open_list:
        current_node = heapq.heappop(open_list)
        current_state = current_node[2]
        if current_state == goal_state:
            solution_count += 1
            print("Solution", solution_count)
            print("Number of steps:", current_node[1])
            print_solution(parents, current_state)
            break
        visited.add(tuple(map(tuple, current_state)))
        neighbors = generate_neighbors(current_state)
        for neighbor_state in neighbors:
            if tuple(map(tuple, neighbor_state)) not in visited:
                closed_node_count += 1
                g = current_node[1] + 1
                h = misplaced_tiles(neighbor_state)
                f = g + h
                heapq.heappush(open_list, (f, g, neighbor_state))
                parents[tuple(map(tuple, neighbor_state))] = current_state

    if solution_count == 0:
        print("No solution found!")

# Print solution path
def print_solution(parents, state):
    if state in parents:
        print_solution(parents, parents[state])
    print_state(state)

# Print current state
def print_state(state):
    for row in state:
        print(row)
    print()

if __name__ == "__main__":
    solution_count = 0
    closed_node_count = 1
    solve_puzzle(initial_state)
    print("Total generated nodes: %d" % closed_node_count)
