

typedef enum {inmac,outmac}mac;
/*-----------------------------------------------------------*/
typedef union {
    struct {
        unsigned ARE : 2;
        unsigned des : 3;
        unsigned op  : 4;
        unsigned sou : 3;
    } fields;
    struct {
        unsigned ARE : 2;
        unsigned info: 10;
    } data;
    struct {
        unsigned ARE : 2;
        unsigned reg1 : 5;
        unsigned reg2 : 5;
    } regis;
} word; /*define 12 bits word*/
/*-----------------------------------------------------------*/
typedef enum {
    OPERAND_NUMBER,
    OPERAND_REGISTER,
    OPERAND_SYMBOL,
    OPERAND_UNKNOWN
} OperandType;
typedef enum {
    islabel,
    isntlabel
}Label; /*define lable*/
/*-----------------------------------------------------------*/
typedef struct {
    OperandType type;
    int value;
    char *name;
} OperandInfo;
/*-----------------------------------------------------------*/
typedef enum {there_is_symbol,there_isnt_symbol}Symbolfound;
/*-----------------------------------------------------------*/
typedef enum {ext,ent,non}Symboltype;
/*-----------------------------------------------------------*/
typedef struct/*define macro*/
{
    char* name;
    char* body;
}macro;
/*-----------------------------------------------------------*/
typedef struct /*define word*/
{
    int decimal_address;
    word binary;
    Label con;
    char* name1;
} code;
/*----------------------------------------------------------*/
typedef struct/*define symbol*/
{
    char* name;
    Symboltype kind;
    code cod;

}symbol;

