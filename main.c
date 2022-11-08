
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define ONEMILL 1000000

typedef struct bigint {
	int arr[10000];
	int sz;
}bigint;

void sumbig(bigint *b1, bigint* b2, bigint *res) {

	if (b1->sz > b2->sz) {
		//swap
		bigint *temp = b1;
		b1 = b2;
		b2 = temp;
	}

	int i;
	int rem = 0, v;
	for (i = 0; i < b1->sz; i++) {
		v = b1->arr[i] + b2->arr[i] + rem;
		if (v >= ONEMILL) {
			res->arr[i] = v % ONEMILL;
			rem = v / ONEMILL;
		}
		else {
			res->arr[i] = v;
			rem = 0;
		}
	}

	for (; i < b2->sz; i++) {
		v = b2->arr[i] + rem;
		if (v >= ONEMILL) {
			res->arr[i] = v % ONEMILL;
			rem = v / ONEMILL;
		}
		else {
			res->arr[i] = v;
			rem = 0;
		}
	}

	for (; rem >= ONEMILL; i++) {
		res->arr[i] = rem % ONEMILL;
		rem /= ONEMILL;
	}

	if (rem)
		res->arr[i++] = rem;

	res->sz = i;
}

void multipbig(bigint *b1, int64_t f, bigint *res) {
	int rem = 0, i;
	uint64_t v;
	for (i = 0; i < b1->sz; i++) {
		v = b1->arr[i] * f + rem;
		if (v >= ONEMILL) {
			res->arr[i] = v % ONEMILL;
			rem = v / ONEMILL;
		}
		else {
			res->arr[i] = v;
			rem = 0;
		}
	}
	while (rem >= ONEMILL) {
		res->arr[i++] = rem % ONEMILL;
		rem /= ONEMILL;
	}

	if (rem)
		res->arr[i++] = rem;

	res->sz = i;
}

int sumdig(bigint *b) {
	int sum = 0, v;
	for (int i = 0; i < b->sz; i++) {
		v = b->arr[i];
		while (v > 9) {
			sum += v % 10;
			v /= 10;
		}
		sum += v;
	}

	return sum;
}

void printbig(bigint* b) {
	for (int i = b->sz - 1; i >= 0; i--) {
		printf("%d", b->arr[i]);
	}
	puts("");
}

typedef struct acoeff {
	int a0;
	int aperiod[1000];
	int period;
}acoeff;

int contfracperiod(int d, acoeff* ac) {
	int a0, a, b, c, b0, c0, i = 0;
	a0 = sqrt(d);

	c = d - a0*a0;
	if (!c) return 0;
	
	ac->a0 = a0;
	c0 = c;
	b = b0 = a0;
	int a02 = a0 << 1;
	do {
		a = (a0 + b) / c;
		b = a*c - b;
		c = (d - b*b) / c;
		ac->aperiod[i++] = a;
	} while (a != a02);
	ac->period = i;

	return i;
}

int arka(const acoeff* ac, int k) {
	if (k<= ac->period) return (k == 0)?ac->a0:ac->aperiod[k-1];
	
	return ac->aperiod[(k - 1) % ac->period];	
}

int compare(bigint *b1, bigint* b2) {
	int cmp = b1->sz - b2->sz;
	if (cmp) return cmp;

	for (int i = b1->sz - 1; i >= 0; i--) {
		cmp = b1->arr[i] - b2->arr[i];
		if (cmp) return cmp;
	}

	return 0;
}

void bigintcpy(bigint* dst, bigint *ori) {
	dst->sz = ori->sz;
	memcpy(dst->arr, ori->arr, dst->sz*(sizeof(int)));
}

bigint bufbigint[5];
int nbigint;
bigint *newbigint() {
	bigint* nb = (bufbigint + nbigint);
	nbigint++;
	return nb;
}

//a < 1.000.000
void setbigint(bigint *b, int a) {
	b->arr[0] = a;
	b->sz = 1;
}

int main()
{
	bigint *ans = newbigint();
	bigint *h2 = newbigint();
	bigint *h1 = newbigint();
	bigint *h = newbigint();
	bigint *h1a = newbigint();
	bigint *temp;
	acoeff ac;

	int n;
	scanf("%d", &n);

	int period, ak;
	int dans = 0;

	setbigint(ans, 0);

	for (int d = 2; d <= n; d++) {
		if (!(period=contfracperiod(d, &ac))) continue;

		setbigint(h1, 0);
		setbigint(h, 1);

		int ktarget = ((period & 1) ? 2 * period - 1 : period - 1);
		for (int k = 0; k <= ktarget; k++) {
			temp = h2; h2 = h1; h1 = h; h = temp; 
			ak = arka(&ac, k);
			multipbig(h1, ak, h1a);
			sumbig(h1a, h2, h);
		}
		printf("%d->", d);
		printbig(h);
		if (compare(h, ans) > 0) {
			bigintcpy(ans, h);
			dans = d;
		}
	}

	printf("%d\n", dans);

	return 0;
}
