#include <iostream>
#include <cstdio>
#include <fstream>//LER ARQUIVO
#include <stdlib.h>

#include<map>
#include<queue>

using namespace std;


const int TAMANHO = 9;
const int ESTRATEGIA = 1;// 0 - backtracking; 1 - MVR; 2 - VERIFICAÇÃO PRÉVIA
const int VERIFICACAO_ADIANTE = 1;

bool retrocessoRecursivo(int** jogo);
int* proximoVazio(int** jogo);
bool consistente(int** jogo, int linha,int coluna);
bool verificarLinha(int** jogo, int linha,int coluna);
bool verificarColuna(int** jogo, int linha,int coluna);
bool verificarBloco(int** jogo, int linha,int coluna);
void mostrarJogo(int** jogo);
bool atribuicaoCompleta(int** jogo);

int* proximoValorMenosRestritivo(int** jogo);
int nValoresValidos(int** jogo,int linha,int coluna);
int calcularGrau(int** jogo, int linha,int coluna);


int** verificacoes = new int*[81];//Valor 0: não tem restrição; Valor 1:tem restrição

void desfazerAtualizacoes(int* atualizacoes,int valor){
    valor = valor - 1;
    for(int i = 0; i < 21; i++){
        if( atualizacoes[i] != -1){
            verificacoes[atualizacoes[i]][valor] = 0;
        }
    }
    delete(atualizacoes);
}

int* atualizarVerificacoes(int linha, int coluna,int valor){

    int posicao = linha * 9;
    int bLinha = linha/3;
    int bColuna = coluna/3;
    int cont = 0;

    int* atualizados = new int[21];

    for(int i = 0; i < 21; i++){
        atualizados[i] = -1;
    }

    valor = valor - 1;

    for(int i = posicao; i < posicao+9; i++){
        if(verificacoes[i][valor] == 0){
            verificacoes[i][valor] = 1;
            atualizados[cont] = i;
            cont++;
        }
    }

    for(int i = coluna; i < 81; i+=9){
        if(verificacoes[i][valor] == 0){
            verificacoes[i][valor] = 1;
            atualizados[cont] = i;
            cont++;
        }
    }

    posicao = (bLinha *27) + (bColuna * 3);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(verificacoes[posicao+(9*i)+j][valor] == 0){
                verificacoes[posicao+(9*i)+j][valor] = 1;
                atualizados[cont] = posicao+(9*i)+j;
                cont++;
            }
        }
    }

    for(int i = 0;i < 81; i++){
        for(int j = 0; j < 9; j++){
            if(verificacoes[i][j] == 0){
                return atualizados;
            }
        }
    }
    /*for(int i = 0;i < 81; i++){
        cout << i << " ) ";
        for(int j = 0; j < 9; j++){
            cout << verificacoes[i][j] << " ";
        }
        cout << endl;
    }*/

    return false;
}

int main(){

    int nJogos;
    int totalMatrizVerificacaoPrevia = TAMANHO*TAMANHO;
    ifstream infile("teste2.txt");

    //cin >> nJogos;
    infile >> nJogos;

    int** jogo = new int*[TAMANHO];

    for(int i = 0; i < TAMANHO; i++){
        jogo[i] = new int[TAMANHO];
    }



    //atualizarVerificacoes(3,3,9);

    //return 0;

    for(int n = 0; n < nJogos; n++){
        cout << "N: "<< n<<endl;

        for(int i = 0; i < TAMANHO; i++){//LINHA
            for(int j = 0; j < TAMANHO; j++){//COLUNA
                //cin >> jogo[i][j];
                if(VERIFICACAO_ADIANTE == 1){
                    for(int ii = 0; ii < 81; ii++){
                        verificacoes[ii] = new int[TAMANHO]{0,0,0,0,0,0,0,0,0};
                    }
                }
                infile >> jogo[i][j];
            }
        }
        mostrarJogo(jogo);

        //cout << nValoresValidos(jogo,0,1);
        retrocessoRecursivo(jogo);
        mostrarJogo(jogo);
    }

    return 0;
}



int* proximoValorMenosRestritivo(int** jogo){
    int minimoValoresValidos = 10;
    int maximoGrau = 0;
    int valorTemp;
    int grauTemp;
    int* posicoes =  new int[2];

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(jogo[i][j] == 0){
                valorTemp = nValoresValidos(jogo,i,j);
                if(valorTemp < minimoValoresValidos){
                    minimoValoresValidos = valorTemp;
                    maximoGrau = calcularGrau(jogo,i,j);
                    posicoes[0] = i;
                    posicoes[1] = j;
                }else if(valorTemp == minimoValoresValidos){
                    grauTemp = calcularGrau(jogo,i,j);
                    if(maximoGrau < grauTemp){
                        minimoValoresValidos = valorTemp;
                        maximoGrau = grauTemp;
                        posicoes[0] = i;
                        posicoes[1] = j;
                    }
                }
            }
        }
    }

    return posicoes;
}

int nValoresValidos(int** jogo,int linha,int coluna){
    int total = 0;
    for(int i = 1; i <= 9; i++){
        jogo[linha][coluna] = i;
        if(consistente(jogo,linha,coluna)){
            //cout << i << endl;
            total++;
        }
        jogo[linha][coluna] = 0;
    }
    return total;
}

int calcularGrau(int** jogo, int linha,int coluna){
    int total = 0;
    int auxLinha = linha / 3;
    int auxColuna = coluna / 3;
    int x;
    int y;

    for(int i = 0; i < 9; i++){
        if(jogo[linha][i] == 0 && i != (3*auxColuna) && i != (3*auxColuna+1) && i != (3*auxColuna+2)){
            total++;
        }
    }

    for(int i = 0; i < 9; i++){
        if(jogo[i][coluna] == 0 && i != (3*auxLinha) && i != (3*auxLinha+1) && i != (3*auxLinha+2)){
            total++;
        }
    }

    for(int i = 0 ; i < 3; i++){
        x = 3 * auxLinha + i;
        for(int j = 0; j < 3; j++){
            y = 3 * auxColuna + j;
            if(jogo[x][y] == 0){
                total++;
            }
        }
    }

    return total;
}

bool retrocessoRecursivo(int** jogo){
    int x;
    int y;
    int* posicao = new int[2];
    bool resultado = true;
    bool verificado = true;
    int* atualizacoes;

    if(atribuicaoCompleta(jogo)){
        cout << "Completa" << endl;
        return true;
    }

    if(ESTRATEGIA == 0){
        posicao = proximoVazio(jogo);
    }else if(ESTRATEGIA == 1){
        posicao = proximoValorMenosRestritivo(jogo);
    }
    x = posicao[0];
    y = posicao[1];

    free(posicao);
    for(int i = 1; i <= 9; i++){
        jogo[x][y] = i;

        if(VERIFICACAO_ADIANTE == 1){
            atualizacoes = atualizarVerificacoes(x,y,i);
            if(atualizacoes != 0){
                verificado = true;
            }else{
                verificado = false;
            }
        }else{
            verificado = true;
        }

        if(consistente(jogo,x,y) && verificado){
            //cout << "Consistente" << endl;
            resultado = retrocessoRecursivo(jogo);
            if(resultado != false){
                return true;
            }
        }
        if(VERIFICACAO_ADIANTE == 1){
            desfazerAtualizacoes(atualizacoes,i);
        }
    }
    jogo[x][y] = 0;
    return false;
}

int* proximoVazio(int** jogo){

    int* posicoes =  new int[2];

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
                if(jogo[i][j] == 0){
                    posicoes[0] = i;
                    posicoes[1] = j;
                    return posicoes;
                }
        }
    }
    return posicoes;
}

bool consistente(int** jogo, int linha,int coluna){
    if(verificarLinha(jogo,linha,coluna) && verificarColuna(jogo,linha,coluna) && verificarBloco(jogo,linha,coluna)){
        return true;
    }
    return false;
}

bool verificarColuna(int** jogo, int linha,int coluna){
    for(int i = 0; i < 9; i++){
        if(i != linha && jogo[linha][coluna] == jogo[i][coluna]){
            return false;
        }
    }
    return true;
}

bool verificarLinha(int** jogo, int linha,int coluna){
    for(int i = 0; i < 9; i++){
        if(i != coluna && jogo[linha][coluna] == jogo[linha][i]){
            return false;
        }
    }
    return true;
}

bool verificarBloco(int** jogo, int linha,int coluna){
    int l = linha / 3;
    int c = coluna / 3;

    l = l * 3;
    c = c * 3;

    for(int i = l; i < (l+3); i++){
        for(int j = c; j < (c+3); j++){
            if((i != linha || j != coluna) && jogo[linha][coluna] == jogo[i][j]){
                return false;
            }
        }
    }

    return true;
}

bool atribuicaoCompleta(int** jogo){
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(jogo[i][j] == 0){
                return false;
            }
        }
    }
    return true;
}

void mostrarJogo(int** jogo){
    cout << "\tJOGO\n";

    for(int i = 0; i < TAMANHO; i++){//LINHA
        for(int j = 0; j < TAMANHO; j++){//COLUNA

            cout << " " << jogo[i][j];
            if((j+1) % 3 == 0){
                cout << "| ";
            }
        }
        cout << "\n";
        if((i+1) % 3 == 0){
            cout << " ";
            for(int l = 0; l < 21; l++){
                cout << "-";
            }
            cout << "\n";
        }
    }

}
