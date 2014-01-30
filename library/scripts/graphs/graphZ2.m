close all
clear all
% NUMBER ADVICE -> 50
%TMS3_BI_8000_13.5 reward 2 line, base working from reunion5
%TMS3_BI_8000_14 took best action reward -> 0 
%TMS3_BI_8000_15 fix state bug (work well better than MC)
%TMS3_BI_8000_16 took best action reward -> 0 (worst)
%TMS3_BI_8000_17 rm 16 + augment stat 3 action instead best action (best)
%TMS3_BI_8000_18 rm callEpisod (bad)

X = load('../../data/FMS3__300');
%XP = load('../../data/TBMS3_IEA_300_50');
XPP = load('../../data/TBMS3_IMC_300_50_148_148');
%XPP = load('../../data/TBMS3_IEA_300_100');

h = figure ;
plot(mean(X));hold on;
%plot(mean(XP), 'g');hold on;
plot(mean(XPP), 'g');hold on;
axis([0 100 -500 -100]);


Y = load('../../data/TMS3_BI_8000_15#3');
plot(-mean(Y), 'r', "linewidth", 2);hold on;



legend('Learner no advice = Early Advise', 'MC','Learner with advice');
xlabel('Learner episodes');
ylabel('Average episode reward');
title('Learner rewards after the teacher learned');

print('Z1_1.png')

h2b = figure

Z = load('../../data/TMS3_BI_8000_15#2');

plot(mean(Z), 'g', "linewidth", 2);hold on;

xlabel('Learner episodes');
ylabel('Average episode reward');
title('Teacher rewards after the teacher learned');

h2 = figure ;


W = load('../../data/TMS3_BI_8000_15#7');
W = mean(W);
inter=1:10:length(W);
plot(inter, W(inter))

title('Sum of teacher rewards over teacher learning');
xlabel('Teacher episodes');

ylabel('Average episode reward');


print('Z1_2.png')

h3 = figure ;

V = load('../../data/TMS3_BI_8000_15#4');
plot(mean(V), "linewidth", 2)
hold on;

%M = load('../../data/TMS3_BI_8000_15#5');
%plot(mean(M)*100, 'r')

axis([-1 101 -1 101])
legend('Number advice');%, 'Advice Ratio');
xlabel('Learner episodes');
title('Where the advices are given after learning');

%  U = load('../../data/TMS3_BI_8000_15#6');
%  ( size(Y, 2) * size(W, 2) ) / mean(U)
%   size(Y, 2) * size(W, 2) 
%   mean(U)
%  
%  X = mean(X)(1:100);
%  Y = mean(Y)(1:100);
%  
%  sum(Y - X)/100
