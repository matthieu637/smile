close all
clear all

Z = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/correct120/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData/slowS/slowS/advise120/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

print(h, 'lisa_slowS.png');