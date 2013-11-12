close all
clear all
%TMS3_BI_5002_1.25
X = load('../../data/FMS3__300');
XP = load('../../data/TBMS3_IEA_300_100');

h = figure ;
plot(mean(X));hold on;
plot(mean(XP), 'g');hold on;
axis([0 100 -500 -100]);




Y = load('../../data/TMS3_BI_5002_1.25#3');
plot(-mean(Y), 'r', "linewidth", 2);hold on;


%  Z = load('../../data/TMS3_BI_5002_1.25#2');
%  plot(mean(Z), 'g');hold on;


legend('Learner no advice', 'Early Advise', 'Learner with advice');
xlabel('Training episodes');
ylabel('Average episode reward');

print('Z1_1.png')

h2 = figure ;

W = load('../../data/TMS3_BI_5002_1.25#7');
plot(mean(W))

title('Teacher reward over episods');
xlabel('Training episodes');

ylabel('Average episode reward');


print('Z1_2.png')

h3 = figure ;

V = load('../../data/TMS3_BI_5002_1.25#4');
plot(mean(V))
hold on;

M = load('../../data/TMS3_BI_5002_1.25#5');
plot(mean(M)*100, 'r')

legend('Number advice', 'Advice Ratio');

U = load('../../data/TMS3_BI_5002_1.25#6');
( size(Y, 2) * size(W, 2) ) / mean(U)
 size(Y, 2) * size(W, 2) 
 mean(U)

X = mean(X)(1:100);
Y = mean(Y)(1:100);

sum(Y - X)/100
