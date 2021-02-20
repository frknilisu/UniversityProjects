function X = qr_householder( A )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    [m,n] = size(A);
    R = A;
    
    for k = 1 : n
        x = R(k:m, k);
        e = zeros(length(x), 1);    e(1) = 1;
        v =  x + sign(x(1))*norm(x)*e;
        v = v./norm(v);
        
        R(k:m, k:n) = R(k:m, k:n) - 2*v*(v'*R(k:m, k:n));
        V(k:m, k) = v;
    end
    
    X = zeros(m+1, n);
    X(1:m, :) = R;
    
    for k = 1 : n
        X(k+1:m+1, k) = V(k:m, k);
    end

end

