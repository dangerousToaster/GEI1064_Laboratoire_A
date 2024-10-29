%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FIR-LMS EQUALIZER
% Author : Messaoud Ahmed Ouameur
% Contact information : 
% Department of Electrical & Computer Engineering,
% Universit� du Qu�bec � Trois-Rivi�res,
% 3351, boul. des Forges, C.P. 500, Trois-Rivi�res (Qu�bec) G9A 5H7 
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
N_TRAIN=300;

mu=0.25;

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
w = zeros (channelorder,N_TRAIN+1);
yn_input = zeros (channelorder,1);
for n = 1 : N_TRAIN 
	yn_input=[yn(n);yn_input(1:end-1,1)];
    inpest(n,1)= w(:,n).' * yn_input;
    e(n) = inp(n)-inpest(n);
	w(:,n+1) = w(:,n) + mu * e(n) * yn_input;
end 
% equalization
for n =  N_TRAIN+1 : N
	yn_input=[yn(n);yn_input(1:end-1,1)];
    inpest(n,1)= w(:,N_TRAIN+1)' * yn_input;
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

figure(3)
hold on
for i = 1:channelorder
stem(w(i,:))
end
title('Coefficients convergence') ;
xlabel('Samples')
ylabel('Value')
legend('w1','w2','w3','w4','w5')
hold off
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fid=fopen('yn.txt','wt');

for i=1:N
    fprintf(fid,'%f', yn(i));
    fprintf(fid,'\n');
end

fclose(fid);

fid=fopen('inp.txt','wt');

for i=1:N_TRAIN 
    fprintf(fid,'%f', inp(i));
    fprintf(fid,'\n');
end

fclose(fid);

fid=fopen('inpest.txt','wt');

for i=1:N
    fprintf(fid,'%f', inpest(i));
    fprintf(fid,'\n');
end

fclose(fid);