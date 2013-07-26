clear all
close all


q=0.001:0.001:0.040

ii=1
for i=q
   [tok, rem] = strtok(num2str(i), '.');
   if(length(rem) == 3) 
      rem = cstrcat(rem, '0');
   endif
   tmp = mean( load( cstrcat('../../data/mcar_qltable_teacher_', rem) ) ); 
   Y(ii)=tmp(3);
   tmp2 = mean( load( cstrcat('../../data/mcar_qltable_teacher_annonce_', rem) ) ); 
   Z(ii)=tmp2(3);
   ii=ii+1;
end

h = figure

x=1:ii-1


plot(q, Y, 'r-'); hold on;
plot(q, Z, 'g-');

legend('no advise', 'advise after', 'advise before');
xlabel('cout');
ylabel('temps moyen d apprentissage');
title('Evolution du temps d apprentissage par rapport au cout ');

print(h, '../../data/graphs/graphA1.png')

