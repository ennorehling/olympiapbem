
#include	<stdio.h>

#define		MAX	1000

int ar[MAX];
int top = 0;

main() {
	char buf[256];
	int i, j;
	double a;

	while (fgets(buf, 256, stdin) != NULL)
	{
		ar[top++] = atoi(buf);
	}

	for (j = 2; j <= top; j++)
	{
		a = 1.0 - (float) ar[0] / 100.0;

		printf("%d%%", ar[0]);

		for (i = 1; i < j; i++)
		{
			printf(" + %d%%", ar[i]);
			a *= 1.0 - (float) ar[i] / 100.0;
		}

		printf(" = %2.1f%%\n", (1.0 - a) * 100.0);
	}
}

