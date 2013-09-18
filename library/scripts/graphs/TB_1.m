close all
clear all

X = load('../../data/FM_100_1') ;
Y = load('../../data/TBMMC_100_1_150');
Z = load('../../data/TBMEA_100_1_150');
U = load('../../data/TBMIA_100_1_150');


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
%40 11.422
%45 11.379
%35 11.481

U = mean(U);
sum (U - X )/100
%35 11.470
%40 10.987
%30 11.402
%33 10.788

Z = mean(Z);
sum( Z - X )/100
