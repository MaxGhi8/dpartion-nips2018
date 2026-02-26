clear
close all
format short

rand('seed', 0);

% SET YOUR LOCAL PATH TO THE BENCHMARKS
base = "../DOTmark/Data/";

imsize = "/data32_";
images = ["CauchyDensity", "ClassicImages", "GRFmoderate", "GRFrough", "GRFsmooth", "LogGRF", "LogitGRF", "MicroscopyImages", "Shapes", "WhiteNoise"];
fs = ["1001.csv","1002.csv","1003.csv","1004.csv","1005.csv","1006.csv","1007.csv","1008.csv","1009.csv","1010.csv"];
nh = 32;

d1 = nh*nh;
d2 = nh*nh;

C = zeros(d1,d2);
for i = 1:nh
    for j = 1:nh
        for v = 1:nh
            for w = 1:nh
                C(nh*(i-1)+j,nh*(v-1)+w) = (i-v)^2 + (j-w)^2;
            end
        end
    end
end

k = length(fs);
ni = length(images);

% Regularization parameter
lambda = 0.125;

% Pre-compute K and U for sinkhornTransport
K = exp(-lambda * C);
U = K .* C;

for fi = 1:ni
    for f1 = 1:(k-1)
        a = csvread(base+images(fi)+imsize+fs(f1));
        a = a(:);
        % a = a / sum(a); % removed to match myFSscript.m
        for f2 = (f1+1):k
            b = csvread(base+images(fi)+imsize+fs(f2));
            b = b(:);
            % b = b / sum(b); % removed to match myFSscript.m
            
            % Measure time
            tStart = tic;
            % sinkhornTransport(a, b, K, U, lambda, stoppingCriterion, p_norm, tolerance, maxIter, VERBOSE)
            D = sinkhornTransport(a, b, K, U, lambda, 'marginalDifference', inf, 0.1e-5, 2000, 0);
            tElapsed = toc(tStart);

            disp(join([images(fi),fs(f1),fs(f2),'lambda',num2str(lambda),'runtime', num2str(tElapsed), 'UB', num2str(D)]));
        end
    end
end
