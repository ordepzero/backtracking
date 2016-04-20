#include <iostream>
#include <cstdio>
#include <fstream>//LER ARQUIVO
#include <stdlib.h>

using namespace std;


const int TAMANHO = 9;
const int ESTRATEGIA = 1;// 0 - backtracking; 1 - MVR; 2 - VERIFICAÇÃO PRÉVIA

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

int main(){

    int nJogos;
    ifstream infile("teste.txt");

    //cin >> nJogos;
    nJogos = 1;

    int** jogo = new int*[TAMANHO];

    //cout << nJogos << endl;

    for(int i = 0; i < TAMANHO; i++){
        jogo[i] = new int[TAMANHO];
    }

    for(int n = 0; n < nJogos; n++){
        cout << "N: "<< n<<endl;

        for(int i = 0; i < TAMANHO; i++){//LINHA
            for(int j = 0; j < TAMANHO; j++){//COLUNA
                //cin >> jogo[i][j];
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
    int valorTemp;
    int* posicoes =  new int[2];

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(jogo[i][j] == 0){
                valorTemp = nValoresValidos(jogo,i,j);
                if(valorTemp < minimoValoresValidos){
                    minimoValoresValidos = valorTemp;
                    posicoes[0] = i;
                    posicoes[1] = j;
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

bool retrocessoRecursivo(int** jogo){
    int x;
    int y;
    int* posicao = new int[2];
    bool resultado = true;

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

        if(consistente(jogo,x,y)){
            //cout << "Consistente" << endl;
            resultado = retrocessoRecursivo(jogo);
            if(resultado != false){
                return true;
            }
        }
    }
    jogo[x][y] = 0;
    return false;
}



bool consistente(int** jogo, int linha,int coluna){
    if(verificarLinha(jogo,linha,coluna) && verificarColuna(jogo,linha,coluna) && verificarBloco(jogo,linha,coluna)){
        return true;
    }
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
