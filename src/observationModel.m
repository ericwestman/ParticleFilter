clear; close all; clc;

start = 0;
final = 819;
n = 10000;

% Gaussian parameters
mu = 700;
sigma_g = 10;
% g_max = 1/(sigma_g*sqrt(2*pi));
g_max = 1;

% Exponential parameters
sigma_e = -0.001;
a = g_max*3/4;

% Uniform parameters
uniform = g_max*1/2;


% input vector
x = linspace(start,final,n);


exponential = a*exp(sigma_e.*x);

normal_dist = exp(-1/2 .* ((mu-x)/(sigma_g)).^2);

max_range = zeros(1, length(x));
max_range(ceil(818*n/final):end) = g_max*3/4;

%% Plot old distribution
figure(1); hold on;
plot(x,exponential + normal_dist + max_range + uniform,'r-');

%% Plot new distribution
observation_model = max(max(max(exponential,normal_dist),max_range),uniform);
observation_model(1) = 0;
plot(x,observation_model,'b-');