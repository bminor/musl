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

static unsigned long fail(struct st *st)
{
	st->err = 1;
	return 0;
}

static unsigned long evalexpr(struct st *st, int d);

static unsigned long evalterm(struct st *st, int d)
{
	unsigned long a;
	char *e;
	if (--d < 0) return fail(st);
	st->s = skipspace(st->s);
	if (isdigit(*st->s)) {
		a = strtoul(st->s, &e, 10);
		if (e == st->s || a == -1) return fail(st);
		st->s = skipspace(e);
		return a;
	}
	if (*st->s == 'n') {
		st->s = skipspace(st->s + 1);
		return st->n;
	}
	if (*st->s == '(') {
		st->s++;
		a = evalexpr(st, d);
		if (*st->s != ')') return fail(st);
		st->s = skipspace(st->s + 1);
		return a;
	}
	if (*st->s == '!') {
		st->s++;
		return !evalterm(st, d);
	}
	return fail(st);
}

static unsigned long evalmul(struct st *st, int d)
{
	unsigned long b, a = evalterm(st, d);
	int op;
	for (;;) {
		op = *st->s;
		if (op != '*' && op != '/' && op != '%')
			return a;
		st->s++;
		b = evalterm(st, d);
		if (op == '*') {
			a *= b;
		} else if (!b) {
			return fail(st);
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
	int sub = 0;
	for (;;) {
		a += (sub ? -1 : 1) * evalmul(st, d);
		if (*st->s != '+' && *st->s != '-')
			return a;
		sub = *st->s == '-';
		st->s++;
	}
}

static unsigned long evalrel(struct st *st, int d)
{
	unsigned long b, a = evaladd(st, d);
	int less, eq;
	for (;;) {
		if (*st->s != '<' && *st->s != '>')
			return a;
		less = st->s[0] == '<';
		eq = st->s[1] == '=';
		st->s += 1 + eq;
		b = evaladd(st, d);
		a = (less ? a < b : a > b) || (eq && a == b);
	}
}

static unsigned long evaleq(struct st *st, int d)
{
	unsigned long a = evalrel(st, d);
	int c;
	for (;;) {
		c = st->s[0];
		if ((c != '=' && c != '!') || st->s[1] != '=')
			return a;
		st->s += 2;
		a = (evalrel(st, d) == a) ^ (c == '!');
	}
}

static unsigned long evaland(struct st *st, int d)
{
	unsigned long a = evaleq(st, d);
	for (;;) {
		if (st->s[0] != '&' || st->s[1] != '&')
			return a;
		st->s += 2;
		a = evaleq(st, d) && a;
	}
}

static unsigned long evalor(struct st *st, int d)
{
	unsigned long a = evaland(st, d);
	for (;;) {
		if (st->s[0] != '|' || st->s[1] != '|')
			return a;
		st->s += 2;
		a = evaland(st, d) || a;
	}
}

static unsigned long evalexpr(struct st *st, int d)
{
	unsigned long a1, a2, a3;
	if (--d < 0)
		return fail(st);
	a1 = evalor(st, d-6);
	if (*st->s != '?')
		return a1;
	st->s++;
	a2 = evalexpr(st, d);
	if (*st->s != ':')
		return fail(st);
	st->s++;
	a3 = evalexpr(st, d);
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
