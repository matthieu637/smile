close all
clear all

X = load('TBMPP_1000_0_500_M') ;
Y = load('FM_1000_0');
Z = load('TBMPP_1000_0_500_I');


plot(mean(X), "linewidth", 2)
hold on;
axis([0 100 -500 -100])

plot(mean(Y), 'r', "linewidth", 2);hold on;
plot(mean(Z), 'g', "linewidth", 2);hold on;

legend('M','N','I');
X = mean(X);
Y = mean(Y);
Z = mean(Z);

%sum(X - Y)
%sum(Z - Y)

ad = load('TBMPP_1000_0_500_M#2');
mean(ad)(100)
ad = load('TBMPP_1000_0_500_I#2');
mean(ad)(100)
