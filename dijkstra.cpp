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

    public:
    //construtor
    Read_Graph(int room,int hallway,int interruptor)
        //as salas são numeradas de 1 a N entao a primeira posição do vetor fica inutilizada
        : n_rooms(room),n_hallway(hallway),n_interruptor(interruptor),graph(room + 1){}

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
};

/*
        Um dijkstra seco não é capaz de resolver então eu fui atras de uma solução que mais palpavel que ainda tivesse 
    a ver com o algoritmo porque era minha principal referencia de resolução de caminho minimo
    O problema do dijkstra nesse cenáario é ficar preso ao valor minimo de um vertice,o que não importa se no caminho final
    eu chego até esse esse vertice com um caminho mai pesado mas com mais interruptores ativado.

        Então o objetivo aqui é chegar em cada vértice com o maior número de interruptores ativados.Para isso,minha intenção é usar o dijkstra
    com uma modificação na fila de prioridade e na leitura de cada vertice:cada vertice ter armazenado além de seu peso quantos interruptores
    eu  tinha ativado quando eu cheguei nela e isso por meio de um estado ne,ou seja,cada vertice pode ter no maximo 14 eestados contando com o
    com todos ou nenhum interruptor ativado.Para mim faz sentido com o minimo de alteração possível em um conceito que eu já conheço

        Antes de tentar implementar eu imagino que seja apenas uma alteração na fila de prioridade no dijkstra mas eu ainda não sei como 7
    manipular a informação de onde estão os interruptores nesse cenário

*/

std::vector<int> Dijkstra(std::vector<std::vector<std::pair<int,int>>>& graph,int src ){
    int V = graph.size();

    //min heap
    std::priority_queue<
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<std::pair<int,int>>
    > pq;

    //inicilaizando 0 no vetor inicial
    std::vector<int> dist(V,INT_MAX);
    dist[src] = 0;
    pq.emplace(0,src);

    //processando a fila
    while(!pq.empty()){
        auto top = pq.top();
        pq.pop();

        int d = top.first;
        int u = top.second;

        //se nao for menor que o já achado manda matar
        if(d > dist[u] )
            continue;

        for(auto &p : graph[u]){
            int v = p.first;
            int w = p.second;

            //se o resultado for menor,atualiza
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.emplace(v,dist[v]);
            }
        }
    }
    
    return dist;
};
