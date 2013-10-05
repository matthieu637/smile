close all
clear all

Z = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData/slowQ/slowQ/independent/avg_curve');
%  V = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/correct170/avg_curve');
%  X = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/advise170/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
%  plot(V(:,2), 'b', "linewidth", 2);hold on;
%  plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('sarsa', 'qlearning');

print(h, 'lisa_qs.png');