#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<vector>
#include<fstream>
#include<algorithm>
#include<limits>

using namespace std;

struct Registro{
    int registro;
    bool congelado;
};

// Funcoes Secundárias
void limpa_tela(){  // seq Ansi para limpar tela
    cout << "\033[2J\033[H";
}

void limpa_buffer() {  // limpa buffer de entrada
    cin.clear();               
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void entrada(int ini, int fim, int *opcao){ // verifica se a entrada esta dentro do intervalo
    int a;
    cout << "Digite uma opção: ";
    cin >> a;
    while (a < ini || a > fim){
        cout << "Digite nomamente, intervalo aceito [" << ini << ',' << fim << ']' << endl;
        cout << "Digite uma opção: ";
        cin >> a;
    }
    *opcao = a;
}

string criaParticao(int indice) { return "particoes/P" + to_string(indice) + ".txt";}

bool comparaRegistros(const Registro &a, const Registro &b) { return a.registro < b.registro; }

bool temDescongelado(const vector<Registro> &memoria){
    for(int i = 0; i < memoria.size(); i++) if (memoria[i].congelado == false) return true;
    return false;
}

void descongela(vector<Registro> &memoria) { for (int i = 0; i < memoria.size(); i++) memoria[i].congelado = false; }

// Funcoes Primarias
void classificacao(){
    limpa_tela();

    cout << "Digite o nome do arquivo a ser classificado: ";
    string nomeArquivo;
    cin >> nomeArquivo;
    
    cout << "Digite a quantidade de registros a ser carregada para a memória: ";
    int tamMemoriaPrincipal;
    cin >> tamMemoriaPrincipal;

    if (tamMemoriaPrincipal < 1) {
        cout << "Quantidade inválida. Digite novamente: ";
        cin >> tamMemoriaPrincipal;
    }
    
    ifstream arquivo(nomeArquivo);
    if(!arquivo){
        cout << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }

    vector<Registro> memPrincipal;
    Registro regAux;

    for(int i = 0; i < tamMemoriaPrincipal; i++) {
        arquivo >> regAux.registro;
        memPrincipal.push_back(regAux);
    }

    int contParticao = 1;

    #ifdef _WIN32
        system("mkdir particoes");
    #else
        system("mkdir -p particoes");
    #endif

    while(!memPrincipal.empty()){  
        bool contemRegistros = false;
        int ultimoAdicionado = numeric_limits<int>::min();
        ofstream particao(criaParticao(contParticao));
        if(!particao){
            cout << "Erro ao criar partição" << endl;
            return;
        }

        while(temDescongelado(memPrincipal)){  
            int menor = -1;
            for (int i = 0; i < memPrincipal.size(); i++){
                if (!memPrincipal[i].congelado){
                    if (menor == -1 || memPrincipal[i].registro < memPrincipal[menor].registro){
                        menor = i;
                    }
                }
            }

            if (menor == -1) break;


            if (memPrincipal[menor].registro >= ultimoAdicionado){
                particao << memPrincipal[menor].registro << " ";
                ultimoAdicionado = memPrincipal[menor].registro;
                contemRegistros = true;
            }

            Registro novo;

            if (arquivo >> novo.registro){
                novo.congelado = (novo.registro < ultimoAdicionado)? true: false;
                memPrincipal[menor] = novo; 
            }else memPrincipal.erase(memPrincipal.begin() + menor);

        }

        particao.close();
        descongela(memPrincipal);
        if (contemRegistros) contParticao++;
        ultimoAdicionado = numeric_limits<int>::min();
    }

    cout << "Partições criadas com sucesso. Aperte enter para votar. ";
    cin.ignore();
    cin.get();
}

void intercalacao(){

}

void gerar(){
    limpa_tela();
    cout << "Digite o nome do arquivo de salvamento: ";
    string nomeArquivo;
    cin >> nomeArquivo;

    cout << "Digite a quantidade de registros (números) no arquivo: ";
    int qtRegistros;
    cin >> qtRegistros;

    cout << "Digite o range dos números:" << endl << "\tinicio: ";
    int inicio;
    cin >> inicio;

    cout << "\tfim: ";
    int fim;
    cin >> fim;

    while (inicio > fim ){
        cout << "Intervalo invalido. Digite novamente o fim: ";
        cin >> fim;
    }

    srand(time(nullptr));

    vector<int> vet(qtRegistros);

    for (int i = 0; i < qtRegistros; i++) vet[i] = rand() % (fim - inicio + 1) + inicio;

    ofstream arquivo(nomeArquivo);
    if (!arquivo){
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    int coluna = sqrt(qtRegistros); 
    for (int i = 0; i < qtRegistros; i++){
        arquivo << vet[i] << " ";
        if ((i+1) % coluna == 0) arquivo << endl;
    }

    arquivo.close();
    cout << "Arquivo criado com sucesso. Aperte enter para voltar.";
    cin.get();
}

int main(){
    bool continuar = true;
    int opcao;

    while (continuar){
        limpa_tela();
        cout << "1 - Classificacao com substituição" << endl
             << "2 - Intercalação balanceada de N caminhos" <<  endl
             << "3 - Gerar registros aleatórios" << endl
             << "4 - Sair" << endl;
        entrada(1,4,&opcao);

        switch (opcao){
            case 1:
            classificacao(); //ok
            break;
            case 2:
            intercalacao();
            break;
            case 3:
            gerar(); //ok
            break;
            case 4:
            continuar = false;
            break;
        }

    }

}