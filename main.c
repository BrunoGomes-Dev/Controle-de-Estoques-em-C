#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>

typedef struct {
    char word[101];
    int quan;
} Pair;

typedef struct {
    Pair *vetor;
    int cap;
    int size;
} vector;

vector create_vector(){
    vector v;
    v.vetor = (Pair*)malloc(sizeof(Pair)*4);
    v.cap = 4;
    v.size = 0;

    return v;
}

void add_vector(vector *v, Pair item){
    if(v->size >= v->cap){
        v->cap*=2;
        v->vetor = (Pair*)realloc(v->vetor, sizeof(Pair)*v->cap);
    }

    v->vetor[v->size] = item;
    v->size++;
}

void destroy_vector(vector *v){
    free(v->vetor);
    v->vetor = NULL;
}

bool new_operation(){
    printf("Deseja realizar outra operação? (s/n):\n");

    while(true){
        char a;
        scanf(" %c", &a);

        if(a == 's') return true;
        else if(a == 'n') return false;
        else printf("Insira uma opção válida!\n");
    }
}

void add_item(char op, vector *v){
    FILE *arquivo;
    char op_char[2];
    int i;

    op_char[0] = op;
    op_char[1] = '\0';

    arquivo = fopen("estoque.txt", op_char);

    for(i = 0; i < v->size; i++) fprintf(arquivo, "%s\n%d\n", v->vetor[i].word, v->vetor[i].quan);
    
    
    fclose(arquivo);
}

//operations
void add_item_user(){
    vector vetor = create_vector();
    Pair pair;

    printf("Digite o nome do item: ");
    scanf(" %s", pair.word);

    printf("Digite a quantidade: ");
    scanf("%d", &pair.quan);

    add_vector(&vetor, pair);

    add_item('a', &vetor);

    destroy_vector(&vetor);

    printf("Item adicionado com sucesso!\n");
}

void remove_item() {
    FILE *arquivo;
    vector v = create_vector();
    Pair pair;
    char word[101];
    int quan = 0, i;
    bool found = true;

    arquivo = fopen("estoque.txt", "r");

    while(fscanf(arquivo, " %s%d", pair.word, &pair.quan) == 2) add_vector(&v, pair);
    fclose(arquivo);
    
    printf("Digite o nome do item: ");
    scanf(" %s", word);
    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &quan);
    
    for(i = 0; i < v.size; i++){
        if(!found) v.vetor[i] = v.vetor[i+1];
        else if(strcmp(v.vetor[i].word, word) == 0){
            found = false;
            if(v.vetor[i].quan < quan){
                 printf("Estoque insuficiente. Quantidade disponível: %d\n", v.vetor[i].quan);
                 return;
            }
            else if(quan < v.vetor[i].quan) {
                v.vetor[i].quan-=quan;
                printf("Quantidade atualizada com sucesso!\n");
                break;
            }
            else {
                v.size-=1;
                i--;
                printf("Item removido do estoque!\n");
            }
        }
    }

    if(found) printf("Item não encontrado.\n");
    else add_item('w', &v);

    destroy_vector(&v);
}

void list_itens(){
    FILE *arquivo;
    char word[101];
    int quan, i = 0;

    arquivo = fopen("estoque.txt","r");
    
    printf("\n=============================\nEstoque Atual\n=============================\n");

    while(fscanf(arquivo, "%s%d", word, &quan) == 2){
        printf("Nome: %s\n", word);
        printf("Quantidade: %d\n\n", quan);
        i++;
    }

    if(i == 0) printf("O estoque está vazio.\n");

    fclose(arquivo);
}

bool program(){
    char c;

    printf("================================\nControle de Estoque\n================================\n");
    
    while(true){
        printf("Selecione uma opção:\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");
        scanf(" %c", &c);

        if(c == '1') add_item_user();
        else if(c == '2') remove_item();
        else if(c == '3') list_itens();
        else if(c == '4') return 0;
        else {
            printf("Opção inválida! Tente novamente.\n");
            continue;
        }
        break;
    }

    return new_operation();
}

int main(){
    setlocale(LC_ALL, "");

    while(program()){}

    printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");

    return 0;
}