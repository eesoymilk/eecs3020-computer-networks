A = load('result.mat', 'tree').tree;
B = load('result_oyea.mat', 'tree').tree;
count = 0;

for i = 1 : 100
    for j = 1 : 100
        if A(i, j) == B(i, j)
            continue
        end
        count = count + 1;
    end
end

disp(count);
