close all
clear all

Z = load('/home/matthieu/workspace/java/mountaincar/myDataTest/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myDataTest/slowS/slowS/baseline/avg_curve');
%  V = load('/home/matthieu/workspace/java/mountaincar/myDataTest/slowS/slowS/correct140/avg_curve');
%  X = load('/home/matthieu/workspace/java/mountaincar/myDataTest/slowS/slowS/advise40/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
%  plot(V(:,2), 'b', "linewidth", 2);hold on;
%  plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

%  print(h, 'lisa_slowS.png');

%  k=1
%  n=230:-5:5
%  QQ=zeros(size(n,2), 2);
%  for i=n
%      q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myDataTest/slowS/slowS/advise',
%  		num2str(i),'/avg_curve'));
%  	QQ(k,1) = mean(q(:,2));
%  	QQ(k,2) = i;
%  	k = k +1;
%  endfor
%  
%  [m,i] = max(QQ);
%  m
%  QQ(i(1), 2)
%  QQ;
%  
%  figure
%  
%  plot(QQ(:,2),QQ(:,1))
