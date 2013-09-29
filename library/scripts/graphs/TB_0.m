close all
clear all

X = load('../../data/FM_300_0') ;
Y = load('../../data/TBMMC_300_0_150');
Z = load('../../data/TBMEA_300_0_150');
U = load('../../data/TBMIA_300_0_150');


plot(mean(X), "linewidth", 2)
hold on;
axis([0 300 -500 -100])


plot(mean(Y), 'r', "linewidth", 2);hold on;
plot(mean(Z), 'g', "linewidth", 2);hold on;
plot(mean(U), 'm', "linewidth", 2); hold on;
legend('No advice', 'MS', 'EA', 'IA');

X = mean(X)(1:300);
Y = mean(Y);
sum(Y - X)/300

U = mean(U);
sum (U - X )/300

Z = mean(Z);
sum( Z - X )/300

ad = load('../../data/TBMMC_300_0_150#2');
mean(ad)(300)
