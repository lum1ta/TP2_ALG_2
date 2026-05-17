/*Minha ideia inicial é fazer um dijkstra que ache o caminho minimo entre todos os vertices do grafo,apenas modificado para receber as entradas necessárias
Esse problema de salas e corredores pode ser modelado por um grafo nao direcionado  com pesos representando o comprimento de cada corredor
Regras básicas:
é um grafo conexo
tem no maximo um corredor entre duas salas
a sala inicial acessa todas as outras
N: Numero de salas
M:Numero de corredores
T:Numero de interruptores
Aparecem em sequencia na primei linha de entrada
Na segunda linha contem T inteiros que representam os interruptores em seuqencia
Em seguida tem M linhas com 3 inteiros que reprsentam primeiro vertice,segundo vertice e comprimento do corredor entre eles

*/
#include <bits/stdc++.h>

//Primeira que pega a entrada e monta o grafo
class Read_Graph{
    private:
    int n_rooms; //numero de salas
    int n_hallway; //numero de corredores
    int n_interruptor; //numero de interruptores

    //salas com o interruptor
    std::vector<int> switches; //devem ser percorridas primeiro aparentemente
    
    //lista de adjacencia que guarda o vizinho mais o peso da aresta
    std::vector<std::vector<std::pair<int,int>>> graph;

    std::vector<std::vector<int>> reduced_graph;
    
    public:
    //construtor
    Read_Graph(int room,int hallway,int interruptor)
        //as salas são numeradas de 1 a N entao a primeira posição do vetor fica inutilizada
        : n_rooms(room),n_hallway(hallway),n_interruptor(interruptor),graph(room + 1) {} //declarar o grafo reduzido

    void read_input(){
        //leitura dos interruptores
        for(int i = 0;i < n_interruptor;i++){
            int x;
            scanf("%d",&x);
            //colocar cada interruptor lido no vetor separado para ele
            switches.push_back(x);
        }

        //leitura dos corredores e as salas que estão nas extremidades
        for(int i = 0;i < n_hallway;i++){
            int u,v,w;
            scanf("%d %d %d",&u,&v,&w);
            
            //salvando o vertice e o peso da aresta
            graph[u].push_back({v,w});
            graph[v].push_back({u,w});
        }
    }

    //colocar oss getters que eu tinha esquecido ai complica

    std::vector<std::vector<std::pair<int,int>>>& get_graph(){
        return graph;
    }

    std::vector<std::vector<int>>& get_reduced_graph(){
        return reduced_graph;
    }

    std::vector<int>& get_switches(){
        return switches;
    }

    int get_rooms(){
        return n_rooms;
    }
};

std::pair<std::vector<int>,std::vector<int>> Dijkstra(std::vector<std::vector<std::pair<int,int>>>& graph,int src){
    int V = graph.size();
    const int INF = 1e9;

    //min heap
    std::priority_queue<
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<std::pair<int,int>>
    > pq;

    //inicilaizando 0 no vetor inicial
    std::vector<int> dist(V,INF);
    std::vector<int> parent(V,-1);
    dist[src] = 0;

    pq.push({0,src});

    //processando a fila
    while(!pq.empty()){
        auto [d,u] = pq.top();
        pq.pop();

        //se nao for menor que o já achado manda matar
        if(d > dist[u] )
            continue;

        for(auto &[v,w] : graph[u]){
            //se o resultado for menor,atualiza
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v],v});
            }
        }
    }
    
    return {dist,parent};
};


std::vector<int> rebuild_path(
    int src,
    int dest,
    std::vector<int>& parent
){

    std::vector<int> path;

    int cur = dest;

    while(cur != -1){

        path.push_back(cur);

        if(cur == src)
            break;

        cur = parent[cur];
    }

    reverse(path.begin(),path.end());

    return path;
}
void build_reduced_graph(std::vector<std::vector<std::pair<int,int>>>& graph,std::vector<std::vector<int>>& reduced_graph,
    std::vector<int>& switches,std::vector<std::vector<std::vector<int>>>& paths,int start,int finish){

    //coisas para a criação do grafo reduzidostar
    std::vector<int> important;

    //colocando vertice inicial
    important.push_back(start);

    //colocando interruptores
    for(auto s: switches)
        important.push_back(s);

    //colocando o vertice final
    important.push_back(finish);

    int n_important = important.size();

    //matriz completa
    reduced_graph.assign(
        n_important,
        std::vector<int>(n_important,0)
    );

    //inicializacao dos caminhos
    paths.assign(
        n_important,
        std::vector<std::vector<int>>(n_important)

    );
    for(int i = 0;i < n_important;i++){
        int src = important[i];

        //Dijkstra no grafo original
        auto [dist,parent] = Dijkstra(graph,src);

        //ligo o inicial e o final com outros vertices importantes ebaaa
        for(int j = 0;j < n_important;j++){

            if(i == j){
                continue;
            }
            
            int destiny = important[j];

            reduced_graph[i][j] = dist[destiny];
            paths[i][j] = rebuild_path(src,destiny,parent);
        }
    }
   
}

/*Minha ideia de manter um Dijkstra não ia dar certo porque continuou gulosa ebtao eu vou apelar pra DP e usar um Held_karp que meu
marido me falou
*/
std::pair<int,std::vector<int>> held_karp(std::vector<std::vector<int>>& reduced_graph){

    int graph_size = reduced_graph.size(); //importante para saber quantos subsets eu vou ter

    int finish = graph_size - 1;

    int subsets = 1 << finish; //numero de subsets possiveis para percorrer

    const int INF = 1e9;

    //o custo minimo de começar no vertice inicial e chegar no final passando pelos interruptores
    std::vector<std::vector<int>> dp(subsets,std::vector<int>(finish,INF));

    //o melhor predecessor baseado na mask
    std::vector<std::vector<int>> parents(subsets,std::vector<int>(finish,-1));

    dp[1][0] = 0; //mascara inicial do vertice inicial

    //looping imenso com as mascaras
    for(int mask = 1;mask < subsets;mask++){
        if((mask & 1 )== 0){
            continue; // passando pelo primeiro vertice
        }
        for(int j = 1;j < finish;j++){
            if((mask & (1 << j)) == 0) continue; //sempre cobrindo o caso do primeiro vertice

            int prevmask = mask ^ (1 << j);
            
            for(int k = 0;k < finish;k++){
                if(k == j) continue;
                if ((prevmask & (1 << k)) == 0)
                    continue;
                if(dp[prevmask][k] == INF) continue;
                int cost = dp[prevmask][k] + reduced_graph[k][j];
                    
                if (cost < dp[mask][j]){
                    dp[mask][j] = cost;
                    parents[mask][j] = k;
                    }
                }
            }
        }
    

    //de fato fazer o caminho minimo
    int fullmask = subsets - 1;
    int mincost = INF;
    int last = -1;

    //conecta ao vertice final
    for(int j = 0;j < finish;j++){
        
        if(dp[fullmask][j] == INF)
            continue;

        int cost = dp[fullmask][j] + reduced_graph[j][finish];

        if(cost < mincost){

            mincost = cost;

            last =  j;
        }

    }

    //reconstruir a merda do caminho
    std::vector <int> tour;

    int mask = fullmask;
    int cur = last;

    while(cur != -1){

        tour.push_back(cur);

        int p = parents[mask][cur];

        mask ^= (1 << cur);

        cur = p;
    }
    
    reverse(tour.begin(), tour.end());

    //adiciona o vertice final
    tour.push_back(finish);

    return {mincost,tour};
}

int main(){

    int N,M,T;

    scanf("%d %d %d",&N,&M,&T);

    Read_Graph g(N,M,T);

    g.read_input();

    int start = 1;
    int finish = N;

    std::vector<std::vector<std::vector<int>>> paths;

    build_reduced_graph(
        g.get_graph(),
        g.get_reduced_graph(),
        g.get_switches(),
        paths,
        start,
        finish
    );

    std::vector<int> important;

    important.push_back(start);

    for(auto s : g.get_switches())
            important.push_back(s);

    important.push_back(finish);

    auto result = held_karp(g.get_reduced_graph());

    int cost = result.first;

    std::vector<int> path = result.second;

    std::vector<int> final_path;

    for(int i =0;i + 1 < path.size();i++){
        int u = path[i];
        int v = path[i + 1];

        std::vector<int>& segment = paths[u][v];

        if(i == 0){
            for(int x : segment)
                final_path.push_back(x);
        }
        else{

            for(int j = 1;j < segment.size();j++ ){
                final_path.push_back(segment[j]);
            }
        }
    }
    
    // primeira linha:
    // custo minimo + quantidade de vertices visitados
    printf("%d %d\n", cost, (int)final_path.size());

    // segunda linha:
    // vertices visitados em ordem
    for(int v : final_path){
        printf("%d ", v);
    }

    printf("\n");
    
    return 0;
}

