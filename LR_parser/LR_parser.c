#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUMBER 256
#define PLUS 257
#define STAR 258
#define LPAREN 259
#define RPAREN 260
#define END 261
#define EXPRESSION 0
#define TERM 1
#define FACTOR 2
#define ACC 999

int action[12][6] = {
	{5, 0, 0, 4, 0, 0}, {0, 6, 0, 0, 0, ACC}, {0, -2, 7, 0, -2, -2},
	{0, -4, -4, 0, -4, -4}, {5, 0, 0, 4, 0, 0}, {0, -6, -6, 0, -6, -6},
	{5, 0, 0, 4, 0, 0}, {5, 0, 0, 4, 0, 0}, {0, 6, 0, 0, 11, 0},
	{0, -1, 7, 0, -1, -1}, {0, -3, -3, 0, -3, -3}, {0, -5, -5, 0, -5, -5}};

int go_to[12][3]  = {
	{1, 2, 3}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {8, 2, 3}, {0, 0, 0},
	{0, 9, 3}, {0, 0, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


int prod_left[7] = {0, EXPRESSION, EXPRESSION, TERM, TERM, FACTOR, FACTOR};
int prod_length[7] = {0, 3, 1, 3, 1, 3, 1};

int stack[1000];
int top = -1;
int sym;
 
typedef struct {
	int type; // 0:int, 1:float
	union {
		int i;
		float f;
	} value;
}fi;

fi value[1000];
char yytext[32];
fi yylval;

void push(int);
void reduce(int);
void yyerror(char []);
void lex_error();
void yyparse();
int yylex();
void shift(int);

void main(){

	//read input
	printf("Input expression!\n");
	printf("Expression must end with '$'!!\n\n");
	printf("Input : ");

	//simulation parsing
	yyparse();

	//print result
	if (value[1].type == 0)
		printf("Output : %d\n", value[1].value.i);
	else
		printf("Output : %f\n", value[1].value.f);
}

void yyparse(){
	int i;

	stack[++top] = 0;
	sym = yylex();

	do{
		/* test code
		printf("top : %d\n", top);
		printf("stack : ");
		for (int i = 0; i < 8; i++)
		       printf("%d ", stack[i]);
		printf("\n");
		printf("value : ");
		for (int i = 0; i < 8; i++){
			if (value[i].type == 0)
				printf("%d ", value[i].value.i);
			else
				printf("%.0f ", value[i].value.f);
		}
		printf("\n\n");
		*/

		i = action[stack[top]][sym - 256];
		
		if (i == ACC) ; //printf("success!\n");
		else if (i > 0) shift(i);
		else if (i < 0) reduce(-i);
		else yyerror("syntax error\n");

	} while (i != ACC);
}

void push(int i){
	top++;
	stack[top] = i;
}

void shift(int i){
	push(i);
	value[top] = yylval;
	sym = yylex();
}

void reduce(int i){
	int old_top;

	top -= prod_length[i];
	old_top = top;
	push(go_to[stack[old_top]][prod_left[i]]);

	// preprocessing
	// 'fi' type's '+' and '*' operator is undefined
	fi result;
	if (i == 1 || i == 3){
		fi temp1, temp2;
		temp1 = value[old_top + 1];
		temp2 = value[old_top + 3];

		if (temp1.type == 0){
			if (temp2.type == 0){
				result.type = 0;
				if (i == 1)
					result.value.i = temp1.value.i + temp2.value.i;
				else
					result.value.i = temp1.value.i * temp2.value.i;
			}
			else{
				result.type = 1;
				if (i == 1)
					result.value.f = temp1.value.i + temp2.value.f;
				else
					result.value.f = temp1.value.i * temp2.value.f;
			}
		}
		else{
			result.type = 1;
			if (temp2.type == 0){
				if (i == 1)
					result.value.f = temp1.value.f + temp2.value.i;
				else
					result.value.f = temp1.value.f * temp2.value.i;
			}
			else{
				if (i == 1)
					result.value.f = temp1.value.f + temp2.value.f;
				else
					result.value.f = temp1.value.f * temp2.value.f;
			}
		}

	}

	switch (i){
		case 1:
			value[top] = result;
			break;
		case 2:
			value[top] = value[old_top + 1];
			break;
		case 3:
			value[top] = result;
			break;
		case 4:
			value[top] = value[old_top + 1];
			break;
		case 5:
			value[top] = value[old_top + 2];
			break;
		case 6:
			value[top] = value[old_top + 1];
			break;
		default:
			yyerror("parsing table error\n");
			break;
	}

}

void yyerror(char s[]){
	printf("%s", s);
	exit(1);
}

int yylex(){
	static char ch = ' ';
	int i = 0;

	while (ch == ' ' || ch == '\t' || ch == '\n')
		ch = getchar();

	if (isdigit(ch)){
		int dec_flag = 0;

		do{
			yytext[i++] = ch;
			ch = getchar();

			//decimal parts
			if (ch == '.'){
				dec_flag = 1;
				yytext[i++] = ch;
				ch = getchar();

				if (!isdigit(ch))
					yyerror("invalid number\n");
			}

		} while (isdigit(ch));

		yytext[i] = 0;

		//init yylval
		yylval.type = 0;
		yylval.value.i = 0;
		yylval.value.f = 0;

		if (dec_flag == 0){
			yylval.type = 0;
			yylval.value.i = atoi(yytext);
		}
		else{
			yylval.type = 1;
			yylval.value.f = atof(yytext);
		}

		return NUMBER;
	}
	else if (ch == '+'){
		ch = getchar();
		return PLUS;
	}
	else if (ch == '*'){
		ch = getchar();
		return STAR;
	}
	else if (ch == '('){
		ch = getchar();
		return LPAREN;
	}
	else if (ch == ')'){
		ch = getchar();
		return RPAREN;
	}
	else if (ch == '$'){
		ch = getchar();
		return END;
	}
	else
		lex_error();
}

void lex_error(){
	printf("illegal token\n");
	exit(1);
}
