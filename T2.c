#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição de constantes que serão úteis depois
#define MAX_LINE_SIZE 50   // Tamanho máximo da linha, considerando uma linha de senha no arquivo
#define MAX_REGISTERS 1000 // Máximo de registros, definidos no pdf da tarefa

// Estrutura para um registro
typedef struct
{
    int id;
    char login[15];
    char password[30];
    char gender;
    double salary;
} Register;

// Estrutura auxiliar para a definição dos dados na busca e deleção
typedef union
{
    int id;
    char login[15];
    char password[30];
    char gender;
    double salary;
} KeyValue;

// Função para leitura das linhas dentro do arquivo
void readRegister(FILE *base, Register *list)
{
    // Define o tamanho de uma linha máxima na leitura
    char line[MAX_LINE_SIZE];

    // Percorre as linhas do arquivo
    while (fgets(line, sizeof(line), base) != NULL)
    {
        // Caso o objeto chegue ao fim, saimos da leitura
        if (strstr(line, "}"))
        {
            return;
        }

        // Armazenamos os dados coletados dentro de um indice da lista de registros
        if (strstr(line, "\"id\""))
        {
            sscanf(line, " \"id\": %d,", &list->id);
        }
        else if (strstr(line, "\"login\""))
        {
            sscanf(line, " \"login\": \"%[^\"]\",", list->login);
        }
        else if (strstr(line, "\"password\""))
        {
            sscanf(line, " \"password\": \"%[^\"]\",", list->password);
        }
        else if (strstr(line, "\"gender\""))
        {
            sscanf(line, " \"gender\": \"%c\",", &list->gender);
        }
        else if (strstr(line, "\"salary\""))
        {
            sscanf(line, " \"salary\": %lf", &list->salary);
        }
    }
}

void openNreadFile(Register *list, int *busyList)
{
    // Recebe o nome do arquivo
    char fileName[14];
    if (fgets(fileName, sizeof(fileName), stdin) == NULL)
    {
        printf("Erro na leitura");
        exit(EXIT_FAILURE);
    }
    fileName[strcspn(fileName, "\n")] = '\0'; // Substituindo o \n (enter) pelo \0 (fim da string)

    // Abertura do arquivo
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Nao achei mano");
        exit(EXIT_FAILURE);
    }

    // Variáveis auxiliares
    int nRegisters = 0;       // Total de registros lidos
    char line[MAX_LINE_SIZE]; // Linha para leitura do arquivo

    // Percorre o arquivo até o fim
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Chama a função de leitura de dados quando encontrar a abertura de um objeto
        if (strstr(line, "{"))
        {
            readRegister(file, &list[nRegisters]);
            busyList[nRegisters] = 1; // Aciona o indice como ocupado
            nRegisters++;             // Incrementa o número de registros lidos
        }
    }
    fclose(file);                                    // Fecha o arquivo
    printf("%d registro(s) lido(s).\n", nRegisters); // Exibe o total de registros lidos
}

// Função que procura por um valor nos registros
int searchFor(int keyCode, KeyValue value, Register *list, int *resultIndexes, int *busyList)
{
    // Essa função recebe 3 ponteiros
    // 1- lista de registros que precisa ser acessada
    // 2- lista de destino para armazenar os indices que correspondem a busca
    // 3- lista de indices ocupados da lista de registro

    int n = 0; // Número de registros com o valor encontrados
    // Percorremos todos os registros e verificamos sua ocupação
    for (int i = 0; i < MAX_REGISTERS; i++)
    {
        // Só lê campos ocupados
        if (busyList[i] == 1)
        {
            // Dependendo do código de comando recebido, interpreta o dado de uma forma diferente.
            // Por isso usa-se uma union, para economizar memória e obter versatilidade na atribuição do valor
            switch (keyCode)
            {
            // Procura o valor em todos os campos da struct
            // Caso encontre o valor, guarda o indice do elemento na lista de resultados da busca
            case 1:
                if (list[i].id == value.id)
                    resultIndexes[n++] = i; // n++ permite usar o valor atual de n e depois incrementá-lo. Evita o uso de linhas separadas para essa operação
                break;
            case 2:
                if (strcmp(list[i].login, value.login) == 0)
                    resultIndexes[n++] = i;
                break;
            case 3:
                if (strcmp(list[i].password, value.password) == 0)
                    resultIndexes[n++] = i;
                break;
            case 4:
                if (list[i].gender == value.gender)
                    resultIndexes[n++] = i;
                break;
            case 5:
                if (list[i].salary == value.salary)
                    resultIndexes[n++] = i;
                break;
            }
        }
    }
    return n; // Retorna o número de indices encontrados
}

// Função que recebe o tipo de comando e interpreta seu valor
int interpreteDataLine(char command[100], Register *list, int *destinyList, int *busyItens)
{
    char key[9];                             // Variável auxiliar para a chave da struct que a função buscará
    KeyValue searchValue;                    // Variável de apoio para armazenar o valor da chave
    sscanf(command, "%*d \"%8[^\"]\"", key); // Armazena o valor da chave

    // Interpreta a chave e armazena o valor procurado
    // Quando encontra a chave correspondente, chama a função para procurar o campo com o valor lido
    if (strcmp(key, "id") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %d", &searchValue.id);
        return searchFor(1, searchValue, list, destinyList, busyItens); // Chama a função Search e já retorna seu valor - o número de registros correspondentes
        // Note que a função search recebe um ponteiro para a lista de registros correspondentes, assim as alterações  feitas na lista não são perdidas.
    }
    else if (strcmp(key, "login") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" \"%[^\"]\"", searchValue.login);
        return searchFor(2, searchValue, list, destinyList, busyItens);
    }
    else if (strcmp(key, "password") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" \"%[^\"]\"", searchValue.password);
        return searchFor(3, searchValue, list, destinyList, busyItens);
    }
    else if (strcmp(key, "gender") == 0)
    {
        char tmp[2]; // Variável auxiliar para evitar erros de leitura
        sscanf(command, "%*d \"%*[^\"]\" \"%1[^\"]\"", tmp);
        searchValue.gender = tmp[0]; // Ao armazenar apenas o primeiro índice, é garantido que apenas será armazenado 1 caractere, evitando o estouro de armazenamento
        return searchFor(4, searchValue, list, destinyList, busyItens);
    }
    else if (strcmp(key, "salary") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %lf", &searchValue.salary);
        return searchFor(5, searchValue, list, destinyList, busyItens);
    }
    return 0; // Retorna 0 caso não encontre nenhum correspondente de chave ou valor
}

int main()
{
    // Inicia uma lista de registros e uma lista que controla a ocupação da lista de registros
    Register *regList = (Register *)calloc(MAX_REGISTERS, sizeof(Register));
    int busyIndexes[MAX_REGISTERS] = {0};

    openNreadFile(regList, busyIndexes); // Lê o arquivo solicitado e armazena os registros

    // Leitura de comandos
    char line[100]; // Variável de apoio para leitura. Considera o tamanho máximo, ou seja, uma linha de inserção
    // Enquanto houver linhas para ler, esse while é executado
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        // Considera somente o primeiro caractere, ou seja, o comando
        switch (line[0])
        {
        // Função para inserção
        case '1':
        {
            int inserted = 0; // Flag para detectar inserção
            // Percorre a lista de ocupação para verificar se tem espaço na lista
            for (int i = 0; i < MAX_REGISTERS; i++)
            {
                // Se encontrar um espaço vago, insere o registro nesse espaço
                if (busyIndexes[i] == 0)
                {
                    char tmp[2]; // Variável auxiliar para evitar estrouro de buffer
                    sscanf(line, "%*d %d \"%[^\"]\" \"%[^\"]\" \"%1[^\"]\" %lf", &regList[i].id, regList[i].login, regList[i].password, tmp, &regList[i].salary);
                    regList[i].gender = tmp[0]; // Usar esse auxilar garante que será armazenado apenas um caractere
                    busyIndexes[i] = 1;         // Aciona o indice como ocupado
                    inserted = 1;               // Aciona a flag de inserção
                    printf("Registro inserido.\n");
                    break;
                }
            }
            // Caso nada tenha sido inserido, quer dizer que não existe espaço na lista
            if (inserted == 0)
            {
                printf("Sem espaço para inserção.\n");
            }

            break;
        }
        // Função para busca
        case '2':
        {
            int resultsIndexes[MAX_REGISTERS]; // Lista de indices correspondentes a busca
            // A função abaixo lê a linha e identifica a tag (id, login, salary etc.) que deve buscar e qual valor procurar. Retorna o número de ocorrências encontradas
            int n = interpreteDataLine(line, regList, resultsIndexes, busyIndexes); // Número de indices correspondentes
            // Se houverem registros correspondentes, exibe-os formatados
            if (n > 0)
            {
                for (int i = 0; i < n; i++)
                {
                    int r = resultsIndexes[i]; // Armazena o indice atual da lista de correspondencias, além de facilitar a leitura
                    printf("{\n");
                    printf("    \"id\": %d,\n", regList[r].id);
                    printf("    \"login\": \"%s\",\n", regList[r].login);
                    printf("    \"password\": \"%s\",\n", regList[r].password);
                    printf("    \"gender\": \"%c\",\n", regList[r].gender);
                    printf("    \"salary\": %.2lf\n", regList[r].salary);
                    printf("}\n");
                }
            }
            // Se não houver correspondentes, retorna uma mensagem
            else
            {
                printf("Nada encontrado.\n");
            }
            break;
        }
        // Função para remoção
        case '3':
        {

            // A função de remoção usa a função de busca para procurar as correspondências e as desocupa
            // Para facilitar a lógica, usa-se a lista de ocupação para verificar os indices
            // Mesmo que existam dados, um indice só está ocupado se a lista de ocupação estiver marcada como ocupada (1)

            int resultsIndexes[MAX_REGISTERS]; // Lista de indices correspondentes a busca
            // Recebe o número de correspondecias de acordo com o comando
            int n = interpreteDataLine(line, regList, resultsIndexes, busyIndexes);
            // Se houverem correspondências, então as remove da lista de ocupação
            if (n > 0)
            {
                for (int j = 0; j < n; j++)
                {
                    // Verifica se o índice recebido na lista está ocupado
                    if (busyIndexes[resultsIndexes[j]] == 1)
                    {
                        busyIndexes[resultsIndexes[j]] = 0; // Desocupa o índice
                        // Aqui, mesmo que os dados continuem na lista de registro, eles não são mais considerados na leitura
                        // assim, podem ser substituidos por novas inserções sem precisar limpar o espaço
                    }
                }
                printf("%d registro(s) removido(s).\n", n);
            }
            // Caso não hajam correspondências, retorna uma mensagem
            else
            {
                printf("Remoção inválida.\n");
            }
            break;
        }
        }
    }

    // Libera o espaço alocado dinamicamente
    free(regList);
    return 0;
}