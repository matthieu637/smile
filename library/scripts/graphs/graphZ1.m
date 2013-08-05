close all
clear all

X = load('../../data/FMQ3');

h = figure ;
plot(mean(X));
axis([0 100 -500 -100]);

hold on;


Y = load('../../data/Z1#3');
plot(mean(Y), 'r');


Z = load('../../data/Z1#2');
plot(mean(Z), 'g');


legend('Leaner no advice', 'Leaner with advice', 'Teacher');
xlabel('Training episodes');
ylabel('Average episode reward');

h2 = figure ;

W = load('../../data/Z1#1');
plot(mean(W))

title('Teacher reward over episods');
xlabel('Training episodes');

ylabel('Average episode reward');


h3 = figure ;

V = load('../../data/Z1#4');
plot(mean(V))
hold on;

M = load('../../data/Z1#5');
plot(mean(M)*100, 'r')

legend('Number advice', 'Advice Ratio');

U = load('../../data/Z1#6');
( size(Y, 2) * size(W, 2) ) / mean(U)
 size(Y, 2) * size(W, 2) 
 mean(U)


