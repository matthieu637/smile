close all
clear all

Z = load('/home/matthieu/workspace/java/mountaincar/myData1/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData1/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData1/slowS/slowS/correct134/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData1/slowS/slowS/advise132/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

xlabel('Episodes');
ylabel('Average reward');
title('Advice effects with heuristics');

print(h, 'lisa_slowS.png');

k=1;
n=248:-2:0;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myData1/slowS/slowS/correct',
		num2str(i),'/avg_curve'));
	QQ(k,1) = mean(q(:,2));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m(1)
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])

clear all
%################################################################################################

Z = load('/home/matthieu/workspace/java/mountaincar/myData2/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData2/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData2/slowS/slowS/correct186/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData2/slowS/slowS/advise182/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

%  print(h, 'lisa_slowS.png');

k=1;
n=248:-2:0;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myData2/slowS/slowS/advise',
		num2str(i),'/avg_curve'));
	QQ(k,1) = mean(q(:,2));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m(1)
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])

clear all
%################################################################################################

Z = load('/home/matthieu/workspace/java/mountaincar/myData3/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData3/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData3/slowS/slowS/correct48/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData3/slowS/slowS/advise4/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

xlabel('Episodes');
ylabel('Average reward');
title('Advice effects with heuristics');

print(h, 'lisa_slowS_3.png');

k=1;
n=248:-2:0;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myData3/slowS/slowS/correct',
		num2str(i),'/avg_curve'));
	QQ(k,1) = mean(q(:,2));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m(1)
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])


clear all
%################################################################################################

Z = load('/home/matthieu/workspace/java/mountaincar/myData4/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData4/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData4/slowS/slowS/correct100/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData4/slowS/slowS/advise102/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

  print(h, 'lisa_slowS_50.png');

k=1;
n=248:-2:0;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myData4/slowS/slowS/correct',
		num2str(i),'/avg_curve'));
	QQ(k,1) = mean(q(:,2));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m(1)
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])

clear all
%################################################################################################

Z = load('/home/matthieu/workspace/java/mountaincar/myData5/slowS/slowS/independent/avg_curve');
U = load('/home/matthieu/workspace/java/mountaincar/myData5/slowS/slowS/baseline/avg_curve');
V = load('/home/matthieu/workspace/java/mountaincar/myData5/slowS/slowS/correct66/avg_curve');
X = load('/home/matthieu/workspace/java/mountaincar/myData5/slowS/slowS/advise6/avg_curve');

h = figure

plot(Z(:,2), 'g', "linewidth", 2);hold on;
plot(U(:,2), 'y', "linewidth", 2);hold on;
plot(V(:,2), 'b', "linewidth", 2);hold on;
plot(X(:,2), 'r', "linewidth", 2);hold on;
axis([0 300 -500 -100])
legend('no advice', 'early adv.', 'mistake cor.', 'important adv');

  print(h, 'lisa_slowS_250.png');

k=1;
n=248:-2:0;
QQ=zeros(size(n,2), 2);
for i=n
    q = load( cstrcat('/home/matthieu/workspace/java/mountaincar/myData5/slowS/slowS/correct',
		num2str(i),'/avg_curve'));
	QQ(k,1) = mean(q(:,2));
	QQ(k,2) = i;
	k = k +1;
endfor

[m,i] = max(QQ);
m(1)
QQ(i(1), 2)
QQ;

figure

plot(QQ(:,2),QQ(:,1))
axis([0 250 -220 -150])
