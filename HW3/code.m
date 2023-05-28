% Computer Networks HW3
% 109061138 張育瑋

A = load('network_A.mat', 'A').A;

visited = false(1, 100);
tree = zeros(100, 100);

% Create a queue for BFS.
queue = zeros(1, 100);  % Max queue size is equal to the number of nodes
front = 1;
rear = 1;

% Push the root node into the queue.
queue(rear) = 1;
visited(1) = true;

while front <= rear  % While the queue is not empty
    % Pop the queue
    node = queue(front);
    front = front + 1;

    % Check all neighbors of this node.
    for neighbor = 1:100
        if A(node, neighbor) == 0 || visited(neighbor)
            continue;  % Skip this neighbor if there is no edge or it has been visited
        end

        % Mark this neighbor as visited and add it to the queue.
        visited(neighbor) = true;
        rear = rear + 1;
        queue(rear) = neighbor;

        % Add the edge between the current node and this neighbor to the tree.
        tree(node, neighbor) = 1;
        tree(neighbor, node) = 1;  % since it's an undirected graph
    end
end

save("result.mat", "tree");
