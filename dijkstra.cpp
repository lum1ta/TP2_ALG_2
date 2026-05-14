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