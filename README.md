# 🗃️ Trabalho 02 – Estrutura de Dados (Classificação e Intercalação)

Este repositório contém a implementação de um sistema de **classificação com substituição** e **intercalação ótima** de registros, desenvolvido como parte do Trabalho 2 da disciplina de **Estrutura de Dados(EDA)**.

## 📌 Objetivos

- Implementar o algoritmo de **classificação com substituição** utilizando registros em memória
- Gerar **partições classificadas** a partir de um arquivo de entrada
- Aplicar a **intercalação ótima** entre as partições
- Gerar um arquivo final classificado

## 📥 Entradas do sistema

- Arquivo de texto contendo os registros (números inteiros) a serem classificados
- Parâmetros fornecidos pelo usuário:
  - Nome do arquivo de entrada
  - Quantidade de registros a serem carregados na memória
  - Quantidade de arquivos I/O para intercalação
  - Quantidade e intervalo dos registros a serem gerados

## 🛠️ Funcionalidades

### 🔹 Funções Principais

Estas funções representam os **processos centrais** do programa:

- `classificacao()`  
  Realiza a **classificação com substituição**, gerando as partições ordenadas a partir do arquivo de entrada.

- `intercalacaoOtima()`  
  Executa a **intercalação ótima** entre as partições para produzir o arquivo final ordenado.

- `gerar()`  
  Gera um arquivo com **registros aleatórios**, dentro de um intervalo e quantidade definidos pelo usuário.

### 🔸 Funções Secundárias

Essas funções **auxiliam ou suportam** as operações principais:

- `limpa_tela()`  
  Limpa a tela do terminal utilizando sequência ANSI.

- `limpa_buffer()`  
  Limpa o buffer de entrada padrão (`cin`) para evitar erros de leitura.

- `entrada(int ini, int fim, int *opcao)`  
  Garante que a entrada do usuário está dentro do intervalo `[ini, fim]`.

- `criaParticao(int indice)`  
  Gera o nome de uma partição no formato `"particoes/P{indice}.txt"`.

- `temDescongelado(const vector<Registro> &memoria)`  
  Verifica se ainda existem registros não congelados na memória principal.

- `descongela(vector<Registro> &memoria)`  
  Marca todos os registros da memória principal como não congelados.

- `carregaParticoes(vector<pair<filesystem::path, bool>> &arquivosParticoes, int qtEntrada)`  
  Lê os arquivos de partição ainda não utilizados e os carrega como filas para intercalação.

## 📊 Exemplo de Execução

1. O usuário escolhe gerar registros aleatórios, ou pode utilizar um arquivo próprio.
2. Executa a **classificação com substituição** com memória limitada, o limite é determinado pelo usuário.
3. As partições são geradas na pasta `particoes/`.
4. Executa a **intercalação ótima** com os arquivos gerados, o arquivo de saída é gravado em uma pasta de saída.
5. O resultado final do arquivo de entrada classificado aparece em `arquivo_classificado.txt`.

❗ obs: a pasta do trabalho contém um `arquivo_teste.txt` com 400 registros gerados aleatóriamente, e utilizando 20 espaços na memória principal gera 11 partições.

## 📂 Como executar

1. Compile o código com um compilador C++ (recomenda-se o `g++`):
   ```bash
   g++ trabalho.cpp && ./a.out

## 👨‍🏫 Envolvidos

- Professor: Andre Tavares da Silva
- Estudante: [Rian Valcanaia](https://github.com/RianValcanaia)


## 📅 Curso

- Universidade: Universidade do Estado de Santa Catarina (UDESC)
- Disciplina: Estrutura de Dados
- Semestre: 4º 

## 📄 Arquivo-fonte

O código completo do projeto está disponível no GitHub:  
[https://github.com/RianValcanaia/TC_Classificacao_de_Arquivos](https://github.com/RianValcanaia/TC_Classificacao_de_Arquivos)
