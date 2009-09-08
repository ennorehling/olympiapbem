
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

  strcpy(libdir, "/alpha");

  while ((c = getopt(argc, argv, "el:n")) != -1) {
    switch (c) {
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

  if (errflag) {
    fprintf(stderr, "usage: u [-e] [-n] [-l libdir] entity\n");
    exit(1);
  }

  while (optind < argc) {
    if (edit_flag)
      edit_ent(argv[optind]);
    else
      show_ent(argv[optind]);
    optind++;
  }

  exit(0);
}


char *
lookup_name(name)
     char *name;
{
  FILE *fp;
  static char kind[LEN];
  static char fnam[LEN];
  static char nam[LEN];
  int l = strlen(name);

  sprintf(buf, "%s/master", libdir);

  fp = fopen(buf, "r");
  if (fp == NULL) {
    fprintf(stderr, "can't open %s: ", buf);
    perror("");
    exit(1);
  }

  while (fgets(buf, LEN, fp) != NULL) {
    sscanf(buf, "%d %s %s %s", &scan_num, kind, fnam, nam);

    if (strincmp(name, nam, l) == 0) {
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
  while (fgets(buf, LEN, scan_fp) != NULL) {
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
  if (fp == NULL) {
    fprintf(stderr, "can't open %s: ", buf);
    perror("");
    exit(1);
  }

  while (fgets(buf, LEN, fp) != NULL) {
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

  scan_num = atoi(name);

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

  if (locate(name)) {
#if 0
    printf("%d\n", scan_num);
#else
    fputs(buf, stdout);
#endif
    while (fgets(buf, LEN, scan_fp) != NULL) {
      if (*buf == '\n') {
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

  if (locate(name)) {
    fclose(scan_fp);
    sprintf(buf, "vi +%d %s", line_number, fnam_path);

    system(buf);
  }
}


strincmp(s, t, n)
     char *s;
     char *t;
     int n;
{
  char a, b;
  do {
    a = tolower(*s);
    b = tolower(*t);
    if (a != b)
      return a - b;
    s++;
    t++;
    n--;
  } while (a && n > 0);
  return 0;
}
