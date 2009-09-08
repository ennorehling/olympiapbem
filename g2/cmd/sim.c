
#include	<stdio.h>



unsigned short seed[3];

void
init_random()
{
	long l;

	if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0)
	{
		l = time(NULL);
		seed[0] = l & 0xFFFF;
		seed[1] = getpid();
		seed[2] = l >> 16;
	}
}


int
rnd(int low, int high)
{
	extern double erand48();

	return (int) (erand48(seed) * (high - low + 1) + low);
}


resolv(int king, int pawn, int npawn)
{
	static int alt = 0;

	alt = !alt;

	while (npawn > 0)
	{
		if (alt)
		{
			if (rnd(1, king+pawn) <= king)
				npawn--;

			if (npawn > 0 && rnd(1, king+pawn) <= pawn)
				return 0;		/* pawns got king */
		}
		else
		{
			if (rnd(1, king+pawn) <= pawn)
				return 0;		/* pawns got king */

			if (rnd(1, king+pawn) <= king)
				npawn--;
		}
	}

	return 1;	/* king wins */
}


sim(int king, int pawn, int npawn)
{
	int king_cnt = 0;
	int pawn_cnt = 0;
	int i;

	for (i = 1; i <= 500; i++)
	{
		if (resolv(king, pawn, npawn))
			king_cnt++;
		else
			pawn_cnt++;
	}

	return king_cnt > pawn_cnt;
}


main()
{
	int king, pawn;
	char buf[100];
	int i;

	init_random();

	printf("king: ");
	fflush(stdout);
	fgets(buf, 100, stdin);
	king = atoi(buf);

	printf("pawn: ");
	fflush(stdout);
	fgets(buf, 100, stdin);
	pawn = atoi(buf);

	for (i = 1; i <= 1000; i++)
	{
		if (!sim(king, pawn, i))
		{
			printf("%d-%d\n", i-1, i);
			exit(0);
		}
	}

	printf("???\n");
	exit(1);
}

