
#include	<stdio.h>


#define	TRUE	1
#define	FALSE	0

#define	LEN	200


char libdir[LEN];

int force_name = FALSE;

FILE *scan_fp = NULL;
int line_number;
char fnam_path[LEN];
int scan_num;

char buf[LEN];


main(argc, argv)
int argc;
char *argv[];
{
	extern int optind, opterr;
	extern char *optarg;
	int errflag = 0;
	int c;
	int edit_flag = FALSE;

	strcpy(libdir, "/g2");

	while ((c = getopt(argc, argv, "el:n")) != -1)
	{
		switch(c)
		{
		case 'e':
			edit_flag = TRUE;
			break;

		case 'l':
			strcpy(libdir, optarg);
			break;

		case 'n':
			force_name = TRUE;
			break;

		case '?':
		default:
			errflag++;
		}
	}

	if (errflag)
	{
	    fprintf(stderr, "usage: u [-e] [-n] [-l libdir] entity\n");
	    exit(1);
	}

	while (optind < argc)
	{
		if (edit_flag)
			edit_ent(argv[optind]);
		else
			show_ent(argv[optind]);
		optind++;
	}

	exit(0);
}


int
i_strcmp(char *s, char *t)
{
	char a, b;

	do {
		a = tolower(*s);
		b = tolower(*t);
		s++;
		t++;
		if (a != b)
			return a - b;
	} while (a);

	return 0;
}

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

char *
lookup_name(name)
char *name;
{
	FILE *fp;
	static char kind[LEN];
	static char fnam[LEN];
	static char nam[LEN];
	char *p;
	int i;

	sprintf(buf, "%s/master", libdir);

	fp = fopen(buf, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s: ", buf);
		perror("");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL)
	{
		for (p = buf; *p && *p != '\n'; p++)
			;
		*p = '\0';

		sscanf(buf, "%d %s %s %s", &scan_num, kind, fnam, nam);

		p = buf;
		for (i = 1; i <= 3; i++)
		{
			while (*p && *p != ' ' && *p != '\t')
				p++;
			while (*p && (*p == ' ' || *p == '\t'))
				p++;
		}

		if (i_strcmp(name, p) == 0)
		{
			fclose(fp);
			return fnam;
		}
	}

	fclose(fp);
	return NULL;
}


locate_fnam(num, fnam)
int num;
char *fnam;
{
	char *p;

	sprintf(fnam_path, "%s/%s", libdir, fnam);
	scan_fp = fopen(fnam_path, "r");
	if (scan_fp == NULL)
		return FALSE;

	line_number = 0;
	while (fgets(buf, LEN, scan_fp) != NULL)
	{
		line_number++;
		if (atoi(buf) == num)
			return TRUE;
	}

	scan_fp = NULL;
	return FALSE;
}


char *
lookup_num(num)
int num;
{
	FILE *fp;
	static char kind[LEN];
	static char fnam[LEN];
	static char nam[LEN];
	char *p, *q;

	sprintf(buf, "%s/master", libdir);

	fp = fopen(buf, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s: ", buf);
		perror("");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL)
	{
		sscanf(buf, "%d %s %s %s", &scan_num, kind, fnam, nam);

		if (num == scan_num)
			return fnam;
	}

	fclose(fp);
	return NULL;
}


locate(name)
char *name;
{
	char *s;

	scan_num = code_to_int(name);

	if (scan_num > 0 && !force_name)
		s = lookup_num(scan_num);
	else
		s = lookup_name(name);

	if (s != NULL)
		return locate_fnam(scan_num, s);

	return FALSE;
}


show_ent(name)
char *name;
{

	if (locate(name))
	{
#if 0
		printf("%d\n", scan_num);
#else
		fputs(buf, stdout);
#endif
		while (fgets(buf, LEN, scan_fp) != NULL)
		{
			if (*buf == '\n')
			{
				fclose(scan_fp);
				scan_fp = NULL;
				return;
			}

			fputs(buf, stdout);
		}
	}
}


edit_ent(name)
char *name;
{

	if (locate(name))
	{
		fclose(scan_fp);
		sprintf(buf, "vi +%d %s", line_number, fnam_path);

		system(buf);
	}
}

