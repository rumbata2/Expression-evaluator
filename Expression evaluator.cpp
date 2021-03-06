#include "stdafx.h"
#include <iostream>
#include <stack>

bool isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool isOperator(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int precedence(char ch) {
	if (ch == '+' || ch == '-')
		return 1;
	else if (ch == '*' || ch == '/')
		return 2;
}

void putIntInString(int num, int& pos, char* str) {
	int temp = num;
	int posCopy = pos;
	while (temp != 0) {
		temp /= 10;
		pos++;
	}
	for (int i = pos - 1; i >= posCopy; i--) {
		str[i] = (num % 10) + '0';
		num /= 10;
	}
}

int takeIntFromString(int& pos, char* str) {
	int result = 0;
	while (str[pos] >= '0' && str[pos] <= '9') {
		result = 10 * result + (str[pos] - '0');
		pos++;
	}
	return result;
}

char* convertToRPN(char* input) {
	int size = strlen(input);
	int posInput = 0;
	int posOutput = 0;
	char* output = new char[128];
	output[127] = '\0';
	std::stack<char> operators;

	while (posInput < size) {
		if (isDigit(input[posInput])) {
			putIntInString(takeIntFromString(posInput, input), posOutput, output);
			output[posOutput] = ' ';
			posOutput++;
		}
		else if (isOperator(input[posInput])) {	
			while (!operators.empty() && operators.top() != '(' &&
				(
				precedence(input[posInput]) < precedence(operators.top()) ||
				(precedence(operators.top()) == precedence(input[posInput]))			
				))         
			{
				output[posOutput] = operators.top();
				posOutput++;
				output[posOutput] = ' ';
				posOutput++;
				operators.pop();
			}
			operators.push(input[posInput]);
			posInput++;
		}
		else if (input[posInput] == '(') {
			operators.push(input[posInput]);
			posInput++;
		}
		else if (input[posInput] == ')') {
			while (!operators.empty() &&  operators.top() != '(') {
				output[posOutput] = operators.top();
				posOutput++;
				output[posOutput] = ' ';
				posOutput++;
				operators.pop();				
			}
			posInput++;
			if (!operators.empty() && operators.top() == '(') {
				operators.pop();
			}
		}
	}
	while (!operators.empty()) {
		output[posOutput] = operators.top();
		posOutput++;
		output[posOutput] = ' ';
		posOutput++;
		operators.pop();
	}

	for (int i = posOutput; i < strlen(output); i++)
		output[i] = ' ';
	return output;
}

int evaluateRPN(char* input) {
	int size = strlen(input);
	int pos = 0;
	std::stack<int> numbers;
	int x, y;

	while (pos < size) {
		if (isDigit(input[pos])) {
			numbers.push(takeIntFromString(pos, input));
		}
		else if (isOperator(input[pos])) {
			x = numbers.top();
			numbers.pop();
			y = numbers.top();
			numbers.pop();
			switch (input[pos]) {
			case '+':
				numbers.push(y + x);
				pos++;
				break;
			case '-':
				numbers.push(y - x);
				pos++;
				break;
			case '*':
				numbers.push(x*y);
				pos++;
				break;
			case '/':
				numbers.push(y / x);
				pos++;
				break;
			default:
				break;
			}
		}
		else pos++;
	}
	return numbers.top();
}

int main()
{
	std::cout << "Please enter an expression: ";
	char input[128];
	std::cin.getline(input, 128);

	char* rpn = convertToRPN(input);
	std::cout << "RPN of expression: " << rpn << std::endl;
	std::cout << "Evaluates to: " << evaluateRPN(rpn) << std::endl;

	return 0;
}

