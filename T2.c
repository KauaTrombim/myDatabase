#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 50

typedef struct
{
    int id;
    char login[15];
    char password[30];
    char gender;
    double salary;
} Register;

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

int main()
{
    Register *regList = (Register *)calloc(1000, sizeof(Register));

    int regsInFile = openNreadFile(regList); // Lê o arquivo solicitado e
    printf("%d registro(s) lido(s).\n", regsInFile);
    int possibleInserts = 1000 - regsInFile;

    // 1-- Ler o numero de registros em um arquivo || FEITO
    // 2-- Implementar a busca de registros
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
        case '2':
            // Implementar a função de busca
            printf("Query");
            break;
        case '3':
        {
            char key[9];
            sscanf(line, "%*d \"%[^\"]\"", key);
            if (strcmp(key, "id") == 0)
            {
                int id;
                sscanf(line, "%*d \"%*[^\"]\" %d", &id);
                // Chamar a busca pelo Id
            }
            break;
        }
        }
    }
    return 0;
}