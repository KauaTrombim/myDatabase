#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 50
#define MAX_REGISTERS 1000

typedef struct
{
    int id;
    char login[15];
    char password[30];
    char gender;
    double salary;
} Register;

typedef union{
    int id;
    char login[15];
    char password[30];
    char gender;
    double salary;
} KeyValue;

void readRegister(FILE *base, Register *list)
{
    char line[MAX_LINE_SIZE];

    while (fgets(line, sizeof(line), base) != NULL)
    {
        if (strstr(line, "}"))
        {
            return;
        }
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

// Auxiliar
void printData(Register registro)
{
    printf("----------------------------------------\n");
    printf("ID:       %d\n", registro.id);
    printf("Login:    %s\n", registro.login);
    printf("Password: %s\n", registro.password);
    printf("Gender:   %c\n", registro.gender);
    printf("Salary:   %.2f\n", registro.salary);
    printf("----------------------------------------\n\n");
}

int openNreadFile(Register *list)
{
    char fileName[14];
    if (fgets(fileName, sizeof(fileName), stdin) == NULL)
    {
        printf("Erro na leitura");
        return 1;
    }
    fileName[strcspn(fileName, "\n")] = '\0'; // Substituindo o \n (enter) pelo \0 (fim da string)

    // Abertura do arquivo
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Nao achei mano");
        return 1;
    }

    int nRegisters = 0;
    int lineMaxSize = MAX_LINE_SIZE;
    char line[lineMaxSize];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "{"))
        {
            readRegister(file, &list[nRegisters]);
            nRegisters++;
        }
    }
    fclose(file);

    // for (int i = 0; i < nRegisters; i++)
    // {
    //     printData(regList[i]);
    // }

    return nRegisters;
}

int searchFor(int keyCode, KeyValue value, Register *list){
    int index = -1;
    switch (keyCode)
    {
    case 1:{
        for(int i = 0; i < MAX_REGISTERS; i++){
            if(list[i].id == value.id){
                index = i;
            }
        }
        break;
    }
    case 2:{
        for(int i = 0; i < MAX_REGISTERS; i++){
            if(strcmp(list[i].login, value.login) == 0){
                index = i;
            }
        }
        break;
    }
    case 3:{
        for(int i = 0; i < MAX_REGISTERS; i++){
            if(strcmp(list[i].password, value.password) == 0){
                index = i;
            }
        }
        break;
    }
    case 4:{
        for(int i = 0; i < MAX_REGISTERS; i++){
            if(list[i].gender == value.gender){
                index = i;
            }
        }
        break;
    }
    case 5:{
        for(int i = 0; i < MAX_REGISTERS; i++){
            if(list[i].salary == value.salary){
                index = i;
            }
        }
        break;
    }
    }

    return index;
}

int interpreteDataLine(char command[100], Register *list){
    char key[9];
    KeyValue keyValue;
    int rgstIndex;
    sscanf(command, "%*d \"%[^\"]\"", key);

    //Interpreta a chave e armazena o valor procurado
    if (strcmp(key, "id") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %d", &keyValue.id);
        rgstIndex = searchFor(1, keyValue, list);
    }
    else if (strcmp(key, "login") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %s", keyValue.login);
        rgstIndex = searchFor(2, keyValue, list);
    }
    else if (strcmp(key, "password") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %s", keyValue.password);
        rgstIndex = searchFor(3, keyValue, list);
    }
    else if (strcmp(key, "gender") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %c", &keyValue.gender);
        rgstIndex = searchFor(4, keyValue, list);
    }
    else if (strcmp(key, "salary") == 0)
    {
        sscanf(command, "%*d \"%*[^\"]\" %lf", &keyValue.salary);
        rgstIndex = searchFor(5, keyValue, list);
    }

    return rgstIndex;
}



int main()
{
    Register *regList = (Register *)calloc(1000, sizeof(Register));

    int regsInFile = openNreadFile(regList); // Lê o arquivo solicitado e
    printf("%d registro(s) lido(s).\n", regsInFile);
    int possibleInserts = 1000 - regsInFile;

    // 1-- Ler o numero de registros em um arquivo || FEITO
    // 2-- Implementar a busca de registros || FEITO
    // 3-- Implementar a deleção de registros
    // 4-- Implementar a inserção de registros

    char line[100];
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        switch (line[0])
        {
        case '1':
            if (possibleInserts > 0)
            {
                printf("Insert");
                // implementar a função de inserção (lembre da regra)
                break;
            }
            printf("Sem espaço para inserção.");
            break;
        case '2':{
            int i = interpreteDataLine(line, regList);
            if(i != -1){
                printf("{\n");
                printf("    \"id\": %d,\n",regList[i].id);
                printf("    \"login\": \"%s\",\n",regList[i].login);
                printf("    \"password\": \"%s\",\n",regList[i].password);
                printf("    \"gender\": \"%c\",\n",regList[i].gender);
                printf("    \"salary\": %.2lf\n",regList[i].salary);
                printf("}\n");   
            }
            else{
                printf("Nada encontrado.\n");
            }
            break;
        }
        case '3':
        {
            int i = interpreteDataLine(line, regList);
            if(i != -1){
                //Terminar essa implementação
            }
            else{
                printf("Remoção inválida\n");
            }
            break;
        }
        }
    }
    return 0;
}