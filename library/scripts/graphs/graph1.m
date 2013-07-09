clear all
close all



X = std( load( '../../data/mcar_qltable_learner' ) );

q=0.001:0.001:0.006

ii=1
for i=q
   tmp = std( load( cstrcat('../../data/mcar_qltable_tearner_', num2str(i))) ); 
   Y(ii)=tmp(1)
   ii=ii+1;
end

h = figure

x=1:ii-1


plot(q, ones(1, length(x))*X , '-'); hold on;
plot(q, Y, 'r-')

legend('no advise', 'advise after');
xlabel('cout');
ylabel('temps moyen d apprentissage');
title('Evolution du temps d apprentissage par rapport au cout ');

print(h, '../../data/graphs/graph1.png')

