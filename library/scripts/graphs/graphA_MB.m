clear all
close all

X = load('../../data/TMQ1Q1AMTB');
Y = load('../../data/TMQ1Q1AOTB');
Z = load('../../data/TMQ1Q1ANTB');
U = load('../../data/TMQ1Q1AITB');

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

print(h, '../../data/graphs/graphA_MB.png')
