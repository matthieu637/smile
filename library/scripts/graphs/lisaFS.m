close all
clear all

Z = load('/home/matthieu/workspace/java/mountaincar/myData/fastS/fastS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData/fastS/fastS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData/fastS/fastS/correct100/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData/fastS/fastS/advise100/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 100 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

print(h, 'lisa_fastS.png');