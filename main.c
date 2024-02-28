#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_LETRAS 26
#define ASCII_A 65 
#define LINE_LENGTH 100 //Quantidade maxima de caracter por linhas
#define ARQ_LINES 10 //Quantidade de linhas do arquivo
#define TAM_SENHA 5 //Quantidade de caracter por senha


//Struct responsavel por guardar as senhas dos arquivos

struct password
{
    char senha[5];
}password;

//Nota: Tive que mudar o nome, pois estava dando conflito com alguma função do unistd.h
//Nota: Adicionei o caractere '\0' para str_result virar uma string completa

char* encrypte(const char* str, int tamanho) {
    char* str_result = (char*) malloc(sizeof(char)*tamanho);
    str_result[4] = '\0';
    for (int i = 0; i < tamanho; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            int chave = str[i] - ASCII_A;
            str_result[i] = (str[i] - ASCII_A + chave) % NUM_LETRAS + ASCII_A;
        }
    }
    return str_result;
}

//Abre um novo arquivo para escrita das senhas

void escreve_arq(char *senha, char *crypto, char* argv){
    char dest[30] = "quebradas_";
    FILE * arq;
    strcat(dest,argv);
    arq = fopen(dest,"a");

    if(arq == NULL){
        printf("Erro ao escrever no arquivo.\n");
        exit(1);
    }

    fprintf(arq, "Senha criptografada: %s - Senha quebrada: %s\n", crypto, senha);
    fclose(arq);
}

//gera as senhas, envia para o encripadador e em seguida compara as strings

void gera_senha(struct password * crypt, int pos, char * argv){

    char * senha = (char*) malloc(sizeof(char)*TAM_SENHA); //Alocando vetor de char para gerar as senhas
    char * str_result; //Vai receber a endereço da senha criptografada
    int result; // Variavel usada na comparação das senhas

    //Contruindo a primeira string "AAAA"

    for(int i = 0; i < TAM_SENHA - 2; ++i){
        senha[i] = 'A';
    }
    senha[TAM_SENHA - 1] = '\0';

    //4 Fors responsáveis por gerar todas as combinações possíveis entre "AAAA" e "ZZZZ"

    for(int first = ASCII_A; first < ASCII_A + NUM_LETRAS; ++first){ //Variando o primeiro caracter
        senha[0] = first;
        for(int second = ASCII_A; second < ASCII_A + NUM_LETRAS; ++second){ //Variando o segundo caracter
            senha[1] = second;
            for(int third = ASCII_A; third < ASCII_A + NUM_LETRAS; ++third){ //Variando o terceiro caracter
                senha[2] = third;
                for(int fourth = ASCII_A; fourth < ASCII_A + NUM_LETRAS; ++fourth){ //Variando o quarto caracter
                    senha[3] = fourth;

                    str_result = encrypte(senha, TAM_SENHA); //Recebendo senhas encriptografadas
                    result = strcmp(str_result, crypt[pos].senha); //Comparando com as senhas do arquivo

                    if(result == 0){
                        escreve_arq(senha, str_result, argv); //Escrevendo no arquivo
                        return;
                    }
                }
            }
        }
    }
}

//Ler o arquivos com as senhas e retorna um ponteiro para uma estrutura que contem as senhas com criptografia

struct password * ler_arquivo(char *argv){
    struct password *senha;
    senha = (struct password *) malloc(sizeof(struct password) * ARQ_LINES); //Alocando structs parar guardar senhas do arquivo
    FILE *arq;
    arq = fopen(argv, "r");
    char linha[LINE_LENGTH]; //Salva senha por senha
    int contador = 0;

    if(arq == NULL){
        printf("Não abriu o arquivo.\n");
        exit(1);
    }

    //Escrevendo e guradando no struct

    while (fgets(linha, LINE_LENGTH, arq) != NULL && contador < ARQ_LINES)
    {
        sscanf(linha, "%s", senha[contador].senha);
        contador++;
    }

    fclose(arq);

    return senha;
}


int main(int argc, char **argv){
    int fd[10][2];
    pid_t pid;
    char buffer[100];

//Criando todos o pipes

    for(int i = 1; i < argc; ++i){
        if(pipe(fd[i]) == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

//Criando os processos filhos

    for(int i = 1; i < argc; ++i){
        printf("Processos criados: %d\n", i);

        pid = fork();
        if(pid < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(pid == 0){  //Garante que o processo filho vai sair do laço sem criar mais processos
            close(fd[i][1]);
            read(fd[i][0], buffer, sizeof(buffer));
            printf("Processo filho recebeu: %s\n", buffer);
            close(fd[i][0]);
            
            for(int j = 0; j < 10; j++){
            gera_senha(ler_arquivo(argv[i]), j, argv[i]);
            }

            break;
        }else{ //Processo pai vai enviar o endereço do aquivo para o processo filho e em seguida voltar o laço
            close(fd[i][0]);
            write(fd[i][1], argv[i], strlen(argv[i]));
            printf("Processo pai mandou: %s\n", argv[i]);
            close(fd[i][1]);
        }
    }


    return 0;
}