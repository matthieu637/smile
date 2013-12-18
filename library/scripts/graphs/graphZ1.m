close all
clear all
%TMS3_BI_5003_1.8 not that bad
%TMS3_BI_5003_1.9 10 500 -2 1 
%TMS3_BI_5003_1.7 10 500 -2 1 ntb
%TMS3_BI_5003_1.6 50 500 -2 1 good
%TMS3_BI_5003_1.60 50 500 -2 1 good
%TMS3_BI_10000_1.61 50 500 5003 -> 10000 ntb
%TMS3_BI_10000_1.601 50 500 10000 features 50->100 really bad
%TMS3_BI_2_1.601 forced to not advice
%TMS3_BI_3_1.601 forced to advice
%TMS3_BI_10000_10 remove cost really bad
%TMS3_BI_10000_11 remove cost +1 to 0 good
%TMS3_BI_10000_12 remove all TOTALY WORKS!
%TMS3_BI_5000_12 remove callEpisod
%TMS3_BI_5000_13 add callEpi, specify action


X = load('../../data/FMS3__300');
XP = load('../../data/TBMS3_IEA_300_100');

h = figure ;
plot(mean(X));hold on;
plot(mean(XP), 'g');hold on;
axis([0 100 -500 -100]);


Y = load('../../data/TMS3_BI_10000_12#3');
plot(-mean(Y), 'r', "linewidth", 2);hold on;



legend('Learner no advice', 'Early Advise', 'Learner with advice');
xlabel('Learner episodes');
ylabel('Average episode reward');
title('Learner rewards after the teacher learned');

print('Z1_1.png')

h2b = figure

Z = load('../../data/TMS3_BI_10000_12#2');

plot(mean(Z), 'g', "linewidth", 2);hold on;

xlabel('Learner episodes');
ylabel('Average episode reward');
title('Teacher rewards after the teacher learned');

h2 = figure ;


W = load('../../data/TMS3_BI_10000_12#7');
W = mean(W);
inter=1:10:length(W);
plot(inter, W(inter))

title('Sum of teacher rewards over teacher learning');
xlabel('Teacher episodes');

ylabel('Average episode reward');


print('Z1_2.png')

h3 = figure ;

V = load('../../data/TMS3_BI_10000_12#4');
plot(mean(V), "linewidth", 2)
hold on;

%M = load('../../data/TMS3_BI_10000_12#5');
%plot(mean(M)*100, 'r')

axis([-1 101 -1 101])
legend('Number advice');%, 'Advice Ratio');
xlabel('Learner episodes');
title('Where the advices are given after learning');

%  U = load('../../data/TMS3_BI_10000_12#6');
%  ( size(Y, 2) * size(W, 2) ) / mean(U)
%   size(Y, 2) * size(W, 2) 
%   mean(U)
%  
%  X = mean(X)(1:100);
%  Y = mean(Y)(1:100);
%  
%  sum(Y - X)/100
