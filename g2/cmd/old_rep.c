
#include	<stdio.h>

#define	LEN	256


char *repdir = ".";

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp;
	char fnam[LEN];
	char buf[LEN];

	if (argc < 2)
	{
		fprintf(stderr, "usage: %s report-dir\n", argv[0]);
		exit(1);
	}

	repdir = argv[1];

	sprintf(fnam, "%s/0", repdir);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s: ", fnam);
		perror("");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL)
	{
		if (strncmp(buf, "#include", 8) == 0)
			include_file(&buf[8]);
		else
			fputs(buf, stdout);
	}

	fclose(fp);
}


include_file(s)
char *s;
{
	char buf[LEN];
	char fnam[LEN];
	FILE *fp;
	char *p;

	while (*s && (*s == ' ' || *s == '\t'))
		s++;

	for (p = s; *p && *p != '\n'; p++)
		;
	*p = '\0';


	sprintf(fnam, "%s/%s", repdir, s);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s: ", fnam);
		perror("");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL)
	{
		fputs(buf, stdout);
	}

	fclose(fp);
}

