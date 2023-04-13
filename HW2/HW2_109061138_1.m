% Computer Networks HW2
% 109061138 張育瑋

packet = load('inputdata.mat', 'packet').packet;

C = [1 ...
     0 0 0 0 0 1 0 0 ...
     1 1 0 0 0 0 0 1 ...
     0 0 0 1 1 1 0 1 ...
     1 0 1 1 0 1 1 1];
T = [packet zeros(1, 32)];
R = T;

for i = 1:length(R) - length(C) + 1
    if R(i) == 1
        R(i:i + length(C) - 1) = bitxor(R(i:i + length(C) - 1), C);
    end
end

R = R(end - length(C) + 2: end);

codepacket = bitxor(T, [zeros(1, length(packet)) R]);

if exist("109061138.mat", 'file')
  save("109061138.mat", "codepacket", '-append');
else
  save("109061138.mat", "codepacket");
end
