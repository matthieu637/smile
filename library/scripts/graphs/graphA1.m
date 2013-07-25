clear all
close all

X = load('../../data/TMQ1Q1AMTA');
Y = load('../../data/TMQ1Q1AOTA');
Z = load('../../data/TMQ1Q1ANTA');
U = load('../../data/TMQ1Q1AITA');

X = mean(X);
Y = mean(Y);
Z = mean(Z);
U = mean(U);

inter=1:15:length(X);

h = figure

plot(inter, X(inter), '-'); hold on;
plot(inter, Y(inter), 'r-'); hold on;
plot(inter, Z(inter), 'g-'); hold on;
plot(inter, U(inter), 'm-');

legend('Max', 'Override', 'None', 'Max+Override');
xlabel('step of teacher');
ylabel('step to reach the goal for the learner');
title('How the advice affects the learner');

print(h, '../../data/graphs/graphA1.png')
