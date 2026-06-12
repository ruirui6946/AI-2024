prompt={'输入姓名','输入年龄','输入职业'};
title='信息登记';                                %% 标题
lines=[1 1 1]';                           %%  输入框的行数，每个输入框都是一行
def={'张平','3','教师'};
answer=inputdlg(prompt,title,lines,def);
