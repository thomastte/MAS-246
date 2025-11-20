
clc; clear; close all; 

name = 'COM5'
rate = 9600
s = serialport(name,rate)

nsamples = 5000;
data = zeros(nsamples,1);

t0 = tic;

for k = 1:nsamples
    line = readline(s);
    val = str2double(line)
    if ~isnan(val)
        data(k) = val;
        timeVals(k) = toc(t0);
    else
        data(k) = data(max(k-1,1));  % simple hold if parse fails
        timeVals(k) = toc(t0);
    end
end
clear s;

%%
clc; close all; 
CPR = 8384;
dataCorr = data/CPR; %Position in rotation/floor
dataAng = dataCorr*(2*pi);

fs = 1/mean(diff(timeVals));
fcut = 0.0001; 

dataAngFiltered = lowpass(dataAng,fcut,fs);

%finding the velocity and filtering
vel = gradient(dataAng,timeVals);
velFiltered = lowpass(vel,fcut,fs);

%finding the acceleration and filtering
acc = gradient(vel,timeVals);
accFiltered = lowpass(acc,fcut,fs);



figure
subplot(2,1,1)
plot(timeVals,dataCorr,LineWidth=2,Color="#FFC0CB")
ylabel('Floor')
xlabel('Time(s)')
ylim([0 7])
xlim([0 45])
yticks(0:7)
grid on

subplot(2,1,2)
yyaxis("left")
plot(timeVals,velFiltered)
ylabel('Velocity(rad/s)')
ylim([-5 5])

hold on
yyaxis("right")
plot(timeVals,accFiltered)
ylabel('Acceleration(rad/s^2)')
ylim([-40 40])

xlabel('Time(s)')
grid on