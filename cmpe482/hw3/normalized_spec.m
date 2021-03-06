load('toy1.mat')

sigma = 0.7;
str1 = 'Toy Data 1: Normalized Cluster with K';
str2 = 'Toy Data 2: Normalized Cluster with K';

for k = 5:7
    C = normalized_spectral_clustering(X, k, sigma);
    print(X, C, k, str1);
end

clearvars X;
load('toy2.mat')
k = 3;
C = normalized_spectral_clustering(X, k, sigma);
print(X, C, k, str2);


function print(X, C, K, str)
    figure;
    colors = ['r', 'g', 'b', 'k', 'y', 'c', 'm'];
    for idx = 1 : K
      mask = C == idx;
      scatter(X(mask, 1), X(mask, 2), 50, 'MarkerFaceColor', colors(idx), 'MarkerEdgeColor', colors(idx));
      hold on
    end
    hold off
    
    title(sprintf('%s=%d',str,K))
    xlabel('x coordinates of points')
    ylabel('y coordinates of points');
end

function C = normalized_spectral_clustering(X, K, sigma)
    N = size(X, 1);
    dist_mat = squareform(pdist(X));
    
    W = zeros(N, N);

    for i = 1:N
        for j = 1:N
            if i == j
                W(i, j) = 1;
            else
                W(i, j) = exp(-(dist_mat(i, j)^2)/(2*(sigma^2)));
            end
        end
    end


    D = diag(sum(W,2));
    
    % normilazed laplacian
    L = ones(N, N) - D^(-0.5)*W*D^(-0.5);

    [V, E] = eigs(L, K);
    U = V./sqrt(sum(V.^2, 2));
    
    C = kmeans(U, K);
    
end
