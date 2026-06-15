## 马尔可夫模型

在之前的笔记中，我们讨论了贝叶斯网络，以及它们如何被用于紧凑地表示随机变量之间的关系。现在，我们将介绍一个与之紧密相关的结构，称为马尔可夫模型，对于本课程的目的，可以将其视为类似于链式的、无限长度的贝叶斯网络。我们在这一部分中将要使用的运行示例是日常天气模式的波动。我们的天气模型将是时间相关的（就像马尔可夫模型通常是的），这意味着我们将有一个单独的随机变量代表每天的天气。如果我们定义 $W_{i}$ 为代表第 $i$ 天天气的随机变量，那么我们天气示例的马尔可夫模型看起来像这样：

![](https://cdn.mathpix.com/cropped/2023_11_28_28f56ea6fe53f667bbb0g-01.jpg?height=128&width=1025&top_left_y=1080&top_left_x=607)

我们应该存储关于马尔可夫模型中涉及的随机变量的哪些信息？为了跟踪我们正在考虑的数量（在这种情况下，是天气）随时间的变化，我们需要知道它在时间 $t=0$ 时的初始分布以及一些转移模型，这些模型表征了从一个状态到另一个状态之间的概率。马尔可夫模型的初始分布由概率表 $P\left(W_{0}\right)$ 给出，从状态 $i$ 转移到 $i+1$ 的转移模型由 $P\left(W_{i+1} \mid W_{i}\right)$ 给出。注意，这个转移模型意味着 $W_{i+1}$ 的值只与 $W_{i}$ 的值有条件依赖。换句话说，时间 $t=i+1$ 的天气满足马尔可夫性质或无记忆性质，并且除了 $t=i$ 以外的所有其他时间步与天气无关。

使用我们的天气马尔可夫模型，如果我们想要用链式规则重构 $W_{0}, W_{1}$ 和 $W_{2}$ 之间的联合概率，我们会希望：

$$
P\left(W_{0}, W_{1}, W_{2}\right)=P\left(W_{0}\right) P\left(W_{1} \mid W_{0}\right) P\left(W_{2} \mid W_{1}, W_{0}\right)
$$

然而，根据我们的假设，马尔可夫性质成立且 $W_{0} \perp W_{2} \mid W_{1}$，联合概率简化为：

$$
P\left(W_{0}, W_{1}, W_{2}\right)=P\left(W_{0}\right) P\left(W_{1} \mid W_{0}\right) P\left(W_{2} \mid W_{1}\right)
$$

我们有了从马尔可夫模型计算所需的一切。更一般地说，马尔可夫模型在每个时间步做出以下独立假设：$W_{i+1} \perp\left\{W_{0}, \ldots, W_{i-1}\right\} \mid W_{i}$。这使我们能够通过链式规则重构前 $n+1$ 个变量的联合分布，如下所示：

$$
P\left(W_{0}, W_{1}, \ldots, W_{n}\right)=P\left(W_{0}\right) P\left(W_{1} \mid W_{0}\right) P\left(W_{2} \mid W_{1}\right) \ldots P\left(W_{n} \mid W_{n-1}\right)=P\left(W_{0}\right) \prod_{i=0}^{n-1} P\left(W_{i+1} \mid W_{i}\right)
$$

马尔可夫模型中通常做出的最后一个假设是转移模型是平稳的。换句话说，对于所有的 $i$ 值（所有时间步），$P\left(W_{i+1} \mid W_{i}\right)$ 是相同的。这使我们能够仅用两个表格表示一个马尔可夫模型：一个用于 $P\left(W_{0}\right)$，另一个用于 $P\left(W_{i+1} \mid W_{i}\right)$。

## 迷你前向算法

我们现在知道如何计算马尔可夫模型随时间步的联合分布。然而，这并没有明确帮助我们回答关于给定某一天 $t$ 的天气分布的问题。当然，我们可以计算联合概率，然后将所有其他变量边缘化（求和），但这通常是极其低效的，因为如果我们有 $j$ 个变量，每个变量可以取 $d$ 个值，联合分布的大小是 $O\left(d^{j}\right)$。相反，我们将介绍一种更有效的技术，称为迷你前向算法。

这是它的工作原理。根据边缘化的性质，我们知道

$$
P\left(W_{i+1}\right)=\sum_{w_{i}} P\left(w_{i}, W_{i+1}\right)
$$

通过链式规则，我们可以将其重新表达为：

$$
P\left(W_{i+1}\right)=\sum_{w_{i}} P\left(W_{i+1} \mid w_{i}\right) P\left(w_{i}\right)
$$

这个方程应该有一些直观的意义 - 要计算时间步 $i+1$ 的天气分布，我们看看由 $P\left(W_{i}\right)$ 给出的时间步 $i$ 的概率分布，并用我们的转移模型 $P\left(W_{i+1} \mid W_{i}\right)$ 将这个模型“推进”一个时间步。有了这个方程，我们可以通过从初始分布 $P\left(W_{0}\right)$ 开始，用它来计算 $P\left(W_{1}\right)$，然后依次使用 $P\left(W_{1}\right)$ 来计算 $P\left(W_{2}\right)$，等等，迭代地计算任何我们选择的时间步的天气分布。让我们用以下初始分布和转移模型来举一个例子：

| $W_{0}$ | $P\left(W_{0}\right)$ |
| :---: | :---: |
| sun | 0.8 |
| rain | 0.2 |


| $W_{i+1}$ | $W_{i}$ | $P\left(W_{i+1} \mid W_{i}\right)$ |
| :---: | :---: | :---: |
| sun | sun | 0.6 |
| rain | sun | 0.4 |
| sun | rain | 0.1 |
| rain | rain | 0.9 |

使用迷你前向算法，我们可以这样计算 $P\left(W_{1}\right)$：

$$
\begin{aligned}
P\left(W_{1}=\text { sun }\right) & =\sum_{w_{0}} P\left(W_{1}=\operatorname{sun} \mid w_{0}\right) P\left(w_{0}\right) \\
& =P\left(W_{1}=\operatorname{sun} \mid W_{0}=\operatorname{sun}\right) P\left(W_{0}=\text { sun }\right)+P\left(W_{1}=\operatorname{sun} \mid W_{0}=\text { rain }\right) P\left(W_{0}=\text { rain }\right) \\
& =0.6 \cdot 0.8+0.1 \cdot 0.2=0.5 \\
P\left(W_{1}=\text { rain }\right) & =P\left(W_{1}=\operatorname{rain} \mid w_{0}\right) P\left(w_{0}\right) \\
& =P\left(W_{1}=\operatorname{rain} \mid W_{0}=\text { sun }\right) P\left(W_{0}=\text { sun }\right)+P\left(W_{1}=\text { rain } \mid W_{0}=\text { rain }\right) P\left(W_{0}=\text { rain }\right) \\
& =0.4 \cdot 0.8+0.9 \cdot 0.2=0.5
\end{aligned}
$$

因此我们的 $P\left(W_{1}\right)$ 分布为

| $W_{1}$ | $P\left(W_{1}\right)$ |
| :---: | :---: |
| sun | 0.5 |
| rain | 0.5 |

值得注意的是，晴朗天气的概率从时间 $t=0$ 的 $80 \%$ 降低到时间 $t=1$ 的 $50 \%$。这是我们的转移模型的直接结果，该模型倾向于转移到雨天而不是晴天。这引起了一个自然的后续问题：在给定时间步的状态的概率是否会收敛？我们将在接下来的部分中回答这个问题。

## 静止分布

为了解决上述问题，我们必须计算天气的静止分布。顾名思义，静止分布是在时间流逝后保持不变的分布，即

$$
P\left(W_{t+1}\right)=P\left(W_{t}\right)
$$

我们可以通过将上述等价性与迷你前向算法所使用的相同方程结合起来来计算给定状态的收敛概率：

$$
P\left(W_{t+1}\right)=P\left(W_{t}\right)=\sum_{w_{t}} P\left(W_{t+1} \mid w_{t}\right) P\left(w_{t}\right)
$$

对于我们的天气示例，这给了我们以下两个方程：

$$
\begin{aligned}
P\left(W_{t}=\text { sun }\right) & =P\left(W_{t+1}=\operatorname{sun} \mid W_{t}=\operatorname{sun}\right) P\left(W_{t}=\text { sun }\right)+P\left(W_{t+1}=\operatorname{sun} \mid W_{t}=\text { rain }\right) P\left(W_{t}=\text { rain }\right) \\
& =0.6 \cdot P\left(W_{t}=\operatorname{sun}\right)+0.1 \cdot P\left(W_{t}=\text { rain }\right) \\
P\left(W_{t}=\text { rain }\right) & =P\left(W_{t+1}=\text { rain } \mid W_{t}=\operatorname{sun}\right) P\left(W_{t}=\text { sun }\right)+P\left(W_{t+1}=\text { rain } \mid W_{t}=\text { rain }\right) P\left(W_{t}=\text { rain }\right) \\
& =0.4 \cdot P\left(W_{t}=\text { sun }\right)+0.9 \cdot P\left(W_{t}=\text { rain }\right)
\end{aligned}
$$

我们现在正好有了解决静止分布的所需的东西，即两个未知数的两个方程系统！我们可以通过使用 $P\left(W_{t}\right)$ 是一个概率分布的事实来得到第三个方程，因此必须加和为 1 ：

$$
\begin{aligned}
P\left(W_{t}=\text { sun }\right) & =0.6 \cdot P\left(W_{t}=\text { sun }\right)+0.1 \cdot P\left(W_{t}=\text { rain }\right) \\
P\left(W_{t}=\text { rain }\right) & =0.4 \cdot P\left(W_{t}=\text { sun }\right)+0.9 \cdot P\left(W_{t}=\text { rain }\right) \\
1 & =P\left(W_{t}=\text { sun }\right)+P\left(W_{t}=\text { rain }\right)
\end{aligned}
$$

求解这个方程组得到 $P\left(W_{t}=\text { sun }\right)=0.2$ 和 $P\left(W_{t}=\text { rain }\right)=0.8$。因此，我们静止分布的表格，我们今后将其表示为 $P\left(W_{\infty}\right)$，如下所示：

| $W_{\infty}$ | $P\left(W_{\infty}\right)$ |
| :---: | :---: |
| sun | 0.2 |
| rain | 0.8 |

为了验证这个结果，让我们将转移模型应用于静止分布：

$$
\begin{aligned}
P\left(W_{\infty+1}=\text { sun }\right) & =P\left(W_{\infty+1}=\operatorname{sun} \mid W_{\infty}=\operatorname{sun}\right) P\left(W_{\infty}=\text { sun }\right)+P\left(W_{\infty+1}=\operatorname{sun} \mid W_{\infty}=\text { rain }\right) P\left(W_{\infty}=\text { rain }\right) \\
& =0.6 \cdot 0.2+0.1 \cdot 0.8=0.2 \\
P\left(W_{\infty+1}=\text { rain }\right) & =P\left(W_{\infty+1}=\operatorname{rain} \mid W_{\infty}=\operatorname{sun}\right) P\left(W_{\infty}=\text { sun }\right)+P\left(W_{\infty+1}=\text { rain } \mid W_{\infty}=\text { rain }\right) P\left(W_{\infty}=\text { rain }\right) \\
& =0.4 \cdot 0.2+0.9 \cdot 0.8=0.8
\end{aligned}
$$

正如预期的那样，$P\left(W_{\infty+1}\right)=P\left(W_{\infty}\right)$。一般来说，如果 $W_{t}$ 的域的大小为 $k$，则等式

$$
P\left(W_{t}\right)=\sum_{w_{t}} P\left(W_{t+1} \mid w_{t}\right) P\left(w_{t}\right)
$$

产生了一个 $k$ 方程的系统，我们可以用它来求解静止分布。

## 隐马尔可夫模型

在马尔可夫模型中，我们看到了如何通过一系列随机变量来表示随时间变化的方式。例如，如果我们想知道标准马尔可夫模型中第 10 天的天气，我们可以从初始分布 $P\left(W_{0}\right)$ 开始，并使用我们的转移模型通过迷你前向算法计算 $P\left(W_{10}\right)$。然而，在时间 $t=0$ 和时间 $t=10$ 之间，我们可能会收集到新的气象证据，这可能会影响我们在任何给定时间步的天气概率分布的信念。简而言之，如果天气预报在第 10 天有 $80 \%$ 的几率下雨，但在第 9 天晚上天空晴朗，那么 $80 \%$ 的概率可能会大幅下降。这正是隐马尔可夫模型帮助我们解决的问题 - 它允许我们在每个时间步观察到一些证据，这可能会影响每个状态的信念分布。我们的天气模型的隐马尔可夫模型可以使用一个贝叶斯网结构来描述，看起来如下：

![](https://cdn.mathpix.com/cropped/2023_11_28_28f56ea6fe53f667bbb0g-04.jpg?height=309&width=1131&top_left_y=810&top_left_x=562)

与普通马尔可夫模型不同，我们现在有两种不同类型的节点。为了区分这一点，我们将每个 $W_{i}$ 称为状态变量，每个天气预报 $F_{i}$ 称为证据变量。由于 $W_{i}$ 编码了我们对第 $i$ 天天气概率分布的信念，因此第 $i$ 天的天气预报取决于这种信念是一个自然的结果。该模型暗示了与标准马尔可夫模型相似的条件独立关系，以及证据变量的额外一组关系：

$$
\begin{array}{ll}
F_{1} \perp W_{0} \mid W_{1} & \\
\forall i=2, \ldots, n ; & W_{i} \perp\left\{W_{0}, \ldots, W_{i-2}, F_{1}, \ldots, F_{i-1}\right\} \mid W_{i-1} \\
\forall i=2, \ldots, n ; & F_{i} \perp\left\{W_{0}, \ldots, W_{i-1}, F_{1}, \ldots, F_{i-1}\right\} \mid W_{i}
\end{array}
$$

就像马尔可夫模型一样，隐马尔可夫模型假设转移模型 $P\left(W_{i+1} \mid W_{i}\right)$ 是平稳的。隐马尔可夫模型还做出了额外的简化假设，即传感器模型 $P\left(F_{i} \mid W_{i}\right)$ 也是平稳的。因此，任何隐马尔可夫模型都可以用仅三个概率表格紧凑地表示：初始分布、转移模型和传感器模型。

关于符号的最后一点，我们将定义时间 $i$ 时的信念分布，并且到目前为止观察到了所有证据 $F_{1}, \ldots, F_{i}$：

$$
B\left(W_{i}\right)=P\left(W_{i} \mid f_{1}, \ldots, f_{i}\right)
$$

同样，我们将定义 $B^{\prime}\left(W_{i}\right)$ 为时间 $i$ 的信念分布，并且观察到了证据 $f_{1}, \ldots, f_{i-1}$：

$$
B^{\prime}\left(W_{i}\right)=P\left(W_{i} \mid f_{1}, \ldots, f_{i-1}\right)
$$

将在时间步 $i$ 观察到的证据定义为 $e_{i}$，你可能有时会看到从时间步 $1 \leq i \leq t$ 聚集的证据以以下形式重新表达：

$$
e_{1: t}=e_{1}, \ldots, e_{t}
$$

在这种符号下，$P\left(W_{i} \mid f_{1}, \ldots, f_{i-1}\right)$ 可以写作 $P\left(W_{i} \mid f_{1:(i-1)}\right)$。这种符号在接下来的部分中将变得相关，那时我们将讨论通过迭代地将新证据纳入我们的天气模型来进行时间流逝更新。

## 前向算法

使用上述条件概率假设和条件概率表的边缘化属性，我们可以推导出 $B\left(W_{i}\right)$ 和 $B^{\prime}\left(W_{i+1}\right)$ 之间的关系，其形式与迷你前向算法的更新规则相同。我们首先使用边缘化：

$$
B^{\prime}\left(W_{i+1}\right)=P\left(W_{i+1} \mid f_{1}, \ldots, f_{i}\right)=\sum_{w_{i}} P\left(W_{i+1}, w_{i} \mid f_{1}, \ldots, f_{i}\right)
$$

然后我们可以使用链式规则将其重新表达为：

$$
B^{\prime}\left(W_{i+1}\right)=P\left(W_{i+1} \mid f_{1}, \ldots, f_{i}\right)=\sum_{w_{i}} P\left(W_{i+1} \mid w_{i}, f_{1}, \ldots, f_{i}\right) P\left(w_{i} \mid f_{1}, \ldots, f_{i}\right)
$$

注意 $P\left(w_{i} \mid f_{1}, \ldots, f_{i}\right)$ 简单地是 $B\left(w_{i}\right)$，而且 $W_{i+1} \perp\left\{f_{1}, \ldots f_{i}\right\} \mid W_{i}$，这简化为我们最终的关系，即 $B\left(W_{i}\right)$ 到 $B^{\prime}\left(W_{i+1}\right)$：

$$
B^{\prime}\left(W_{i+1}\right)=\sum_{w_{i}} P\left(W_{i+1} \mid w_{i}\right) B\left(w_{i}\right)
$$

现在让我们考虑如何从 $B^{\prime}\left(W_{i+1}\right)$ 导出 $B\left(W_{i+1}\right)$ 的关系。通过简单地应用贝叶斯规则，我们可以看到

$$
B\left(W_{i+1}\right)=P\left(W_{i+1} \mid f_{1}, \ldots, f_{i+1}\right)=\frac{P\left(W_{i+1}, f_{i+1} \mid f_{1}, \ldots, f_{i}\right)}{P\left(f_{i+1} \mid f_{1}, \ldots, f_{i}\right)}
$$

处理条件概率时，一个常用的技巧是延迟归一化，直到我们需要归一化的概率，现在我们将使用这个技巧。更具体地说，由于上述展开中 $B\left(W_{i+1}\right)$ 的分母是 $B\left(W_{i+1}\right)$ 所代表的概率表中的每个项的公共项，我们可以省略实际除以 $P\left(f_{i+1} \mid f_{1}, \ldots, f_{i}\right)$。相反，我们可以简单地注意到 $B\left(W_{i+1}\right)$ 与 $P\left(W_{i+1}, f_{i+1} \mid f_{1}, \ldots, f_{i}\right)$ 成正比：

$$
B\left(W_{i+1}\right) \propto P\left(W_{i+1}, f_{i+1} \mid f_{1}, \ldots, f_{i}\right)
$$

其比例常数等于 $P\left(f_{i+1} \mid f_{1}, \ldots, f_{i}\right)$。当我们决定我们想要恢复信念分布 $B\left(W_{i+1}\right)$ 时，我们可以通过这个常数将每个计算出的值除以。现在，使用链式规则我们可以观察到以下情况：

$$
B\left(W_{i+1}\right) \propto P\left(W_{i+1}, f_{i+1} \mid f_{1}, \ldots, f_{i}\right)=P\left(f_{i+1} \mid W_{i+1}, f_{1}, \ldots, f_{i}\right) P\left(W_{i+1} \mid f_{1}, \ldots, f_{i}\right)
$$

根据之前所述的隐马尔可夫模型的条件独立假设，$P\left(f_{i+1} \mid W_{i+1}, f_{1}, \ldots, f_{i}\right)$ 等价于简单的 $P\left(f_{i+1} \mid W_{i+1}\right)$，而 $P\left(W_{i+1} \mid f_{1}, \ldots, f_{i}\right)$ 根据定义是 $B^{\prime}\left(W_{i+1}\right)$。这使我们能够以其最终形式表达 $B^{\prime}\left(W_{i+1}\right)$ 和 $B\left(W_{i+1}\right)$ 之间的关系：

$$
B\left(W_{i+1}\right) \propto P\left(f_{i+1} \mid W_{i+1}\right) B^{\prime}\left(W_{i+1}\right)
$$

结合我们刚刚推导出的两个关系，就得到了一个称为前向算法的迭代算法，这是隐马尔可夫模型中早期迷你前向算法的类似物：

$$
B\left(W_{i+1}\right) \propto P\left(f_{i+1} \mid W_{i+1}\right) \sum_{w_{i}} P\left(W_{i+1} \mid w_{i}\right) B\left(w_{i}\right)
$$

前向算法可以被认为包含两个独特的步骤：时间流逝更新，对应于从 $B\left(W_{i}\right)$ 确定 $B^{\prime}\left(W_{i+1}\right)$；观察更新，对应于从 $B^{\prime}\left(W_{i+1}\right)$ 确定 $B\left(W_{i+1}\right)$。因此，为了将我们的信念分布前进一个时间步（即从 $B\left(W_{i}\right)$ 计算 $B\left(W_{i+1}\right)$），我们必须首先用时间流逝更新前进模型的状态一个时间步，然后用观察更新纳入该时间步的新证据。考虑以下初始分布、转移模型和传感器模型：

| $W_{0}$ | $B\left(W_{0}\right)$ |
| :---: | :---: |
| sun | 0.8 |
| rain | 0.2 |


| $W_{i+1}$ | $W_{i}$ | $P\left(W_{i+1} \mid W_{i}\right)$ |
| :---: | :---: | :---: |
| sun | sun | 0.6 |
| rain | sun | 0.4 |
| sun | rain | 0.1 |
| rain | rain | 0.9 |


| $F_{i}$ | $W_{i}$ | $P\left(F_{i} \mid W_{i}\right)$ |
| :---: | :---: | :---: |
| good | sun | 0.8 |
| bad | sun | 0.2 |
| good | rain | 0.3 |
| bad | rain | 0.7 |

为了计算 $B\left(W_{1}\right)$，我们首先执行时间更新以获得 $B^{\prime}\left(W_{1}\right)$：

$$
\begin{aligned}
B^{\prime}\left(W_{1}=\text { sun }\right) & =\sum_{w_{0}} P\left(W_{1}=\operatorname{sun} \mid w_{0}\right) B\left(w_{0}\right) \\
& =P\left(W_{1}=\operatorname{sun} \mid W_{0}=\operatorname{sun}\right) B\left(W_{0}=\operatorname{sun}\right)+P\left(W_{1}=\operatorname{sun} \mid W_{0}=\operatorname{rain}\right) B\left(W_{0}=\text { rain }\right) \\
& =0.6 \cdot 0.8+0.1 \cdot 0.2=0.5 \\
B^{\prime}\left(W_{1}=\text { rain }\right) & =\sum_{w_{0}} P\left(W_{1}=\operatorname{rain} \mid w_{0}\right) B\left(w_{0}\right) \\
& =P\left(W_{1}=\operatorname{rain} \mid W_{0}=\operatorname{sun}\right) B\left(W_{0}=\operatorname{sun}\right)+P\left(W_{1}=\text { rain } \mid W_{0}=\text { rain }\right) B\left(W_{0}=\text { rain }\right) \\
& =0.4 \cdot 0.8+0.9 \cdot 0.2=0.5
\end{aligned}
$$

因此：

| $W_{1}$ | $B^{\prime}\left(W_{1}\right)$ |
| :---: | :---: |
| sun | 0.5 |
| rain | 0.5 |

接下来，我们假设第 1 天的天气预报是好的（即 $F_{1}=\text{good}$），并进行观察更新以获得 $B\left(W_{1}\right)$：

$$
\begin{aligned}
B\left(W_{1}=\text { sun }\right) & \propto P\left(F_{1}=\text { good } \mid W_{1}=\text { sun }\right) B^{\prime}\left(W_{1}=\text { sun }\right)=0.8 \cdot 0.5=0.4 \\
B\left(W_{1}=\text { rain }\right) & \propto P\left(F_{1}=\text { good } \mid W_{1}=\text { rain }\right) B^{\prime}\left(W_{1}=\text { rain }\right)=0.3 \cdot 0.5=0.15
\end{aligned}
$$

最后一步是对 $B\left(W_{1}\right)$ 进行归一化，注意 $B\left(W_{1}\right)$ 表中的项加起来为 $0.4+0.15=0.55$：

$$
\begin{aligned}
B\left(W_{1}=\text { sun }\right) & =0.4 / 0.55=\frac{8}{11} \\
B\left(W_{1}=\text { rain }\right) & =0.15 / 0.55=\frac{3}{11}
\end{aligned}
$$

因此，我们的最终 $B\left(W_{1}\right)$ 表如下：

| $W_{1}$ | $B^{\prime}\left(W_{1}\right)$ |
| :---: | :---: |
| sun | $8 / 11$ |
| rain | $3 / 11$ |

注意观察天气预报的结果。因为天气预报预测天气晴朗，我们相信晴天的可能性从时间更新后的 $\frac{1}{2}$ 增加到观察更新后的 $\frac{8}{11}$。

作为最后一点说明，上面讨论的归一化技巧实际上可以在使用隐马尔可夫模型时显著简化计算。如果我们从某个初始分布开始，并且对在时间 $t$ 的信念分布感兴趣，我们可以使用前向算法迭代地计算 $B\left(W_{1}\right), \ldots, B\left(W_{t}\right)$，并且只在最后通过将 $B\left(W_{t}\right)$ 表中的每个项除以其项的总和来进行一次归一化。

## 维特比算法

在前向算法中，我们使用递归来求解 $P\left(X_{N} \mid e_{1: N}\right)$，即系统在迄今为止观察到的证据变量下可能处于的状态的概率分布。另一个与隐马尔可夫模型相关的重要问题是：鉴于到目前为止观察到的证据变量，系统最有可能遵循的隐藏状态序列是什么？换句话说，我们想要求解 $\arg \max _{x_{1: N}} P\left(x_{1: N} \mid e_{1: N}\right)=\arg \max _{x_{1: N}} P\left(x_{1: N}, e_{1: N}\right)$。这个轨迹也可以使用动态规划和维特比算法来求解。

该算法包括两个步骤：第一个向前进行，并计算到目前为止观察到的证据下每个（状态，时间）元组的最佳路径的概率。第二个步骤向后进行：首先找到位于最高概率路径上的终止状态，然后沿着通往这个状态的路径（必然是最佳路径）向后穿越时间。

为了可视化算法，考虑以下状态格栅，即随时间变化的状态和转换的图：

![](https://cdn.mathpix.com/cropped/2023_11_28_28f56ea6fe53f667bbb0g-07.jpg?height=382&width=1076&top_left_y=931&top_left_x=579)

在这个有两个可能隐藏状态的隐马尔可夫模型中，即晴天或雨天，我们想要计算从 $X_{1}$ 到 $X_{N}$ 的最高概率路径（每个时间步的状态分配）。从 $X_{t-1}$ 到 $X_{t}$ 的边的权重等于 $P\left(X_{t} \mid X_{t-1}\right) P\left(E_{t} \mid X_{t}\right)$，路径的概率通过取其边权重的乘积来计算。权重公式中的第一项表示特定转换的可能性有多大，第二项表示观察到的证据与结果状态的匹配程度有多好。

回想一下：

$$
P\left(X_{1: N}, e_{1: N}\right)=P\left(X_{1}\right) P\left(e_{1} \mid X_{1}\right) \prod_{t=2}^{N} P\left(X_{t} \mid X_{t-1}\right) P\left(e_{t} \mid X_{t}\right)
$$

前向算法计算（在归一化之前）

$$
P\left(X_{N}, e_{1: N}\right)=\sum_{x_{1}, ., x_{N-1}} P\left(X_{N}, x_{1: N-1}, e_{1: N}\right)
$$

在维特比算法中，我们想要计算

$$
\arg \max _{x_{1}, \ldots, x_{N}} P\left(x_{1: N}, e_{1: N}\right)
$$

以找到隐藏状态序列的最大似然估计。注意，乘积中

的每一项正好是从层 $t-1$ 到层 $t$ 之间的边权重的表达式。因此，格栅上路径的权重乘积给出了给定证据下路径的概率。

我们可以为所有可能的隐藏状态求解一个联合概率表，但这会导致指数级的空间成本。有了这样的表，我们可以使用动态规划在多项式时间内计算最佳路径。然而，由于我们可以使用动态规划来计算最佳路径，我们并不一定需要在任何给定时间拥有整个表。

定义 $m_{t}\left[x_{t}\right]=\max _{x_{1: t-1}} P\left(x_{1: t}, e_{1: t}\right)$，或者是从任何 $x_{0}$ 开始并到目前为止所见证据到给定 $x_{t}$ 的时间 $t$ 的最大概率路径。这与从步骤 1 到 $t$ 通过格栅的最高权重路径相同。还要注意

$$
\begin{aligned}
m_{t}\left[x_{t}\right] & =\max _{x_{1: t-1}} P\left(e_{t} \mid x_{t}\right) P\left(x_{t} \mid x_{t-1}\right) P\left(x_{1: t-1}, e_{1: t-1}\right) \\
& =P\left(e_{t} \mid x_{t}\right) \max _{x_{t-1}} P\left(x_{t} \mid x_{t-1}\right) \max _{x_{1: t-2}} P\left(x_{1: t-1}, e_{1: t-1}\right) \\
& =P\left(e_{t} \mid x_{t}\right) \max _{x_{t-1}} P\left(x_{t} \mid x_{t-1}\right) m_{t-1}\left[x_{t-1}\right]
\end{aligned}
$$

这表明我们可以通过动态规划递归地计算所有 $m_{t}$。这使我们能够确定最有可能路径的最后状态 $x_{N}$，但我们仍然需要一种方法来回溯以重构整个路径。让我们定义 $a_{t}\left[x_{t}\right]=P\left(e_{t} \mid x_{t}\right) \arg \max _{x_{t-1}} P\left(x_{t} \mid x_{t-1}\right) m_{t-1}\left[x_{t-1}\right]=\arg \max _{x_{t-1}} P\left(x_{t} \mid x_{t-1}\right) m_{t-1}\left[x_{t-1}\right]$ 来跟踪通往 $x_{t}$ 的最佳路径的最后一次转换。我们现在可以概述算法。

结果：隐藏状态 $x_{1: N}^{*}$ 的最有可能序列

![](https://cdn.mathpix.com/cropped/2023_11_28_28f56ea6fe53f667bbb0g-08.jpg?height=857&width=1590&top_left_y=1007&top_left_x=296)

注意我们的 $a$ 数组定义了一组 $N$ 个序列，每个序列是到特定结束状态 $x_{N}$ 的最有可能序列。一旦我们完成前向传递，我们查看 $N$ 个序列的可能性，选择最好的一个，并在后向传递中重构它。因此，我们已经在多项式空间和时间内计算了我们的证据的最有可能的解释。

## 粒子滤波

回想一下，当使用贝叶斯网进行精确推理计算成本过高时，使用我们讨论过的其中一种抽样技术是有效近似我们想要的概率分布的可行替代方案。隐马尔可夫模型也有同样的缺点- 使用前向算法进行精确推理所需的时间随着随机变量域中值的数量而增长。在我们当前的天气问题表述中，这是可以接受的，因为天气只能取两个值，$W_{i} \in\{\text{sun}, \text{rain}\}$，但假设我们想要进行推理，计算某一天实际温度到十分之一度的分布。

隐马尔可夫模型中贝叶斯网抽样的类似物称为粒子滤波，它涉及通过状态图模拟一组粒子的运动来近似所讨论的随机变量的概率（信念）分布。这解决了与前向算法相同的问题：它为我们提供了 $P\left(X_{N} \mid e_{1: N}\right)$ 的近似。

我们不再存储将每个状态映射到其信念概率的完整概率表，而是存储一个包含 $n$ 个粒子的列表，其中每个粒子处于我们时间相关随机变量的域中的 $d$ 个可能状态之一。通常，$n$ 显著小于 $d$（符号表示为 $n<<d$），但仍足够大以产生有意义的近似；否则，粒子滤波的性能优势将变得可以忽略。粒子只是这个算法中样本的名称。

我们认为粒子在任何给定时间步的任何给定状态中的信念完全取决于我们模拟中该状态在该时间步的粒子数量。例如，假设我们确实想要模拟某一天 $i$ 温度 $T$ 的信念分布，并为了简单起见，假设这个温度只能在范围 $[10,20]$ 内取整数值（$d=11$ 个可能状态）。进一步假设我们有 $n=10$ 个粒子，在我们模拟的时间步 $i$ 中取以下值：

$$
[15,12,12,10,18,14,12,11,11,10]
$$

通过计算粒子列表中出现的每个温度的计数并除以粒子总数，我们可以生成我们在时间 $i$ 所需的经验分布：

| $T_{i}$ | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| $B\left(T_{i}\right)$ | 0.2 | 0.2 | 0.3 | 0 | 0.1 | 0.1 | 0 | 0 | 0.1 | 0 | 0 |

现在我们已经看到了如何从粒子列表中恢复信念分布，剩下要讨论的就是如何为我们选择的时间步生成这样的列表。

## 粒子滤波模拟

粒子滤波模拟从粒子初始化开始，这可以非常灵活地完成 - 我们可以随机、均匀地抽样粒子，或者从某个初始分布中抽样。一旦我们抽样出了一个初始粒子列表，模拟就会采取与前向算法类似的形式，在每个时间步进行时间流逝更新和观察更新：

- 时间流逝更新 - 根据转移模型更新每个粒子的值。对于处于状态 $t_{i}$ 的粒子，从由 $P\left(T_{i+1} \mid t_{i}\right)$ 给出的概率分布中抽样更新值。注意时间流逝更新与贝叶斯网的先验抽样的相似之处，因为粒子在任何给定状态的频率反映了转移概率。
- 观察更新 - 在粒子滤波的观察更新期间，我们使用传感器模型 $P\left(F_{i} \mid T_{i}\right)$ 根据观察到的证据和粒子的状态对每个粒子进行加权。具体来说，对于处于状态 $t_{i}$ 的粒子和传感器读数 $f_{i}$，分配一个权重 $P\left(f_{i} \mid t_{i}\right)$。观察更新的算法如下：

1. 如上所述，计算所有粒子的权重。
2. 计算每个状态的总权重。
3. 如果所有状态的权重总和为 0，则重新初始化所有粒子。
4. 否则，对状态上的总权重分布进行归一化，并从该分布中重新抽样粒子列表。

注意观察更新的相似之处与似然加权，我们再次根据我们的证据对样本进行加权。

让我们通过一个例子更好地理解这个过程。定义我们的天气场景中使用温度作为时间相关随机变量的转移模型：对于特定的温度状态，你可以保持在同一状态或转移到相差一度的状态，范围在 $[10,20]$ 内。在可能的结果状态中，转移到最接近 15 的状态的概率为 $80 \%$，其余的结果状态均匀地分摊剩余的 $20 \%$ 概率。

我们的温度粒子列表如下：

$$
[15,12,12,10,18,14,12,11,11,10]
$$

为了对这个粒子列表中的第一个粒子进行时间流逝更新，它处于状态 $T_{i}=15$，我们需要相应的转移模型：

| $T_{i+1}$ | 14 | 15 | 16 |
| :---: | :---: | :---: | :---: |
| $P\left(T_{i+1} \mid T_{i}=15\right)$ | 0.1 | 0.8 | 0.1 |

实际操作中，我们为 $T_{i+1}$ 域中的每个值分配一个不同的值范围，以便这些范围共同覆盖区间 $[0,1)$，且不重叠。对于上述转移模型，范围如下：

1. $T_{i+1}=14$ 的范围是 $0 \leq r<0.1$。
2. $T_{i+1}=15$ 的范围是 $0.1 \leq r<0.9$。
3. $T_{i+1}=16$ 的范围是 $0.9 \leq r<1$。

为了重新抽样状态为 $T_{i}=15$ 的粒子，我们只需在区间 $[0,1)$ 中生成一个随机数，并查看它落在哪个范围内。因此，如果我们的随机数是 $r=0.467$，那么粒子在 $T_{i}=15$ 将保持在 $T_{i+1}=15$，因为 $0.1 \leq r<0.9$。现在考虑以下区间 $[0,1)$ 中的 10 个随机数：

$$
[0.467,0.452,0.583,0.604,0.748,0.932,0.609,0.372,0.402,0.026]
$$

如果我们将这 10 个值作为重新抽样我们 10 个粒子的随机值，我们的新粒子列表在完整的时间流逝更新后应该看起来像这样：

$$
[15,13,13,11,17,15,13,12,12,10]
$$

验证一下这个过程！更新后的粒子列表产生了相应更新后的信念分布 $B\left(T_{i+1}\right)$：

| $T_{i}$ | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| $B\left(T_{i+1}\right)$ | 0.1 | 0.1 | 0.2 | 0.3 | 0 | 0.2 | 0 | 0.1 | 0 | 0 | 0 |

对比我们更新前和更新后的信念分布 $B\left(T_{i+1}\right)$，我们可以看到作为一般趋势，粒子趋向于聚集在 $T=15$ 的温度。

接下来，让我们进行观察更新，假设我们的传感器模型 $P\left(F_{i} \mid T_{i}\right)$ 表明正确预报 $f_{i}=t_{i}$ 的概率是 $80 \%$，预报其他 10 个状态中的任何一个的概率均匀为 $2 \%$。假设预报 $F_{i+1}=13$，我们 10 个粒子的权重如下：

| 粒子 | $p_{1}$ | $p_{2}$ | $p_{3}$ | $p_{4}$ | $p_{5}$ | $p_{6}$ | $p_{7}$ | $p_{8}$ | $p_{9}$ | $p_{10}$ |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 状态 | 15 | 13 | 13 | 11 | 17 | 15 | 13 | 12 | 12 | 10 |
| 权重 | 0.02 | 0.8 | 0.8 | 0.02 | 0.02 | 0.02 | 0.8 | 0.02 | 0.02 | 0.02 |

然后我们按状态汇总权重：

| 状态 | 10 | 11 | 12 | 13 | 15 | 17 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 权重 | 0.02 | 0.02 | 0.04 | 2.4 | 0.04 | 0.02 |

所有状态的权重总和加起来为 2.54，我们可以通过将每个条目除以这个总和来对权重表进行归一化，生成一个概率分布：

| 状态 | 10 | 11 | 12 | 13 | 15 | 17 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 权重 | 0.02 | 0.02 | 0.04 | 2.4 | 0.04 | 0.02 |
| 归一化权重 | 0.0079 | 0.0079 | 0.0157 | 0.9449 | 0.0157 | 0.0079 |

最后一步是从这个概率分布中重新抽样，使用我们在时间流逝更新期间用来重新抽样的相同技术。假设我们生成了区间 $[0,1)$ 中的 10 个随机数，其值如下：

$$
[0.315,0.829,0.304,0.368,0.459,0.891,0.282,0.980,0.898,0.341]
$$

这将产生以下重新抽样的粒子列表：

$$
[13,13,13,13,13,13,13,15,13,13]
$$

对应的最终新信念分布为：

| $T_{i}$ | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| $B\left(T_{i+1}\right)$ | 0 | 0 | 0 | 0.9 | 0 | 0.1 | 0 | 0 | 0 | 0 | 0 |

注意我们的传感器模型编码了我们的天气预报具有 $80 \%$ 的准确率，而且我们新的粒子列表与此一致，因为大多数粒子被重新抽样为 $T_{i+1}=13$。

## 总结

在这篇笔记中，我们介绍了两种新的模型：

- 马尔可夫模型，编码具有马尔可夫性质的时间依赖随机变量。我们可以使用概率推理和迷你前向算法计算马尔可夫模型在我们选择的任何时间步的信念分布。
- 隐马尔可夫模型，是具有额外属性的马尔可夫模型，即在每个时间步可以观察到可能影响我们的信念分布的新证据。要计算隐马尔可夫模型在任何给定时间步的信念分布，我们使用前向算法。

有时，在这些模型上运行精确推理可能计算成本过高，在这种情况下，我们可以使用粒子滤波作为近似推理的方法。