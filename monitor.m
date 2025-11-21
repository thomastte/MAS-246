clc; clear; close all; 

name = 'COM5';
rate = 9600;
s = serialport(name,rate,"Timeout", 2);

nsamples = 8000; % how many data points you want available
data = zeros(nsamples,6);
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
        data(k, :) = [NaN, NaN, NaN, NaN, NaN, enc];

    % Case 2: Arduino outputs full "a,b,c,d,e"
    elseif numel(nums) >= 6 && all(~isnan(nums(1:6)))
        data(k, :) = nums(1:6).';

    % Case 3: broken line → repeat previous
    elseif k > 1
        data(k,:) = data(k-1,:);
    end

    timeVals(k) = toc(t0);
end

clear s;
%%
clc; close all; 
power = data(:,1); %Pådrag fra PID
start = 1; 
endP = 669;

elapTime = data([start:endP],2);
elapTime = timeVals(start:endP);
desPos = data([start:endP],3);
desVel = data([start:endP],4);
dest = data([start:endP],5);
encCount = data([start:endP],6);
Pos = encCount;

Pos(203) = Pos(202);
Pos(397:398) = Pos(396);
%Pos(end) = Pos(end-1);

fcut = 1e-50;
fs = 1/mean(diff(elapTime));

PosFilt = lowpass(Pos,fcut,fs);

vel = gradient(PosFilt,elapTime);
VelFilt = lowpass(vel,fcut,fs);
acc = gradient(VelFilt, elapTime);
accFilt = lowpass(acc,fcut,fs);

figure
plot(elapTime, desPos,LineWidth=4)
hold on
plot(elapTime,Pos,Color='r', LineWidth=2)
ylabel('Desired Position')
ylim([-0.1 7.2])
hold on 
yyaxis("right")
ylim([-1 1])
plot(elapTime, desVel,LineWidth=3)
hold on
plot(elapTime, vel,LineStyle="-",Color='g',LineWidth=2)
hold on
plot(elapTime, accFilt, LineStyle= "-", Color='m',LineWidth=2 )
ylabel('Desired Velocity')

grid on
xlabel('Time(s)')
legend('Desired Position','Pos','Desired Velocity','Velocity','Acceleration')
