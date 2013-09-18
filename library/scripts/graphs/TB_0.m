close all
clear all

X = load('../../data/FM_100_0') ;
Y = load('../../data/TBMMC_100_0_150');
Z = load('../../data/TBMEA_100_0_150');
U = load('../../data/TBMIA_100_0_150');


plot(mean(X), "linewidth", 2)
hold on;
axis([0 100 -500 -100])


plot(mean(Y), 'r', "linewidth", 2);hold on;
plot(mean(Z), 'g', "linewidth", 2);hold on;
plot(mean(U), 'm', "linewidth", 2); hold on;
legend('No advice', 'MS', 'EA', 'IA');

X = mean(X)(1:100);
Y = mean(Y);
sum(Y - X)/100

U = mean(U);
sum (U - X )/100

Z = mean(Z);
sum( Z - X )/100

ad = load('../../data/TBMIA_100_0_150#2');
mean(ad)(100)
ad = load('../../data/TBMMC_100_0_150#2');
mean(ad)(100)
