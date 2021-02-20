load('hw2_data.mat')

[m,n] = size(X);
M = 3;
b = X(M+1:m, 2);
A = zeros(m-M, M);

for i = 1 : m-M
    for j = M : -1 : 1
        A(i, M-j+1) = X(i+j-1, 2);
    end
end

A = [ones(m-M, 1) A];
[mA, nA] = size(A);

T = qr_householder(A);
R = zeros(mA, nA);

for k = 1 : nA
    R(1:k, k) = T(1:k, k);
end

q_her_b = b;
for k = 1 : nA
    v = T(k+1:mA+1, k);
    q_her_b(k:mA, :) = q_her_b(k:mA, :) - 2*v*(v'*q_her_b(k:mA, :));
end

xQR = R\(q_her_b)

