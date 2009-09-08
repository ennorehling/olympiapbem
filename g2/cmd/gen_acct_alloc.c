
static char *letters = "abcdefghijklmnopqrstuvwxyz";

main() {
  int i, j, k;

  for (i = 0; i < 26; i++)
    for (j = 0; j < 26; j++)
      for (k = 0; k < 10; k++)
        printf("%c%c%d\n", letters[i], letters[j], k);
}
