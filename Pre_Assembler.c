#include "header.h"

/* Definition of variables */
char buf[81];
char buf_copy[81];
mac status = outmac; /* Macro status initially set to 'outmac' */
macro *maclist; /* Pointer to array of macros */
int macnum = 0; /* Number of macros */
char *token; /* Pointer for tokenized string */
size_t macro_body_size = 0;
char temp_buf[81];

void pre_Assembler(const char *inputFilePath)
{
    FILE *inputFile;
    if (!(inputFile = fopen(inputFilePath, "r")))
    {
        fprintf(stderr, "Cannot open input file\n");
        exit(0);
    }
    FILE *fd1;
    int i;

    /* Open file for writing and reading */
    if (!(fd1 = fopen("Complete_Code.as", "w+")))
    {
        fprintf(stderr, "Cannot open file\n");
        exit(0);
    }

    /* Allocate initial memory for 10 macros */
    maclist = (macro*)malloc(10 * sizeof(macro));

    for (;;)
    {
        /* Clear 'buf' at the start of each iteration */
        memset(buf, 0, sizeof(buf));

        /* Break loop if fgets returns NULL (end of file) */
        if (fgets(buf, 80, inputFile) == NULL)
        {
            break;
        }

        strcpy(buf_copy, buf);

        token = strtok(buf, " ,\n\t");

        while (token != NULL)
        {
            /* Check if token is the name of a macro */
            for (i = 0; i < macnum; i++)
            {
                /* If token is a macro name, print macro body and get next token */
                if (!strcmp(token, maclist[i].name))
                {
                    fputs(maclist[i].body, fd1);
                    token = strtok(NULL, " \t,");
                    break;
                }
            }
            /* If token is "mcro", start of macro definition */
            if (token && strcmp(token, "mcro") == 0)
            {
                status = inmac;
                token = strtok(NULL, "\t\n,");

                /* Allocate memory for macro name */
                char *name = (char *) malloc(sizeof(char) * 20);
                if (!name)
                {
                    fprintf(stderr, "Cannot allocate memory for macro name\n");
                    exit(0);
                }
                strcpy(name, token);

                /* Prepare to store macro body */
                 macro_body_size = 0;
                char *str = (char *) malloc(1); 
                if (!str)
                {
                    fprintf(stderr, "Cannot allocate memory for macro body\n");
                    exit(0);
                }
                str[0] = '\0'; 

                /* Read macro body until 'endmcro' */
                char temp_buf[81];
                while (fgets(temp_buf, 80, inputFile) != NULL && strncmp(temp_buf, "endmcro", 7) != 0)
                {
                    macro_body_size += strlen(temp_buf);
                    str = (char *) realloc(str, macro_body_size + 1);
                    if (!str)
                    {
                        fprintf(stderr, "Cannot allocate memory for macro body\n");
                        exit(0);
                    }
                    strcat(str, temp_buf);
                }

                /* Switch status to 'outmac' */
                status = outmac;


                /* Reallocate memory if necessary */
                if (macnum % 10 == 0)
                {
                    maclist = (macro*)realloc(maclist, (macnum + 10) * sizeof(macro));
                    if (!maclist)
                    {
                        fprintf(stderr, "Cannot allocate memory\n");
                        exit(0);
                    }
                }

                /* Create new macro and increment macro number */
                maclist[macnum] = newmac(name, str);
                macnum++;

                if (fgets(buf, 80, inputFile) == NULL)
                {
                    break;
                }

                memset(buf_copy, 0, sizeof(buf_copy));
                strcpy(buf_copy, buf);

                token = strtok(buf, " ,\n\t");
            }
            else if (token)
            {
                int macro_found = 0;
                /* Check if token is the name of a macro */
                for (i = 0; i < macnum; i++)
                {
                    if (!strcmp(token, maclist[i].name))
                    {
                        fputs(maclist[i].body, fd1);
                        macro_found = 1;
                        break;
                    }
                }
                /* If no macro found, print the original line */
                if (!macro_found)
                {
                    fputs(buf_copy, fd1);
                }
                /* Get next line */
                if (fgets(buf, 80, inputFile) == NULL)
                {
                    break;
                }
                /* Clear 'buf_copy' and copy new 'buf' into it */
                memset(buf_copy, 0, sizeof(buf_copy));
                strcpy(buf_copy, buf);

                /* Tokenize 'buf' again */
                token = strtok(buf, " ,\n\t");
            }
        }
    }

    /* Free memory for macro names and bodies */
    for(i = 0; i < macnum; i++)
    {
        free(maclist[i].name);
        free(maclist[i].body);
    }

    /* Free memory for macro list */
    if (maclist)
    {
        free(maclist);
        maclist = NULL;
    }
    fclose(fd1);
    fclose(inputFile);

}
