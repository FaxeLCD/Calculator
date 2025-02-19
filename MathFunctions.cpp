﻿#include <string>

//	константы
int iterations = 32;
const unsigned long long	$inf_0   = 0x7ff0000000000000,
							$n_inf_0 = 0xfff0000000000000,
							$NaN_0   = 0x7fffffffffffffff,
							$n_NaN_0 = 0xffffffffffffffff;
const double	pi  = 3.14159265358979,
				tau = 6.28318530717959,
				e   = 2.71828182845905,
				$inf   = *(double*)&$inf_0,
				$n_inf = *(double*)&$n_inf_0,
				$NaN   = *(double*)&$NaN_0,
				$n_NaN = *(double*)&$n_NaN_0;

double  power(double x, double y);
double  arccos(double x);
double  factorial(double x);
double D(double x);

//	типы
struct complex {
	double R = 0;
	double i = 0;
	double mod() {
		if (!isNum()) return $NaN;
		if (R == 0)
			if (i == 0)		return  0;
			else if (i > 0)	return  i;
			else			return -i;
		else if (i == 0)
			if (R > 0)		return  R;
			else			return -R;
		else if (R == $n_inf || R == $inf || i == $n_inf || i == $inf) 
							return $inf;
		else				return power(R * R + i * i, 0.5);
	}
	double angle() {
		R = arccos(R / mod());
		if (i < 0) R = -R;
		return R;
	}
	std::string toString() {
		if (!isNum()) return "NaN";
		if (mod() == $inf)			return "inf";
		if ((R == 0) && (i == 0))	return "0";
		std::string str = "";
		if (R != 0)			str += std::to_string(R);
		if ((i > 0) &&
			(R != 0))		str += '+';
		if ((i != -1) &&
			(i != 0) &&
			(i != 1))		str += std::to_string(i);
		else if (i == -1)	str += '-';
		if (i != 0)			str += 'i';
		return str;
	}
	double toDouble() {
		return D(R) * R * R / mod();
	}
	bool isNum() {
		if ((R >= 0 || R < 0) && (i >= 0 || i < 0)) return true;
		return false;
	}
	complex() {}
	complex(double num) {
		R = num;
	}
	complex(double num, double num2) {
		R = num;
		i = num2;
	}
};

complex power(complex x, complex y);
complex gamma_(complex x);
double  arccos(double x);
double  fct(double x);

//	комплексные константы
const complex	i(0, 1),
				infinite($inf);

//	операторы
complex operator + (complex x, complex y) {
	complex z;
	z.R = x.R + y.R;
	z.i = x.i + y.i;
	return z;
}
complex operator - (complex x, complex y) {
	complex z;
	z.R = x.R - y.R;
	z.i = x.i - y.i;
	return z;
}
complex operator - (complex x) {
	complex z;
	z.R = -x.R;
	z.i = -x.i;
	return z;
}
complex operator * (complex x, complex y) {
	complex z;
	z.R = (x.R * y.R) - (x.i * y.i);
	z.i = (x.i * y.R) + (x.R * y.i);
	return z;
}
complex operator / (complex x, complex y) {
	complex z;
	if (y.mod() == 0) return infinite;
	if (y.mod() == $inf) return 0;
	z.R = ((x.R * y.R) + (x.i * y.i)) / (y.R * y.R + y.i * y.i);
	z.i = ((x.i * y.R) - (x.R * y.i)) / (y.R * y.R + y.i * y.i);
	return z;
}
complex operator % (complex x, complex y) {
	return x - y * (int)(x / y).R;
}
bool    operator ==(complex x, complex y) {
	if (x.R == y.R && x.i == y.i) return true;
	return false;
}
bool    operator < (complex x, complex y) {
	if (x.mod() < y.mod()) return true;
	return false;
}
bool    operator > (complex x, complex y) {
	return y < x;
}
bool    operator <=(complex x, complex y) {
	return (x == y) || (x < y);
}
bool    operator >=(complex x, complex y) {
	return (x == y) || (x > y);
}

//	functions
double D(double x) {
	if (x >= 0) return 1;
	if (x < 0) return -1;
	return 0;
}
int Round(double x) {
	return (int)(x + 0.5);
}
double part(double x) {
	return x - (int)x;
}

double  ln_(double x) {
	long double
		G = x / (x + 1),
		m = G;
	for (int n = 2; n < iterations; n++) {
		m = m + power(G, (double)n) / n;
	}
	return m;
}
double  ln2_(double x) {
	return ln_(x) * 1.44269504088896;
}
double  ln( double x) {
	if (x == $inf) return $inf;
	if (x == $n_inf) return $inf;
	if (x == 0) return $n_inf;
	unsigned long long	i = *(unsigned long long*)&x;
	const long long		E = (i >> 52) % 2048 - 1023;																//	экспонента [-1023:1024]
	const double		M = (double)(i % ((unsigned long long)1 << 52)) / (double)((unsigned long long)1 << 52);	//	мантисса [0:1)
	return ln_(M) + 0.6931471805599453 * (double)E;	//	ln(M) + ln(E)		ln(E) = ln(2) * E
}
complex ln(complex x) {
	if ((x.R >= 0) && (x.i == 0)) return ln(x.R);
	if (x.mod() == $inf) return infinite;
	complex z(ln(x.R * x.R + x.i * x.i) / 2, D(x.i) * arccos(x.R / x.mod()));
	return z;
}

double  exp( double x) {
	if (x == 0) return 1;
	if (709 < x) return $inf;
	if (x < -709) return 0;
	long double drob = x - (long long)x, cel = 1;
	drob = 1 + (drob / ((unsigned long long)1 << iterations));
	for (int m = 0; m < iterations; m++) drob = drob * drob;
	if (x > 0)  for (long long E = (long long)x; E > 0; E--)	cel = cel * e;
	else		for (long long E = (long long)x; E < 0; E++)	cel = cel / e;
	return drob * cel;
}
complex exp(complex x) {
	if (x.i == 0) return exp(x.R);
	if (x == 0) return 1;
	if (709 < x.R) return $inf;
	if (x.R < -709) return 0;
	return exp(x.R) * (cos(x.i) + i * sin(x.i));
}
double  fast_exp( double x) {
	long double y;
	if (0 < x) {
		y = 1 + ((long double)x / ((unsigned long long)1 << iterations));
		for (int m = 0; m < iterations; m++) y = y * y;
	}
	else {
		y = 1 - ((long double)x / ((unsigned long long)1 << iterations));
		for (int m = 0; m < iterations; m++) y = y * y;
		y = 1 / y;
	}
	return y;
}
complex fast_exp(complex x) {
	if (x.i == 0) return fast_exp(x.R);
	long double y;
	if (0 < x.R) {
		y = 1 + ((long double)x.R / ((unsigned long long)1 << iterations));
		for (int m = 0; m < iterations; m++) y = y * y;
	}
	else {
		y = 1 - ((long double)x.R / ((unsigned long long)1 << iterations));
		for (int m = 0; m < iterations; m++) y = y * y;
		y = 1 / y;
	}
	return y * (cos(x.i) + i * sin(x.i));
}
complex taylor_exp(complex x) {
	complex z = 1 + x;
	for (int k = 2; k <= iterations; k++) {
		z = z + power(x, k) / factorial(k);
	}
	return z;
}

double  power( double x,  double y) {
	if (y == 0) return 1;
	if (y == $inf) {
		if ((x > 1) || (x < -1))return $inf;
		else if (x == 1)		return 1;
		else					return 0;
	}
	if (x == 0) return 0;
	if (y == (long long)y) {
		double z = x;
		if (y > 0)  for (long long i = (long long)y; 1 < i; i--) z = z * x;
		else		for (long long i = (long long)y; i < 1; i++) z = z / x;
		return z;
	}
	return fast_exp(y * ln(x));
}
complex power(complex x, int y) {
	if (y == 0) return 1;
	if (y == $inf || y == $n_inf) {
		if (x.mod() > 1)		return infinite;
		else if (x.mod() == 1)	return 1;
		else					return 0;
	}
	if (x == 0) return 0;
	complex z = x;
	if (y > 0)	for (long long i = (long long)y; 1 < i; i--) z = z * x;
	else		for (long long i = (long long)y; i < 1; i++) z = z / x;
	return z;
}
complex power(complex x, complex y) {
	if (x.R >= 0 && x.i == 0 && y.i == 0) return power(x.R, y.R);
	if (y == 0) return 1;
	if (y.mod() == $inf) {
		if (x.mod() > 1)		return infinite;
		else if (x.mod() == 1)	return 1;
		else					return 0;
	}
	if (x == 0) return 0;
	if ((y.i == 0) && (y.R == (long long)y.R)) {
		complex z = x;
		if (y.R > 0) for (long long i = (long long)y.R; 1 < i; i--) z = z * x;
		else		 for (long long i = (long long)y.R; i < 1; i++) z = z / x;
		return z;
	}
	return fast_exp(y * ln(x));
}

double  factorial( double x) {
	if (x == 0) return 1;
	if (x == (long long)x) {
		for (long long i = (long long)x - 1; 1 < i; i--) x = x * i;
		return x;
	}
	x++;
	return 2.5066282746310003 / fast_exp(x + ln(x) / 2) * power(x + 1 / (12 * x - 1 / (10 * x)), x);
}
complex factorial(complex x) {
	if (x.mod() == 0) return 1;
	if ((x.R == (long long)x.R) && (x.i == 0)) {
		for (long long i = (long long)x.R - 1; 1 < i; i--) x.R = x.R * i;
		return x;
	}
	return gamma_(x + 1);
}
double  fct( double x) {
	if (!((x < 6500) || (x > -6500))) return $inf;
	if (1 < x) return fct(x - 1) * x;
	if (x < 0) return fct(x + 1) / (x + 1);
	return factorial(x);
}
complex fct(complex x) {
	if (x.i == 0) return fct(x.R);
	if (!(x.R < 6500)) return infinite;
	if (x.R < -6500) return 0;
	if (1 < x.R) return fct(x - 1) * x;
	if (x.R < 0) return fct(x + 1) / (x + 1);
	return factorial(x);
}

double  cos( double x) {
	x = fmod(x, tau);
	double c = 1;
	for (int n = 1; n <= iterations / 2; n++) {
		c += power(x, 4 * (double)n) / factorial(4 * n) - power(x, 4 * (double)n - 2) / factorial(4 * n - 2);
	}
	return c;
}
complex cos(complex x) {
	if (x.i == 0) return cos(x.R);
	x = x % tau;
	complex c = 1;
	for (int n = 1; n <= iterations / 2; n++) {
		c = c + power(x, 4 * n) / factorial(4 * n) - power(x, 4 * n - 2) / factorial(4 * n - 2);
	}
	return c;
}
double  arccos( double x) {
	if (x == $inf) return $inf;
	bool negative = false;
	if (x < 0) {
		negative = true;
		x = -x;
	}
	double t = 2 - 2 * x;
	for (int i = 0; i <= iterations / 2; i++) {
		double t2 = 0.5;
		for (int j = 1; j <= iterations / 2; j++) {
			if (j % 2 == 0) t2 = t2 + power(t, (double)j) / factorial(2 * (j + 1));
			else			t2 = t2 - power(t, (double)j) / factorial(2 * (j + 1));
		}
		t = (1 - x) / t2;
	}
	if (negative) return pi - power(t, 0.5);
	return power(t, 0.5);
}
complex arccos(complex x) {
	if ((x.i == 0) && (-1 <= x.R) && (x.R <= 1)) return arccos(x.R);
	complex z = -i * ln(x + i * power(1 - x * x, 0.5));
	if (x.i == 0)
		if(x.R > 1) z.R = 0;
		else if (x.R < -1) z.R = pi;
	return z;
}

double  sin( double x) {
	return cos(x - 1.5707963267949);
}
complex sin(complex x) {
	if (x.i == 0) return cos(x.R - 1.5707963267949);
	return cos(x - 1.5707963267949);
}

complex cycloid(complex x) {
	complex t = sin(x);
	for (int i = 0; i <= iterations / 2; i++) t = x + sin(t);
	return 1 - cos(t);
}
complex arccyc(complex x) {
	return arccos(1 - x) - power(2 * x - x * x, 0.5);
}

complex gamma_seed(complex x) {
	return power(0.5 * x, x - 1);
}
complex gamma(complex x) {
	int min = 1, max = 2;
	if ((min <= x.R) && (x.R <= max)) {
		complex res = gamma_seed(x);
		for (int n = 0; n < iterations / 2; n++) {
			res = gamma_seed(0.8888888888888889 * (x - 1) + 2) / x + res;
			res = (gamma_seed(x) + res) / 3;
		}
		return res;
	}
	if (max < x.R) return (x - 1) * gamma(x - 1);
	if (x.R < min) return gamma(x + 1) / x;
	return $NaN;
}
complex gamma_(complex x) {
	//	Г(x) = sqrt(2π)/(e^x*sqrt(x))*(x+1/(12x-1/10x))^x		= (sqrt(2π)*e^(-x)*sqrt(1/x)*(x+1/(12x-1/10x))^x)
	return 2.5066282746310003 / fast_exp(x + ln(x) / 2) * power(x + 1 / (12 * x - 1 / (10 * x)), x);
}
complex gamma_s(complex x) {
	complex res = gamma_seed(x);
	for (int n = 0; n < iterations / 2; n++) {
		res = gamma_seed(0.8888888888888889 * (x - 1) + 2) / x + res;
		res = (gamma_seed(x) + res) / 3;
	}
	return res;
}

complex zeta(complex x) {
	complex z = 1;
	for (int n = 2; n <= iterations; n++) {
		z = z + power(n, -x);
	}
	return z;
}