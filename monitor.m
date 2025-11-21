clc; clear; close all; 

name = 'COM5';
rate = 9600;
s = serialport(name,rate,"Timeout", 2);

nsamples = 5000;
data = zeros(nsamples,5);
timeVals = zeros(nsamples,1);
flush(s);

t0 = tic;

for k = 1:nsamples

    try
        rawLine = readline(s);
    catch
        warning("readline failed at sample %d", k);
        break
    end
    

    line  = string(rawLine);
    parts = split(strtrim(line), ",");
    nums  = str2double(parts);

    % Case 1: Arduino outputs "0"
    if numel(nums) == 1 && ~isnan(nums(1))
        enc = nums(1);
        % fill all 5 columns with encoder-only info
        data(k, :) = [NaN, NaN, NaN, NaN, enc];

    % Case 2: Arduino outputs full "a,b,c,d,e"
    elseif numel(nums) >= 5 && all(~isnan(nums(1:5)))
        data(k, :) = nums(1:5).';

    % Case 3: broken line → repeat previous
    elseif k > 1
        data(k,:) = data(k-1,:);
    end

    timeVals(k) = toc(t0);
end

clear s;
%%
clc; close all; 
elapTime = data(:,1);
desPos = data(:,2);
desVel = data(:,3);
dest = data(:,4);
encCount = data(:,5);
Pos = encCount./8384;

fcut = 1e-50;
fs = 1/mean(diff(elapTime));

PosFilt = lowpass(Pos,fcut,fs)

figure
plot(elapTime, desPos,LineWidth=3)
ylabel('Desired Position')
ylim([0 7])
hold on 
yyaxis("right")
ylim([-1 1])
plot(elapTime, desVel,LineWidth=3)
ylabel('Desired Velocity')
%hold on
%plot(elapTime, Pos)
grid on
xlabel('Time(s)')
legend('Desired Position','Desired Velocity')

%%
clc; close all; 
CPR = 8384;
dataCorr = data/CPR; %Position in rotation/floor
dataAng = dataCorr*(2*pi);

fs = 1/mean(diff(timeVals));
fcut = 1e-50; 
%fcut = 50;

dataAngFiltered = lowpass(dataAng,fcut,fs);

%finding the velocity and filtering
vel = gradient(dataAngFiltered,timeVals);
velFiltered = lowpass(vel,fcut,fs);

%finding the acceleration and filtering
acc = gradient(velFiltered,timeVals);
accFiltered = lowpass(acc,fcut,fs);



figure
subplot(2,1,1)
plot(timeVals,dataCorr,LineWidth=2,Color="#FFC0CB")
ylabel('Floor')
xlabel('Time(s)')
ylim([0 87])
xlim([0 50])
yticks(0:10:87)
grid on

subplot(2,1,2)
yyaxis("left")
plot(timeVals,velFiltered)
ylabel('Velocity(rad/s)')
ylim([-10 10])
xlim([0 50])

hold on
yyaxis("right")
plot(timeVals,accFiltered)
ylabel('Acceleration(rad/s^2)')
ylim([-40 40])

xlabel('Time(s)')
grid on