#include <stdio.h>
#include <stdlib.h>

char input[100] = {"3+5*7$"};
int input_idx;
double num;

enum Token {INVALID, PLUS, STAR, NUMBER, LPAREN, RPAREN, END} token;

void get_token();
void error(int i);
double expression();
double term();
double factor();

void get_token(){
	// token = next token of input
	char cur = input[input_idx++];
	double temp = 0;

	//printf("%d\n", cur);

	if (cur == '+') token = PLUS;
	else if (cur == '*') token = STAR;
	else if (cur == '(') token = LPAREN;
	else if (cur == ')') token = RPAREN;
	else if (cur >= '0' && cur <= '9'){

		token = NUMBER;
		temp += cur - '0';

		while (1){
			cur = input[input_idx++];
			if (cur >= '0' && cur <= '9')
				temp = temp * 10 + cur - '0';
			else{
				input_idx--;
				break;
			}
		}
	}
	else if (cur == '$') token = END;
	else token = INVALID;

	// num = number value
	if (token == NUMBER)
		num = temp;
}

void error(int i){
	switch (i){
		case 1:
			printf("error: number or '(' expected\n");
			break;
		case 2:
			printf("error: ')' expected\n");
			break;
		case 3:
			printf("error: EOF expected\n");
			break;
	}
	exit(1);
}

double expression(){
	//printf("exp\n");
	double result;
	result = term();
	while (token == PLUS){
		get_token();
		result = result + term();
	}
	return result;
}

double term(){
	//printf("term\n");
	double result;
	result = factor();
	while (token == STAR){
		get_token();
		result = result * factor();
	}
	return result;
}

double factor(){
	//printf("fact\n");
	double result;
	if (token == NUMBER){
		result = num;
		get_token();
	}
	else if (token == LPAREN){
		get_token();
		result = expression();
		if (token == RPAREN)
			get_token();
		else
			error(2);
	}
	else
		error(1);
	return result;
}

void main(){
	double result;
	get_token();
	result = expression();
	//priintf("%f\n", result);
	if (token != END)
		error(3);
	else
		printf("%f\n", result);
}
