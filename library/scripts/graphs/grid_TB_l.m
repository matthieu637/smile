close all
clear all

X = load('../../data/FGS3__200') ;
Y = load('../../data/TBGS3_IEA_200_150');
Z = load('../../data/TBGS3_IIA_200_150');
U = load('../../data/TBGS3_IMC_200_150');


plot(mean(X), "linewidth", 2)
hold on;
axis([0 200 -1000 50])


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

ad = load('../../data/TBGS3_IEA_200_150#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBGS3_IIA_200_150#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBGS3_IMC_200_150#2');
mean(ad)(size(ad, 2))

print('grid_TB_t_s.png')
