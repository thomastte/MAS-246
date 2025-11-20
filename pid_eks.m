clc; clear; close all; 

num = 1; 
den = [1 5 6];
P = tf(num,den);

Kp = 60; 
Ki = 10; 
Kd = 1; 
C = pid(Kp, Ki, Kd);

T = feedback(C * P,1);
figure
step(P);
hold on 
step(T);
legend('raw', 'pid')