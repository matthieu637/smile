clear all
close all

X = load('../../data/TGQ1Q1AMTA');
Y = load('../../data/TGQ1Q1AOTA');
Z = load('../../data/TGQ1Q1ANTA');
U = load('../../data/TGQ1Q1AITA');

X = mean(X);
Y = mean(Y);
Z = mean(Z);
U = mean(U);

inter=1:15:length(X);

h = figure

plot(inter, X(inter), '-', "linewidth", 2); hold on;
plot(inter, Y(inter), 'r-', "linewidth", 2); hold on;
plot(inter, Z(inter), 'g-', "linewidth", 2); hold on;
plot(inter, U(inter), 'm-', "linewidth", 2);

legend('Max', 'Override', 'None', 'Max+Override');
xlabel('step of teacher');
ylabel('step to reach the goal for the learner at the first episod');
title('How the advice affects the learner');

print(h, '../../data/graphs/graphA_GA.png')
