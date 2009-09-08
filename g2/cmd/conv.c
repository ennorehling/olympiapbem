
#include	<stdio.h>

#define	tolower(c)	(((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

#define		LEN	100



/*
 *  G2 Entity coding system:
 *
 *  range	      extent	use
 *  1-999               999	items
 *  1000-8999	       8000	chars
 *  9000-9999          1000	skills
 *
 *  10,000-19,999    10,000	provinces		(CCNN: AA00-DV99)
 *  20,000-49,999    20,000	more provinces		(CCNN: DW00-ZZ99)
 *  50,000-56,759      6760	player entities		(CCN)
 *  56,760-58,759      2000	lucky locs		(CNN)
 *  58,760-58,999	240	regions			(NNNNN)
 *  59,000-78,999    20,000	sublocs, misc		(CNNN: A000-Z999)
 *  79,000-100,000   21,000     storms			(NNNNN)
 *
 *  Note: restricted alphabet, no vowels (except a) or l:
 *	"abcdfghjkmnpqrstvwxz"
 */


static char *letters = "abcdefghijklmnopqrstuvwxyz";
static char *letters2 = "abcdfghjkmnpqrstvwxz";


int
letter_val(char c, char *let)
{
	char *p;

	for (p = let; *p; p++)
		if (*p == c)
			return p-let;

	return 0;	/* error */
}


char *
int_to_code(int l)
{
	static char buf[LEN];
	int n, a, b, c;

	if (l < 10000)
	{
		sprintf(buf, "%d", l);
	}
	else if (l < 50000)		/* CCNN */
	{
		l -= 10000;

		n = l % 100;
		l /= 100;

		a = l % 20;
		b = l / 20;

		sprintf(buf, "%c%c%02d", letters2[b], letters2[a], n);
	}
	else if (l < 56760)		/* CCN */
	{
		l -= 50000;

		n = l % 10;
		l /= 10;

		a = l % 26;
		b = l / 26;

		sprintf(buf, "%c%c%d", letters[b], letters[a], n);
	}
	else if (l < 58760)		/* CNN */
	{
		l -= 56760;

		n = l % 10;
		l /= 10;

		a = l % 10;
		b = l / 10;

		sprintf(buf, "%c%d%d", letters2[b], a, n);
	}
	else if (l < 59000)
	{
		sprintf(buf, "%d", l);
	}
	else if (l < 79000)
	{
		l -= 59000;

		a = l / 1000;
		n = l % 1000;

		sprintf(buf, "%c%03d", letters2[a], n);
	}
	else
	{
		sprintf(buf, "%d", l);
	}

	return buf;
}


int
code_to_int(char *s)
{
	char a, b, c, d;

	if (isdigit(*s))
		return atoi(s);

	if (!isalpha(*s))
		return 0;

	switch (strlen(s))
	{
	case 3:
		if (isalpha(*(s+1)) && isdigit(*(s+2)))		/* CCN */
		{
			a = tolower(*s) - 'a';
			b = tolower(*(s+1)) - 'a';
			c = *(s+2) - '0';

			return a * 260 + b * 10 + c + 50000;
		}

		if (isdigit(*(s+1)) && isdigit(*(s+2)))		/* CNN */
		{
			a = letter_val(tolower(*s), letters2);
			b = *(s+1) - '0';
			c = *(s+2) - '0';

			return a * 100 + b * 10 + c + 56760;
		}

		return 0;

	case 4:							/* CCNN */
		if (isalpha(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)))
		{
			a = letter_val(tolower(*s), letters2);
			b = letter_val(tolower(*(s+1)), letters2);
			c = *(s+2) - '0';
			d = *(s+3) - '0';


			return a * 2000 + b * 100 + c * 10 + d + 10000;
		}

		if (isdigit(*(s+1)) && isdigit(*(s+2)) && isdigit(*(s+3)))
		{
			a = letter_val(tolower(*s), letters2);
			b = *(s+1) - '0';
			c = *(s+2) - '0';
			d = *(s+3) - '0';


			return a * 1000 + b * 100 + c * 10 + d + 59000;
		}
		return 0;

	default:
		return 0;
	}
}


main(argc, argv)
int argc;
char **argv;
{

	if (argc > 1 && isalpha(argv[1][0]))
		printf("%d\n", code_to_int(argv[1]));
	else if (argc > 1 && isdigit(argv[1][0]))
		printf("%s\n", int_to_code(atoi(argv[1])));
	else
	{
		fprintf(stderr, "usage: %s code|entity-number\n", argv[0]);
		exit(1);
	}

	exit(0);
}

