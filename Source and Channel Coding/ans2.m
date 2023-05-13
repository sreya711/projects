% (a) n = 15, k = 10, p = 0.015
% (b) n = 15, k = 10, p = 0.1
% (c) n = 15, k = 10, p = 0.45
% (d) n = 20, k = 10, p = 0.015
% (e) n = 20, k = 10, p = 0.1
% (f) n = 20, k = 10, p = 0.45

clear;
n1=[15 15 15 20 20 20];
k1=[10 10 10 10 10 10];
p1=[0.015 0.1 0.45 0.015 0.1 0.45];
for g=1:6
n=n1(g);
k=k1(g);
p=p1(g);

c=zeros(5,2^k);                 % initialising the code arry of 5 code of same n,k,p
E=zeros(1,5);
N=1000;

for j=1:5
    a=0;
    c(j,:)=abs(floor(2^n.*rand(1,2^k)));
%     code=zeros(2^k,n);
    temp=c(j,:);
    temp=temp';
    code=decimalToBinaryVector(temp,n);
%     for z=1:2^k
%         code(z)=decimalToBinaryVector(c(j,z),n);
%     end

% c1=unique(c(j,:));

    for z=1:N
        num_ind=abs(floor(rand()*2^k))+1;
        num=c(j,num_ind);
        bits=decimalToBinaryVector(num,n);
        bits1=bits;
        for x=1:n
            num=rand();                 % simulating BSC
            if num<p
               if bits(x)==0
                   bits(x)=1;
               else 
                   bits(x)=0;
               end
            end
        end
        dist=zeros(1,2^k);
        for x=1:2^k
            for y=1:n                       % implimenting the MDD
                dist(x)=dist(x)+abs(bits(y)-code(x,y));
            end
        end
        [min_val,min_ind]=min(dist);
        bits2=code(min_ind);
        if min_val>0
            E(j)=E(j)+1;
        end
    end
    E(j)=E(j)/N;
end
Pe(g)=min(E);
end

figure(1);
plot(Pe);
        
        
