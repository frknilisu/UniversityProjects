load('hw1_data.mat')

% Change each nonzero entry of X as X_ijk = log(X_ijk) + 1
index = find(X ~= 0);
X(index) = log(X(index))+1;

% Collapse the three-way array X by summing up over the years mode
Z = sum(X, 3);

% Compute the SVD of Z
[U, S, V] = svd(Z);

% Plot of singular values of matrix Z
plot(diag(S), '*');
title('Singular values of Z')

% Construct the best rank-K approximation of Z denoted as Z_k for different K values
K = [2 10 20 50 100 300];
for i = 1:length(K)
    k = K(i);
    Z_k = U(:,1:k)*S(1:k,1:k)*V(:,1:k)';
    
    % Replace every nonzero entry of Y with 1
    Y(find(Y ~= 0)) = 1;
    
    % Vectorize Y
    Y = Y(:);
    
    % Vectorize Z_k
    Z_k = Z_k(:);
    
    % plot the Receiver Operating Characteristics (ROC) Curve
    % calculate the area under the curve (AUC)
    [Xn,Yn,Tn,AUC] = perfcurve(Y, Z_k, 1);
    
    figure;
    plot(Xn,Yn);
    xlabel('False positive rate');
    ylabel('True positive rate');
    title('ROC');
    legend(strcat('AUC: ', num2str(AUC)), 'Location', 'east');
    
end