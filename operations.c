#include "header.h"


char* base64(const word* w)
{
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i, j;
    int binary[12] = {0};
    char* base64;
    binary[0] = (w->fields.ARE >> 1) & 1;
    binary[1] = w->fields.ARE & 1;
    binary[2] = (w->fields.des >> 2) & 1;
    binary[3] = (w->fields.des >> 1) & 1;
    binary[4] = w->fields.des & 1;
    binary[5] = (w->fields.op >> 3) & 1;
    binary[6] = (w->fields.op >> 2) & 1;
    binary[7] = (w->fields.op >> 1) & 1;
    binary[8] = w->fields.op & 1;
    binary[9] = (w->fields.sou >> 2) & 1;
    binary[10] = (w->fields.sou >> 1) & 1;
    binary[11] = w->fields.sou & 1;

    base64 = malloc(3 * sizeof(char));
    if (!base64)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    for (i = 0; i < 12; i += 6)
    {
        int value = 0;
        for (j = i; j < i + 6; j++)
        {
            value = 2 * value + binary[j];
        }
        base64[i / 6] = base64_chars[value];
    }

    base64[2] = '\0';

    return base64;
}



macro newmac(char* name, char* body)
{
    macro m;
    m.body= body;
    m.name= name;
    return m;
}
OperandInfo getOperandType(const char *token)
{
    OperandInfo info;
    info.value = -1;
    info.name = NULL;

    if (token[0] == '-' || isdigit((unsigned char)token[0])) {
        int i;
        for (i = 1; i < strlen(token); i++) {
            if (!isdigit((unsigned char)token[i]))
                break;
        }
        if (i == strlen(token)) {
            info.type = OPERAND_NUMBER;
            info.value = atoi(token);
            return info;
        }
    }

    /* Check for register name*/
    if (token[0] == '@' && strlen(token) == 3 && token[1] == 'r' && token[2] >= '0' && token[2] <= '7') {
        info.type = OPERAND_REGISTER;
        info.value = token[2] - '0';
        return info;
    }

    info.type = OPERAND_SYMBOL;
    info.name = malloc(strlen(token) + 1);
    if (info.name != NULL)
    {
        strcpy(info.name, token);
    }
    return info;
}
void handleOperand1(code* w, char* token,int * a,char ** b)
{
    OperandInfo operandInfo = getOperandType(token);

    switch (operandInfo.type) {
        case OPERAND_NUMBER:
            w->binary.fields.sou = 1;  /* binary for 1*/
            *a = operandInfo.value;
            break;

        case OPERAND_REGISTER:
            w->binary.fields.sou = 5;  /* binary for 5*/
            *a = operandInfo.value;
            break;

        case OPERAND_SYMBOL:
            w->binary.fields.sou = 3;  /* binary for 3*/
            *b= operandInfo.name;
            break;

        default:
            printf("Error: Unknown operand type\n");
            break;
    }
}
void handleOperand2(code* w, char* token,int * a,char ** b)
{
    OperandInfo operandInfo = getOperandType(token);

    switch (operandInfo.type)
    {
        case OPERAND_NUMBER:
            w->binary.fields.des = 1;
            *a = operandInfo.value;
            break;

        case OPERAND_REGISTER:
            w->binary.fields.des = 5;
            *a = operandInfo.value;
            break;

        case OPERAND_SYMBOL:
            w->binary.fields.des = 3;
            *b= operandInfo.name;
            break;

        default:
            printf("Error: Unknown operand type\n");
            break;
    }
}
code **addToCodeList(code **code_list, code *w, int *code_count, int *IC)
{
    code_list = (code **) realloc(code_list, (*code_count + 1) * sizeof(code *));
    if (!code_list)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    code_list[*code_count] = w;
    (*code_count)++;
    return code_list;
}


void addSymbolToTable(char *token, symbol **Symbol_table, int *symbolcnt, int IC, int DC, Symboltype status, int *symbol_table_size)
{
    int j;
    

    if (token == NULL)
    {
        fprintf(stderr, "Error: token is NULL\n");
        return;
    }

    if (*symbolcnt >= *symbol_table_size)
    {
        *symbol_table_size *= 2;
        *Symbol_table = (symbol *) realloc(*Symbol_table, *symbol_table_size * sizeof(symbol));
        if (!*Symbol_table)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (j = *symbolcnt; j < *symbol_table_size; j++)
        {
            (*Symbol_table)[j].name = NULL;
        }
    }


    for (j = 0; j < *symbolcnt; j++)
    {
        if ((*Symbol_table)[j].name != NULL && strcmp(token, (*Symbol_table)[j].name) == 0)
        {
            printf("Error: label already been initialized\n");
            
            return;
        }
    }

    (*Symbol_table)[*symbolcnt].name = token;
    (*Symbol_table)[*symbolcnt].kind = status;
    (*Symbol_table)[*symbolcnt].cod.decimal_address = 100 + IC + DC;
    
    (*symbolcnt)++;
}


