function [Fib,fibnum1,fibnum2]=fibMain(m,n)
Fib=fibNum(n);
    function Fib=fibNum(n)
        if n==1
            Fib=[1];
        elseif n==2
            Fib=[1 1];
        else
            Fib(1)=1;
            Fib(2)=1;
            for k=3:n
                Fib(k)=Fib(k-1)+Fib(k-2);
            end
        end        
    end

fibnum1=fibNumWhile(m);
    function a=fibNumWhile(m)
        if m<1
            a=1;
        else
            a=0;
            a1=1;
            a2=1;
            while a<=m
                a=a1+a2;
                a1=a2;
                a2=a;
            end
        end
    end
fibnum2=1;
end