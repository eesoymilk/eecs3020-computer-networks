% Computer Networks HW2
% 109061138 張育瑋

% Referring to http://users.ece.cmu.edu/~koopman/crc/notes.html,
% Our generater polynomial C(x) is 0x104c11db7 in hex and on the table, 
% we see: 
% (0x82608edb; 0x104c11db7) <=> 
% (0xedb88320; 0x1db710641) 
% {4294967263,91607,2974,268,171,91,57,34,21,12,10,10,10}
% This indicates that some 4 bit errors that will fail 
% for a total length greater than 2974+32=3006 bits
% In conclusion, we know that we can change only 4 bits to have 
% a undetectable error.

C = [1 ...
     0 0 0 0 0 1 0 0 ...
     1 1 0 0 0 0 0 1 ...
     0 0 0 1 1 1 0 1 ...
     1 0 1 1 0 1 1 1];

% This while loop will go through all combinations for 12032 choose 4
N = 12032;
K = 4;
indices = 1:K;
while true
    % Process current combination
    positions = indices;
    E = zeros(1, N);
    E(positions) = 1;

    % Calculate the remainder R
    R = E;
    for i = 1:(length(R) - 32)
        if R(i) == 1
            R(i: i + length(C) - 1) = bitxor(R(i: i + length(C) - 1), C);
        end
    end

    % Check if the remainder R is all 0's
    % if it is, then we have found the answer.
    if nnz(R) == 0
        error_positions = positions;
        error = E;
        break;
    end

    % Generate next combination
    for i = K:-1:1
        if indices(i) < N - K + i
            indices(i) = indices(i) + 1;
            for j = i+1:K
                indices(j) = indices(j-1) + 1;
            end
            break;
        elseif i == 1
            break;
        end
    end
    if indices(1) == N - K + 1
        break;
    end
end

% The above while loop approximately took 2.5 days on my machine,
% and the final result error_positions is [1, 35, 49, 5870].
% You can simply generate the error as follows:
% error = zeros(1, 12032);
% error([1, 35, 49, 5870]) = 1;

if exist("109061138.mat", 'file')
  save("109061138.mat", "error", '-append');
else
  save("109061138.mat", "error");
end
