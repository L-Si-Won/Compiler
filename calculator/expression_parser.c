#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char input[100] = {};
int input_idx;
double num;
int dec_cnt;

enum Token {INVALID, PLUS, STAR, NUMBER, LPAREN, RPAREN, DOT, END} token;

void get_token();
void error(int i);
double expression();
double term();
double factor();

void get_token(){
	char cur = input[input_idx++];
	double temp = 0;
	dec_cnt = 0;

	// decide token
	if (cur == '+') token = PLUS;
	else if (cur == '*') token = STAR;
	else if (cur == '(') token = LPAREN;
	else if (cur == ')') token = RPAREN;
	else if (cur >= '0' && cur <= '9'){

		token = NUMBER;
		temp += cur - '0';

		// reading until it's not a number
		while (1){
			cur = input[input_idx++];
			dec_cnt++;

			if (cur >= '0' && cur <= '9')
				temp = temp * 10 + cur - '0';
			else{
				input_idx--;
				break;
			}
		}
	}
	else if (cur == '.') token = DOT;
	else if (cur == '\n') token = END;
	else token = INVALID;

	// save number value in num
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
		case 4:
			printf("error: number expected\n");
			break;
	}
	exit(1);
}

double expression(){
	double result;

	result = term();

	while (token == PLUS){
		get_token();
		result = result + term();
	}

	return result;
}

double term(){
	double result;

	result = factor();

	while (token == STAR){
		get_token();
		result = result * factor();
	}

	return result;
}

double decimal(){
	double temp = num;

	// make decimal parts
	while (dec_cnt--)
		temp /= 10;

	return temp;
}

double factor(){
	double result;

	if (token == NUMBER){
		result = num;
		get_token();

		if (token == DOT){
			get_token();

			if (token != NUMBER)
				error(4);

			// calculate decimal parts
			result += decimal();
			get_token();
		}
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

	printf("Input expression!\n");
	printf("Max length is 100!!!\n\n");

	//input expression
	write(1, "Input : ", 8);
	read(0, input, 100);

	//parse and calculate
	get_token();
	result = expression();

	//print result
	if (token != END)
		error(3);
	else
		printf("Output : %f\n", result);
}
