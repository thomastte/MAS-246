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
data(150,:) = data(149,:);
data(292,:) = data(291,:);
data(434,:) = data(433,:);
data(510,:) = data(509,:); 
data(583,:) = data(582,:); 
data(659,:) = data(658,:); 
data(854,:) = data(853,:); 

power = data(:,1); %Pådrag fra PID
start = 1; 
endP = 943; %943
elapTime = data([start:endP],2);
elapTime = timeVals(start:endP);
desPos = data([start:endP],3);
desVel = data([start:endP],4);
dest = data([start:endP],5);
encCount = data([start:endP],6);
Pos = encCount;



fcut = 1e-50;
fs = 1/mean(diff(elapTime));

PosFilt = lowpass(Pos,fcut,fs);

vel = gradient(PosFilt,elapTime);
VelFilt = lowpass(vel,fcut,fs);
acc = gradient(VelFilt, elapTime);
accFilt = lowpass(acc,fcut,fs);

%plotting position data
figure
plot(elapTime, desPos,LineWidth=5,Color='#4682B4') %desired position
hold on
plot(elapTime,Pos,Color='#DC143C', LineWidth=2) %measured position
ylabel('Position(floor)','FontSize',15)
xlabel('Time(s)','FontSize',15)
ylim([-0.1 7.2])
legend('Desired Position', 'Measured Position','Location','north','FontSize', 12)
title('Position of Cabin Over Time','FontSize',20)
grid on

velCorr(920:end) = desVel(920:end);
acc = gradient(velCorr, elapTime);
accFilt = lowpass(acc, fcut, fs);

%Plotting dynamic data
figure
subplot(2,1,1)
plot(elapTime, desVel, LineWidth=5, Color='#4682B4')
hold on
plot(elapTime, velCorr, LineWidth=3, Color='#DC143C')
grid on
ylabel('Velocity (floors per second)', FontSize=12)
xlabel('Time(s)',FontSize=12)
legend('Desired Velocity', 'Measured Velocity','Location','southwest','FontSize',12)
title('Velocity of Cabin Over Time',FontSize=20)

subplot(2,1,2)
plot(elapTime, accFilt, LineWidth=5,Color='#32CD32')
ylabel('Acceleration (units per second^2)','FontSize',12)
xlabel('Time(s)',FontSize=12)
title('Acceleration of Cabin Over Time', FontSize=20)
grid on


