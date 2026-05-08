
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

char *s;
bool error = false;

int level0_add();

void err(char c){
	if (error)
		return;
	error = true;
	if (c == 0 || *s == 0)
		printf("Unexpected end of input");
	else
		printf("Unexpected token '%c'\n", c);
}

int level2_par(){
	int res = 0;
	if (isdigit(*s))
		return (*s++) - '0';
	if (*s == '(')
	{
		s++;
		res = level0_add();
		if (*s != ')')
			return (err(*s++), res);
		s++;
		return res;
	}
	return (err(*s), res);
}

int level1_mult(){
	int	res = level2_par();
	while (*s == '*')
	{
		s++;
		res *= level2_par();
	}
	return res;
}

int level0_add(){
	int res = level1_mult();
	while (*s == '+')
	{
		s++;
		res += level1_mult();
	}
	return res;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return 1;
	s = argv[1];
	int res = level0_add();
	if (*s)
		err(*s++);
	if (error == true)
		return 1;
	printf("%d\n", res);
	return 0;
}
