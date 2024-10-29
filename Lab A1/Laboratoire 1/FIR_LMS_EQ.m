%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FIR-LMS EQUALIZER
% Author : Messaoud Ahmed Ouameur
% Contact information : 
% Department of Electrical & Computer Engineering,
% Université du Québec à Trois-Rivières,
% 3351, boul. des Forges, C.P. 500, Trois-Rivières (Québec) G9A 5H7 
% email :Messaoud.Ahmed.Ouameur@uqtr.ca  
% Date    : 1-9-2019
% Version : 1.0.0
% Reference : S. Haykin, Adaptive Filter Theory. 3rd edition, Upper Saddle River, NJ: Prentice-Hall, 1996. 
% Note :  For any harm caused by using this code the author doesn't endorse any responsibility

clc
clear all
close all


% rng('shuffle')

% channel h
h=  [0.5976; 0.02873; -0.1360; 0.02210; 0.00964;];
%channel system order
channelorder = length(h) ;
% SNR in dB
SNR=10;
% Number of points
N=2000;
%Consider N_TRAIN points for training
N_TRAIN=200 ;

% inp = randn(N,1);
inp = 0.5*sign(2*randn(N,1)+1);      % Generate binnary signal
% inp = 1*sin([1:1:N]*2*10*pi/N)';
noise_n = randn(N,1);
y = zeros(N,1);
inpvector=zeros(channelorder,1);
for n=1:N
    inpvector=[inp(n);inpvector(1:end-1,1)];
    y(n)=inpvector.'*h;
end

% add AWGN 
noise_n = noise_n * 10^(-SNR);
yn = y + noise_n;
totallength=size(yn,1);

% least mean square algorithm
w = zeros (channelorder,1);
yn_input = zeros (channelorder,1);
for n = 1 : N_TRAIN 
	yn_input=[yn(n);yn_input(1:end-1,1)];
    inpest(n,1)= w.' * yn_input;
    e(n) = inp(n)-inpest(n);
    
    if n < 20
        mu=0.52; % Start with large mu to speed up the convergence 
    else
        mu=0.22; % then slow down to get closer to the true weights
    end
	w = w + mu * e(n) * yn_input;
end 
% equalization
for n =  N_TRAIN+1 : N
	yn_input=[yn(n);yn_input(1:end-1,1)];
    inpest(n,1)= w' * yn_input;
    e(n) = inp(n) - inpest(n) ;
end 

figure(1)
hold on
stem(inpest(1:300));
stem(inp(1:300),'r');
title('System output') ;
xlabel('Samples')
ylabel('True and estimated output')
figure(2)
semilogy(e.*e) ;
title('Error curve') ;
xlabel('Samples')
ylabel('Error value')

hw=conv(h,w)
figure(3)
stem(hw)
HW_Fac=10*log10(hw(1)*hw(1)/sum(hw(2:end).*hw(2:end)))
% BER=sum((sign(inpest(N_TRAIN+1 : N))+2)/2-(sign(inp(N_TRAIN+1 : N))+2)/2)/(N-N_TRAIN)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fid=fopen('C:\Users\14505\Desktop\GEI1064\Lab A1\yn.txt','wt');


for i=1:N
    fprintf(fid,'%f', yn(i));
    fprintf(fid,'\n');
end

fclose(fid);

fid=fopen('C:\Users\14505\Desktop\GEI1064\Lab A1\inp.txt','wt');

for i=1:N_TRAIN 
    fprintf(fid,'%f', inp(i));
    fprintf(fid,'\n');
end

fclose(fid);

fid=fopen('C:\Users\14505\Desktop\GEI1064\Lab A1\inpest.txt','wt');


for i=1:N
    fprintf(fid,'%f', inpest(i));
    fprintf(fid,'\n');
end

fclose(fid);