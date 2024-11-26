#include "header.h"

void twoPasses()
{
    FILE *fd2;

    if (!(fd2 = fopen("Complete_Code.as", "r")))
    {
        fprintf(stderr, "Cannot open file\n");
        exit(0);
    }
    rewind(fd2);
    int IC =-1; /* Instruction counter */
    int DC = 0; /* Data counter */
    int extcnt = 0; /*ext counter*/
    int entcnt = 0; /*ent counter*/
    int symbolcnt = 0; /*symboles counter*/
    int linecnt = 0;
    char buf[81]={0};
    int j, k, i;
    char *token;
    char *sym;
    int *val1 = malloc(sizeof(int));
    int *val2 = malloc(sizeof(int));
    char *val3 = malloc(16*(sizeof(char)));
    int symbol_table_size = 10;
    FILE *fd5;
    Symboltype status = non;
    symbol *Symbol_table; /* pointer to symbol list */

    code **code_list = NULL;
    int code_count = 0;
    Symbol_table = (symbol *) malloc((symbol_table_size) * sizeof(symbol));
    if (Symbol_table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (i = 0; i < symbol_table_size; i++)
    {
        Symbol_table[i].name = NULL;
    }
    while (fgets(buf, sizeof(buf), fd2) != NULL) /* first pass on File */
    {
        IC++;
        linecnt++;
        if (strspn(buf, " \t\n") == strlen(buf) || buf[0] == ';') /*if empty line or comment line*/
            continue;

        token = strtok(buf, " ,\n\t");
        if(token!=NULL)
        {

            if (strcmp(token, ".extern") == 0)
            {
                status = ext;
                extcnt++;
                token = strtok(NULL, " \t\n,");
                addSymbolToTable(token, &Symbol_table, &symbolcnt, IC, DC, status, &symbol_table_size);
                status = non;
            }
            else if (strcmp(token, ".entry") == 0)
            {
                status = ent;
                entcnt++;
                token = strtok(NULL, " \t\n,");
                addSymbolToTable(token, &Symbol_table, &symbolcnt, IC, DC, status, &symbol_table_size);
                status = non;
            }

            else if (token[strlen(token) - 1] == ':') /* if label */
    {
        sym = malloc(strlen(token)+1);
        if (sym == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strncpy(sym, token, strlen(token) - 1);
        sym[strlen(token) - 1] = '\0'; /* null-terminate the string */

        addSymbolToTable(sym, &Symbol_table, &symbolcnt, IC, DC, status,&symbol_table_size);
        status = non;
        free(sym);

    }
}
        while ((token = strtok(NULL, " \t\n,")) != NULL)
        {
            if (strcmp(token, ".data") == 0)/*if data*/
            {
                while ((token = strtok(NULL, " \t\n,")) != NULL)
                {
                    code *w;
                    int number;
                    DC++;
                    w = (code *) malloc(sizeof(code));
                    number = atoi(token);
                    w->binary.fields.ARE = (number >> 10) & 0x3;  /* Extract top 2 bits */
                    w->binary.fields.des = (number >> 7) & 0x7;  /* Extract next 3 bits */
                    w->binary.fields.op = (number >> 3) & 0xF;  /* Extract next 4 bits */
                    w->binary.fields.sou = number & 0x7;  /* Extract last 3 bits */
                    w->decimal_address = 100 + IC + DC;

                    code_list = addToCodeList(code_list, w, &code_count, &IC);
                    free(w);
                }
                continue;
            }
            else if (strcmp(token, ".string") == 0) /*if string*/
            {
                token = strtok(NULL, " \t\n,");

                for (k = 0; k <= strlen(token); k++)
                {
                    char asciiChar;
                    code *w;
                    int number;
                    w = (code *) malloc(sizeof(code));
                    if (k == strlen(token))
                    {
                        asciiChar = 0;
                    }
                    else
                    {
                        asciiChar = token[k];
                    }
                    number = (int) asciiChar;
                    DC++;
                    w->binary.fields.ARE = (number >> 6) & 0x3;  /* Extract top 2 bits */
                    w->binary.fields.des = (number >> 3) & 0x7;  /* Extract next 3 bits */
                    w->binary.fields.op = (number >> 1) & 0x7;  /* Extract next 3 bits */
                    w->binary.fields.sou = 0;
                    w->decimal_address = 100 + IC + DC;

                    code_list = addToCodeList(code_list, w, &code_count, &IC);
                    free(w);
                }
            }


            if (strcmp(token, "mov") == 0)
            {

                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand1(w, token,val1,&val3);
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
		free(w);
            }
            else if (strcmp(token, "cmp") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 1;
                token = strtok(NULL, " \t\n,");
                handleOperand1(w, token,val1,&val3);
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
		free(w);
            }
            else if (strcmp(token, "add") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 2;
                token = strtok(NULL, " \t\n,");
                handleOperand1(w, token,val1,&val3);
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);

            }
            else if (strcmp(token, "sub") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 3;
                token = strtok(NULL, " \t\n,");
                handleOperand1(w, token,val1,&val3);
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);

            }
            else if (strcmp(token, "not") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 4;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "clr") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 5;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token, val2, &val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "lea") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 6;
                token = strtok(NULL, " \t\n,");
                handleOperand1(w, token,val1,&val3);
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "inc") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 7;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "dec") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 8;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "jmp") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 9;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "bne") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 10;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "red") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 11;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "prn") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 12;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "jsr") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 13;
                w->binary.fields.sou = 0;
                token = strtok(NULL, " \t\n,");
                handleOperand2(w, token,val2,&val3);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "rts") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 14;
                w->binary.fields.sou = 0;
                w->binary.fields.des = 0;
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (strcmp(token, "stop") == 0)
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.fields.op = 15;
                w->binary.fields.sou = 0;
                w->binary.fields.des = 0;
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
        if (code_list!=NULL && code_list[code_count-1]->binary.fields.sou==1)/*first operand is number*/
        {
            code *w;
            w = (code *) malloc(sizeof(code));
            if (w == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            w->decimal_address = 100 + IC + DC;
            w->binary.data.ARE=0;
            w->binary.data.info = (*val1 & 0x3FF);
            code_list = addToCodeList(code_list, w, &code_count, &IC);
            free(w);
        }
        else if (code_list!=NULL && code_list[code_count-1]->binary.fields.sou==5)/*first operand is register*/
        {
            code *w;
            w = (code *) malloc(sizeof(code));
            if (w == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            w->decimal_address = 100 + IC + DC;
            w->binary.regis.reg1 = (*val1 & 0x3FF);
            if (code_list[code_count - 1]->binary.fields.des == 5)/*first and second operands are registers*/
            {
                w->binary.regis.reg2 = (*val2 & 0x3FF);
            }
            code_list = addToCodeList(code_list, w, &code_count, &IC);
            free(w);
        }
        else if (code_list!=NULL &&code_list[code_count-1]->binary.fields.sou==3)/*first operand is symbol*/
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->con=islabel;
                w->name1 = val3;
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }

            if (code_count >= 2 && code_list[code_count-2] &&code_list[code_count-2]->binary.fields.des==1)/*second operand is number*/
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.data.ARE=0;
                w->binary.data.info = (*val1 & 0x3FF);
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
            else if (code_count >= 2 && code_list[code_count-2]!=NULL &&code_list[code_count-2]->binary.fields.des==5)/*second operand is register*/
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->binary.regis.reg1=(*val1 & 0x3FF);
                w->binary.regis.reg2=(*val2 & 0x3FF);
		free(w);
            }
            else if (code_count >= 2 && code_list[code_count-2]!=NULL &&code_list[code_count-2]->binary.fields.des==3)/*second operand is symbol*/
            {
                code *w;
                w = (code *) malloc(sizeof(code));
                if (w == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                w->decimal_address = 100 + IC + DC;
                w->con=islabel;
                w->name1 = val3;
                code_list = addToCodeList(code_list, w, &code_count, &IC);
                free(w);
            }
        }
        memset(buf, 0, sizeof(buf));
    }

    fclose(fd2);
    for(i = 0; i < code_count; i++) {
        if(code_list[i] != NULL && code_list[i]->con == islabel)
        {
            for(j = 0; j < symbolcnt; j++) {
                if (strcmp(code_list[i]->name1, Symbol_table[j].name) == 0) {
                    code_list[i]->binary.data.info = Symbol_table[j].cod.decimal_address;
                    if(Symbol_table[j].kind == ent) {
                        code_list[i]->binary.data.ARE = 2;
                    } else if(Symbol_table[j].kind == ext) {
                        code_list[i]->binary.data.ARE = 1;
                    } else {
                        code_list[i]->binary.data.ARE = 0;
                    }
                    break;
                }
            }
        }
    }

    if (extcnt) /*make ext file*/
        {
            FILE *fd3;
            int l;
            if (!(fd3 = fopen("ps.ext", "w+")))
            {
                fprintf(stderr, "Cannot open file\n");
                exit(0);
            }

            for (l = 0; l < symbolcnt; l++)
            {
                if (Symbol_table[l].kind == ext)
                {
                    fprintf(fd3, "%s    %d\n", Symbol_table[l].name, Symbol_table[l].cod.decimal_address);
                }
            }
            fclose(fd3);
        }
        if (entcnt) /*make ent file*/
        {
            FILE *fd4;
            int l;
            if (!(fd4 = fopen("ps.ent", "w+")))
            {
                fprintf(stderr, "Cannot open file\n");
                exit(0);
            }

            for (l = 0; l < symbolcnt; l++)
            {
                if (Symbol_table[l].kind == ent)
                {
                    fprintf(fd4, "%s    %d\n", Symbol_table[l].name, Symbol_table[l].cod.decimal_address);
                }
            }
            fclose(fd4);
        }

        if (!(fd5 = fopen("ps.ob", "w+")))
        {
            fprintf(stderr, "Cannot open file\n");
            exit(0);
        }
        fprintf(fd5, "%d  %d\n", linecnt, code_count);

    for (i = 0; i < code_count; i++)
    {
        if (code_list[i] )
        {
            char *encoded = base64(&code_list[i]->binary);
            if (encoded)
            {
                fprintf(fd5, "%s\n", encoded);
                free(encoded);
            }
        }
    }
        /* Free allocated memory */
    fclose(fd5);


    free(val1);
    free(val2);
    free(val3);

}

