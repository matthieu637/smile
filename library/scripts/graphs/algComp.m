close all
clear all

X = load('../../data/FMS3T_300') ;
Y = load('../../data/FMQ3T_300');


plot(mean(X), "linewidth", 2);hold on;
plot(mean(Y), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])

legend('Sarsa', 'QL');

