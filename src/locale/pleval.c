#include <stdlib.h>
#include <ctype.h>

/*
grammar:

Start = Expr ';'
Expr  = Or | Or '?' Expr ':' Expr
Or    = And | Or '||' And
And   = Eq | And '&&' Eq
Eq    = Rel | Eq '==' Rel | Eq '!=' Rel
Rel   = Add | Rel '<=' Add | Rel '>=' Add | Rel '<' Add | Rel '>' Add
Add   = Mul | Add '+' Mul | Add '-' Mul
Mul   = Term | Mul '*' Term | Mul '/' Term | Mul '%' Term
Term  = '(' Expr ')' | '!' Term | decimal | 'n'

internals:

recursive descent expression evaluator with stack depth limit.
eval* functions return the value of the subexpression and set
the current string pointer to the next non-space char.
*/

struct st {
	const char *s;
	unsigned long n;
	int err;
};

static const char *skipspace(const char *s)
{
	while (isspace(*s)) s++;
	return s;
}

static unsigned long evalconst(struct st *st)
{
	char *e;
	unsigned long n;
	n = strtoul(st->s, &e, 10);
	if (!isdigit(*st->s) || e == st->s || n == -1)
		st->err = 1;
	st->s = skipspace(e);
	return n;
}

static unsigned long evalexpr(struct st *st, int d);

static unsigned long evalterm(struct st *st, int d)
{
	unsigned long a;
	if (d <= 0) {
		st->err = 1;
		return 0;
	}
	st->s = skipspace(st->s);
	if (*st->s == '!') {
		st->s++;
		return !evalterm(st, d-1);
	}
	if (*st->s == '(') {
		st->s++;
		a = evalexpr(st, d-1);
		if (*st->s != ')') {
			st->err = 1;
			return 0;
		}
		st->s = skipspace(st->s + 1);
		return a;
	}
	if (*st->s == 'n') {
		st->s = skipspace(st->s + 1);
		return st->n;
	}
	return evalconst(st);
}

static unsigned long evalmul(struct st *st, int d)
{
	unsigned long b, a = evalterm(st, d-1);
	int op;
	for (;;) {
		op = *st->s;
		if (op != '*' && op != '/' && op != '%')
			return a;
		st->s++;
		b = evalterm(st, d-1);
		if (op == '*') {
			a *= b;
		} else if (!b) {
			st->err = 1;
			return 0;
		} else if (op == '%') {
			a %= b;
		} else {
			a /= b;
		}
	}
}

static unsigned long evaladd(struct st *st, int d)
{
	unsigned long a = 0;
	int add = 1;
	for (;;) {
		a += (add?1:-1) * evalmul(st, d-1);
		if (*st->s != '+' && *st->s != '-')
			return a;
		add = *st->s == '+';
		st->s++;
	}
}

static unsigned long evalrel(struct st *st, int d)
{
	unsigned long b, a = evaladd(st, d-1);
	int less, eq;
	for (;;) {
		if (*st->s != '<' && *st->s != '>')
			return a;
		less = st->s[0] == '<';
		eq = st->s[1] == '=';
		st->s += 1 + eq;
		b = evaladd(st, d-1);
		a = (less ? a < b : a > b) || (eq && a == b);
	}
}

static unsigned long evaleq(struct st *st, int d)
{
	unsigned long a = evalrel(st, d-1);
	int neg;
	for (;;) {
		if ((st->s[0] != '=' && st->s[0] != '!') || st->s[1] != '=')
			return a;
		neg = st->s[0] == '!';
		st->s += 2;
		a = evalrel(st, d-1) == a;
		a ^= neg;
	}
}

static unsigned long evaland(struct st *st, int d)
{
	unsigned long a = evaleq(st, d-1);
	for (;;) {
		if (st->s[0] != '&' || st->s[1] != '&')
			return a;
		st->s += 2;
		a = evaleq(st, d-1) && a;
	}
}

static unsigned long evalor(struct st *st, int d)
{
	unsigned long a = evaland(st, d-1);
	for (;;) {
		if (st->s[0] != '|' || st->s[1] != '|')
			return a;
		st->s += 2;
		a = evaland(st, d-1) || a;
	}
}

static unsigned long evalexpr(struct st *st, int d)
{
	unsigned long a1, a2, a3;
	if (d <= 0) {
		st->err = 1;
		return 0;
	}
	a1 = evalor(st, d-1);
	if (*st->s != '?')
		return a1;
	st->s++;
	a2 = evalexpr(st, d-1);
	if (*st->s != ':') {
		st->err = 1;
		return 0;
	}
	st->s++;
	a3 = evalexpr(st, d-1);
	return a1 ? a2 : a3;
}

unsigned long __pleval(const char *s, unsigned long n)
{
	unsigned long a;
	struct st st;
	st.s = s;
	st.n = n;
	st.err = 0;
	a = evalexpr(&st, 100);
	if (st.err || *st.s != ';')
		return -1;
	return a;
}
