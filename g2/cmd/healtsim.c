
#include	<stdio.h>



unsigned short seed[3];

void
init_random() {
  long l;

  if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) {
    l = time(NULL);
    seed[0] = l & 0xFFFF;
    seed[1] = getpid();
    seed[2] = l >> 16;
  }
}


int
rnd(int low, int high) {
  extern double erand48();

  return (int) (erand48(seed) * (high - low + 1) + low);
}


int how_long;

int
sim(int a) {

  how_long = 0;

  while (a > 0 && a < 100) {
    if (rnd(1, 100) <= a)
      a += 25;
    else
      a -= 25;

    how_long++;
  }

  if (a <= 0)
    return 0;
  return 1;
}


run(int a) {
  int count = 0;
  int i;
  int time_sum = 0;
  int n = 0;
  int max_time = 0;

  count = 0;
  for (i = 0; i <= 10000; i++) {
    if (!sim(a)) {
      count++;
      time_sum += how_long;
      if (how_long > max_time)
        max_time = how_long;
      n++;
    }
  }

  printf("%10d  died %2d%%, ave time=%d, max time=%d\n", a, count / 100,
         time_sum / n, max_time);
}



main() {
  int a;
  char buf[100];
  int i;

  init_random();

  for (i = 95; i > 0; i -= 5)
    run(i);
}
