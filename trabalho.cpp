#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<vector>
#include<fstream>
#include<algorithm>
#include<limits>
#include<queue>
#include<filesystem>

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

vector<queue<int>> carregaParticoes(vector<pair<filesystem::path, bool>> &arquivosParticoes, int qtEntrada){
    vector<queue<int>> particoes;
    int aux;
    int cont = 0;

    for(auto &objeto: arquivosParticoes){
        if (cont >= qtEntrada) break;

        if (!objeto.second){
            ifstream arquivo(objeto.first);
            if(!arquivo){
                cout << "Erro ao abrir a partição. Aperte enter para voltar." << endl;
                cin.ignore();
                cin.get();
                return {};
            }


            queue<int> fila;
            int valor;
            while (arquivo >> valor) fila.push(valor);

            particoes.push_back(fila);
            objeto.second = true;
            cont++;
        }
    }
    return particoes;
}

// Funcoes Primarias
void classificacao(){
    limpa_tela();

    cout << "Digite o nome do arquivo a ser classificado: ";
    string nomeArquivo;
    cin >> nomeArquivo;
    
    cout << "Digite a quantidade de registros a ser carregada para a memória: ";
    int tamMemoriaPrincipal;
    cin >> tamMemoriaPrincipal;

    while (tamMemoriaPrincipal < 1) {
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

    //apaga particao caso existir
    if (filesystem::exists("particoes")) {
        for (const auto &entry : filesystem::directory_iterator("particoes")) {
            if (filesystem::is_regular_file(entry)) {
                filesystem::remove(entry);
            }
        }
    }else {
        #ifdef _WIN32
            system("mkdir particoes");
        #else
            system("mkdir -p particoes");
        #endif
    }


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

void intercalacaoOtima(){
    limpa_tela();
    
    cout << "Digite a quantidade de arquivos I/O:" << endl;
    int qtArq;
    cin >> qtArq;

    while (qtArq < 3){
        cout << "Quantidade deve ser maior que 2. Digite novamente: ";
        cin >> qtArq;
    }

    if (!filesystem::exists("particoes")){
        cout << "Pasta de partições não encontrada. Aperte enter para voltar. " << endl;
        cin.ignore();
        cin.get();
        return;
    }

    int qtEntrada = qtArq-1;
    int qtSaida = 1;

    vector<pair<filesystem::path, bool>> arquivosParticoes;


    for (const auto &objeto: filesystem::directory_iterator("particoes")){
        if(objeto.is_regular_file()){
            arquivosParticoes.push_back({objeto.path(), false});
        }
    }

    //apaga saida se existir
    if (filesystem::exists("saida")) {
        for (const auto &entry : filesystem::directory_iterator("saida")) {
            if (filesystem::is_regular_file(entry)) filesystem::remove(entry);
        }
    } else {
        #ifdef _WIN32
            system("mkdir saida");
        #else
            system("mkdir -p saida");
        #endif
    }

    int contSaida = 1;
    while (count_if(arquivosParticoes.begin(), arquivosParticoes.end(), [](const pair<filesystem::path, bool> &p){return !p.second;}) > 1){
        vector<queue<int>> particoes = carregaParticoes(arquivosParticoes, qtEntrada);
        cout << "tamanho particoes:" << particoes.size() << endl;
        if (particoes.empty()) return;
        
        string nomeSaida = "saida/S" + to_string(contSaida) + ".txt";
        ofstream saida(nomeSaida);

        //enquando houver uma fila não vazia
        while(any_of(particoes.begin(),particoes.end(), [](const queue<int> &arq){return !arq.empty();})){
            int menor = -1;

            for (int i = 0; i < particoes.size(); i++){
                if (!particoes[i].empty()){
                    if (menor == -1 || particoes[i].front() < particoes[menor].front()) menor = i;
                }
            }

            if (menor != -1){
                saida << particoes[menor].front() << " ";
                particoes[menor].pop();
            }
        }

        arquivosParticoes.push_back({filesystem::path(nomeSaida),false});
        contSaida++;

    }

    filesystem::copy_file("saida/S" + to_string(contSaida-1) + ".txt", "saida_final.txt", filesystem::copy_options::overwrite_existing);

    cout << "Arquivo intercalado com sucesso. Aperte enter para voltar. ";
    cin.ignore();
    cin.get();
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
             << "2 - Intercalação Ótima" <<  endl
             << "3 - Gerar registros aleatórios" << endl
             << "4 - Sair" << endl;
        entrada(1,4,&opcao);

        switch (opcao){
            case 1:
            classificacao(); //ok
            break;
            case 2:
            intercalacaoOtima(); //ok
            break;
            case 3:
            gerar(); //ok
            break;
            case 4:
            continuar = false;
            break;
        }
    }

    limpa_tela();

}