close all
clear all

X = load('../../data/FMQ3T_300') ;
Y = load('../../data/TBMQ3TIEA_300_100');
Z = load('../../data/TBMQ3TIIA_300_100');
U = load('../../data/TBMQ3TIMC_300_100');


plot(mean(X), "linewidth", 2)
hold on;
axis([0 300 -500 -100])


plot(mean(Y), 'r', "linewidth", 2);hold on;
plot(mean(Z), 'g', "linewidth", 2);hold on;
plot(mean(U), 'm', "linewidth", 2); hold on;
legend('No advice', 'EA', 'IA', 'MC');

X = mean(X);
Y = mean(Y);

sum(Y - X)

Z = mean(Z);
sum(Z - X)

U = mean(U);
sum (U - X )

ad = load('../../data/TBMQ3_IEA_300_100#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBMQ3_IIA_300_100#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBMQ3_IMC_300_100#2');
mean(ad)(size(ad, 2))

print('mcar_TB_t_ql.png')
