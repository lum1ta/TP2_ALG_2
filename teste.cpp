std::pair<std::vector<int>,std::vector<int>>
Dijkstra(std::vector<std::vector<std::pair<int,int>>>& graph,int src){

    int V = graph.size();
    const int INF = 1e9;

    std::priority_queue<
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<std::pair<int,int>>
    > pq;

    std::vector<int> dist(V,INF);
    std::vector<int> parent(V,-1);

    dist[src] = 0;

    pq.push({0,src});

    while(!pq.empty()){

        auto [d,u] = pq.top();
        pq.pop();

        if(d > dist[u])
            continue;

        for(auto &[v,w] : graph[u]){

            if(dist[u] + w < dist[v]){

                dist[v] = dist[u] + w;

                parent[v] = u;

                pq.push({dist[v],v});
            }
        }
    }

    return {dist,parent};
}