close all
clear all

X = load('../../data/FMS3__300') ;
Y = load('../../data/TBMS3_IEA_300_100');
Z = load('../../data/TBMS3_IIA_300_100_24_24');
U = load('../../data/TBMS3_IMC_300_100_8_8');


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

ad = load('../../data/TBMS3_IEA_300_100#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBMS3_IIA_300_100_24_24#2');
mean(ad)(size(ad, 2))
ad = load('../../data/TBMS3_IMC_300_100_12_12#2');
mean(ad)(size(ad, 2))

print('mcar_TB_l_s3.png')



k=1;
n=0:2:250;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('../../data/TBMS3TIIA_300_100_',
		num2str(i),'_',num2str(i)));
	QQ(k,1) = mean(mean(q));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])
