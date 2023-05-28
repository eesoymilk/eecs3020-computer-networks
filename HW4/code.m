% Computer Networks HW4
% 109061138 張育瑋

A = load('network_A.mat', 'A').A;
d = allPairsShortestPaths(A);
save("result.mat", "d");

% % This code is used for testing the input_test
% input_test = load('input_test.mat', 'A').A;
% output_test = load('output_test.mat', 'd').d;
% input_result = allPairsShortestPaths(input_test);
% 
% if isequal(input_result, output_test)
%     disp('Passed')
% else
%     disp('Failed')
% end

function d = allPairsShortestPaths(A)
    % A is an adjacency matrix.
    
    n = size(A, 1);  % Number of nodes.
    d = zeros(n, n);  % Initialize distance matrix.
    
    % Replace zeros in adjacency matrix with Inf (except diagonal)
    A(A == 0) = Inf;  
    A(1:n+1:end) = 0;  
    
    % Loop over all nodes.
    for source = 1:n
        dist = inf(1, n);  % Initialize all distances as Inf.
        dist(source) = 0;  % Distance from source to itself is 0.
        visited = false(1, n);  % No nodes have been visited yet.
        
        for i = 1:n
            % Find the unvisited node with the smallest distance.
            visited_filter = zeros(1, n);
            visited_filter(visited) = Inf;
            [~, u] = min(dist + visited_filter);
            
            % Mark this node as visited.
            visited(u) = true;
            
            % Update distances to neighboring nodes.
            dist = min(dist, dist(u) + A(u, :));
        end
        
        % Save distances from this source.
        d(source, :) = dist;
    end
end
