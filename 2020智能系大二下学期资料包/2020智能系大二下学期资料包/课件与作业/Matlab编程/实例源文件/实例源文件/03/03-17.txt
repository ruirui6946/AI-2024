clear
clc
A = [2 5 7 1 3 4];
odds = 1:2:length(A);

disp('odd values of A using predefined indices')
A(odds)
disp('odd values of A using anonymous indices')
A(1:2:end)
disp('put evens into odd values in a new array')
B(odds) = A(2:2:end)
disp('set the even values in B to 99')
B(2:2:end) = 99
disp('find the small values in A')
small = A < 4
disp('add 10 to the small values')
A(small) = A(small) + 10
disp('this can be done in one ugly operation')
A(A < 4) = A(A < 4) + 10
