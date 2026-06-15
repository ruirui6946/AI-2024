clear all;
close all;

% 设置学习率、惯性项和网络结构参数
xite = 0.01;% 冲量
alfa = 0.05;% 学习率
IN = 4;
H1 = 5;
H2 = 5;
Out = 3;

upper_bound = 0.5;
lower_bound = -0.5;

% 初始化输入到第一个隐藏层的权重矩阵
% wi1 = rand(H1, IN) * (upper_bound - lower_bound) + lower_bound;
wi1 = [
0.224521749742638	0.135594038608965	-0.495374905608729	0.370468638276788
-0.258224317398773	0.208533864549768	-0.410336809224729	0.124555283628723
0.385945000411898	-0.137524480163592	0.473058602432665	0.624246356204780
-0.447454200040710	0.465275008730917	-0.405065119763742	-0.392652795823681
0.0351721811517669	0.409553337425397	-0.0803324923161282	-0.375491804395403
];
wi1_1 = wi1; wi1_2 = wi1; wi1_3 = wi1;

% 初始化第一个隐藏层到第二个隐藏层的权重矩阵
% wi2 = rand(H2, H1) * (upper_bound - lower_bound) + lower_bound;
wi2 = [
0.631514977480097	0.672016408108704	-0.536551088984277	0.705176693911479	0.379235619989735
0.333959443821817	0.886744696061373	-0.711445786607146	0.271839754584109	0.518505808824062
0.642540435060150	0.378662863292834	-0.683195576522955	1.10915289866026	0.301249182419331
-0.230674713754168	-0.502382511830043	0.113896955089837	-0.673460690088704	-1.07024971749569
-1.29795170142967	-0.818768274183648	1.69291875204005	-1.53117130552289	-1.20084542399962
];
wi2_1 = wi2; wi2_2 = wi2; wi2_3 = wi2;

% 初始化第二个隐藏层到输出层的权重矩阵
% wo = rand(Out, H2) * (upper_bound - lower_bound) + lower_bound;
wo = [
1.55284900069045	2.12414592641918	2.21940329986005	1.88136612813388	1.96797247659794
1.62775433269451	1.61374993496040	1.47187162512370	2.18475673574565	2.00950920349742
1.99065894211349	2.22757985361920	2.03407910736411	1.86837150950284	1.65093652507741
];
wo_1 = wo; wo_2 = wo; wo_3 = wo;

% 初始化变量
x = [0, 0, 0];
u_1 = 0; u_2 = 0; u_3 = 0; u_4 = 0; u_5 = 0;
y_1 = 0; y_2 = 0; y_3 = 0;

Oh1 = zeros(H1, 1);
Oh2 = zeros(H2, 1);

error_2 = 0;
error_1 = 0;

ts = 0.01;% time step
for k = 1:1:600
    time(k) = k * ts;
%     rin(k) = 1;% reference input
    % 参考输入（这里使用的是步进输入）
    % 如果需要使用正弦输入，取消下一行的注释
    rin(k) = sin(2 * pi * time(k));

    a(k) = 1.2 * (1 - 0.8 * exp(-0.1 * k));
    yout(k) = a(k) * y_1 / (1 + y_1^2) + u_1;% y_out实际输出
    error(k) = rin(k) - yout(k);% 参考输入（希望的输出）与实际输出的的差
    xi = [rin(k), yout(k), error(k), 1];% 神经网络输入

    % 计算误差历史
    x(1) = error(k) - error_1;
    x(2) = error(k);
    x(3) = error(k) - 2 * error_1 + error_2;
    epid = [x(1); x(2); x(3)];

    % 修改中间层的计算（前向传播）
    I1 = wi1 * xi';
    for j = 1:1:H1    
        Oh1(j) = (exp(I1(j)) - exp(-I1(j))) / (exp(I1(j)) + exp(-I1(j))); 
    end
    
    I2 = wi2 * Oh1;
    for j = 1:1:H2    
        Oh2(j) = (exp(I2(j)) - exp(-I2(j))) / (exp(I2(j)) + exp(-I2(j))); 
    end
    
    % 修改输出层的计算（前向传播）
    K = wo * Oh2;
    for l = 1:1:Out
        K(l) = exp(K(l)) / (exp(K(l)) + exp(-K(l)));
    end

    % 获取PID参数
    kp(k) = K(1); ki(k) = K(2); kd(k) = K(3);
    Kpid = [kp(k), ki(k), kd(k)];

    % 计算控制器输出增量
    du(k) = Kpid * epid;
    u(k) = u_1 + du(k);

    % 限制控制器输出
    if u(k) >= 10
        u(k) = 10;
    end
    if u(k) <= -10
        u(k) = -10;
    end

    % 开始反向传播
    % 计算误差灵敏度
    dyu(k) = sign((yout(k) - y_1) / (u(k) - u_1 + 0.0000001));

    % 计算输出层的误差项
    for j = 1:1:Out
        dK(j) = 2 / (exp(K(j)) + exp(-K(j)))^2;
    end

    for l = 1:1:Out
        delta3(l) = error(k) * dyu(k) * epid(l) * dK(l);
    end

    % 更新输出层权重
    for l = 1:1:Out
        for i = 1:1:H2
            d_wo = xite * delta3(l) * Oh2(i) + alfa * (wo_1 - wo_2);
        end
    end
    wo = wo_1 + d_wo + alfa * (wo_1 - wo_2);

    % 修改第二个隐藏层的误差项计算（反向传播）
    for i = 1:1:H2
        dO2(i) = 4 / (exp(I2(i)) + exp(-I2(i)))^2;% 第二隐藏层的激活层的导数
    end
    
    segma2 = delta3 * wo;% 从第三层传回第二层的误差
    for i = 1:1:H2
        delta2(i) = dO2(i) * segma2(i);% 第二层的更新参考
    end
    
    % 修改第一个隐藏层的误差项计算（反向传播）
    for i = 1:1:H1
        dO1(i) = 4 / (exp(I1(i)) + exp(-I1(i)))^2;
    end
    
    segma1 = delta2 * wi2;
    for i = 1:1:H1
        delta1(i) = dO1(i) * segma1(i);
    end
    
    % 修改权重更新（反向传播）
    d_wi2 = xite * delta2' * Oh1' + alfa * (wi2_1 - wi2_2);% 更新量
    wi2 = wi2_1 + d_wi2;
    
    d_wi1 = xite * delta1' * xi + alfa * (wi1_1 - wi1_2);
    wi1 = wi1_1 + d_wi1;

    % 更新历史参数
    u_5 = u_4; u_4 = u_3; u_3 = u_2; u_2 = u_1; u_1 = u(k);
    y_2 = y_1; y_1 = yout(k);
    wo_3 = wo_2; wo_2 = wo_1; wo_1 = wo;
    wi1_3 = wi1_2; wi1_2 = wi1_1; wi1_1 = wi1;
    wi2_3 = wi2_2; wi2_2 = wi2_1; wi2_1 = wi2;
    error_2 = error_1;
    error_1 = error(k);
end

% 绘图
figure(1);
plot(time, rin, 'r', time, yout, 'b');
xlabel('time(s)'); ylabel('rin,yout');

figure(2);
plot(time, error, 'r');
xlabel('time(s)'); ylabel('error');

figure(3);
plot(time, u, 'r');
xlabel('time(s)'); ylabel('u');

figure(4);
subplot(311);
plot(time, kp, 'r');
xlabel('time(s)'); ylabel('kp');

subplot(312);
plot(time, ki, 'g');
xlabel('time(s)'); ylabel('ki');

subplot(313);
plot(time, kd, 'b');
xlabel('time(s)'); ylabel('kd');
