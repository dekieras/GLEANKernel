#include "Numeric_utilities.h"

#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>

using std::rand;
using std::string;
using std::ostringstream;
using std::log;

// return a double rounded to the nearest integer value
double round_to_integer(double x)
{
	int i = int(x + 0.5);
	return double(i);
}

string int_to_string(int i)
{
	ostringstream ss;
	ss << i;
	return ss.str();
}

// convert hours, minutes, and seconds to milliseconds (note long integer returned)
long time_convert(long hours, long minutes, long seconds)
{
	 return (seconds*1000 + minutes*60000 + hours*3600000);
}

// convert milliseconds into hours, minutes, seconds, and milliseconds
void time_convert(long time_ms, int& hours, int& minutes, int& seconds, int& milliseconds)
{
	hours = time_ms / 3600000;
	time_ms = time_ms % 3600000;
	minutes = time_ms / 60000;
	time_ms = time_ms % 60000;
	seconds = time_ms / 1000;
	time_ms = time_ms % 1000;
	milliseconds = time_ms;
}

// convert milliseconds into hours, minutes, and double seconds
void time_convert(long time_ms, int& hours, int& minutes, double& seconds)
{
	hours = time_ms / 3600000;
	time_ms = time_ms % 3600000;
	minutes = time_ms / 60000;
	time_ms = time_ms % 60000;
	int int_seconds = time_ms / 1000;
	time_ms = time_ms % 1000;
	int milliseconds = time_ms;
	seconds = double(int_seconds) + double(milliseconds) / 1000.;
}



// compute the base 2 log of the supplied value
double logb2(double x) 
{
	return (log(x) / log(2.0));
}



/* Random variable generation */


// this returns a random integer in the range [0, range)
// that is, 0 ... range - 1
int random_int(int range)
{
	int result;
	do {
		double x = double(rand())/RAND_MAX;
		// note! it is possible for x to end up as 1.00000...
		result = int(double(range) * x);
		// so resample if it happens
		} while (result == range);
		
//	Assert(result != range);
	return result;
//	return int(double(range) * x);
}

double unit_uniform_random_variable()
{
	double x;
	do {
		x = double(rand())/RAND_MAX;
		} while (x == 0.0 || x == 1.0);
	return x;
}

// return a random variable that is uniformly distributed
// on each side of the mean +/- the deviation
double uniform_random_variable(double mean, double deviation)
{
	return 2. * deviation * unit_uniform_random_variable() - deviation + mean;
}

double unit_normal_random_variable()
{
	double sum = -6.0;
	for(int i = 0; i < 12; i++)
		sum += unit_uniform_random_variable();
	return sum;
}

double normal_random_variable(double mean, double sd)
{
	return mean + sd * unit_normal_random_variable();
}

double exponential_random_variable(double theta)
{
	return -theta * log(unit_uniform_random_variable());
}

/*
;return a random variable that follows the exponential distribution
(DEFUN EXPONENTIAL-RANDOM-VARIABLE (THETA)
(* (- THETA) (LOG (UNIT-UNIFORM-RANDOM-VARIABLE)))
)

;Since EPIC times are measured in milliseconds, this delivers an integer
;Theta should be in millisecond units also!
(DEFUN EXPONENTIAL-RANDOM-INTEGER (THETA)
(TRUNCATE (EXPONENTIAL-RANDOM-VARIABLE THETA))
)

;return a random variable that follows the exponential distribution
;plus a minimum floor
(DEFUN FLOORED-EXPONENTIAL-RANDOM-VARIABLE (THETA FLOOR)
(+ (* (- THETA) (LOG (UNIT-UNIFORM-RANDOM-VARIABLE))) FLOOR)
)
*/

double floored_exponential_random_variable(double theta, double floor)
{
	return floor + (-theta) * log(unit_uniform_random_variable());
}

/*
;Since EPIC times are measured in milliseconds, this delivers an integer
;Theta and floor should be in millisecond units also!
(DEFUN FLOORED-EXPONENTIAL-RANDOM-INTEGER (THETA FLOOR)
(TRUNCATE (FLOORED-EXPONENTIAL-RANDOM-VARIABLE THETA FLOOR))
)

;return a gamma random variable with parameters N and THETA
(DEFUN GAMMA-RANDOM-VARIABLE(THETA N)
(LET ((SUM 0.))
(DOTIMES (I N)
  (INCF SUM (EXPONENTIAL-RANDOM-VARIABLE THETA)))
SUM
))
*/

double gamma_random_variable(double theta, int n)
{
	double sum = 0.;
	for(int i = 0; i < n; i++)
		sum += exponential_random_variable(theta);
	return sum;
}

/*
;Since EPIC times are measured in milliseconds, this delivers an integer
;Theta should be in millisecond units also!
(DEFUN GAMMA-RANDOM-INTEGER (THETA N)
(TRUNCATE (GAMMA-RANDOM-VARIABLE THETA N))
)

;return a log-normal random variable with median M and parameter S
(DEFUN LOG-NORMAL-RANDOM-VARIABLE(M S)
(* M (EXP (* S (UNIT-NORMAL-RANDOM-VARIABLE))))
)
*/

double log_normal_random_variable(double m, double s)
{
	return m * exp(s * unit_normal_random_variable());
}

/*
;Since EPIC times are measured in milliseconds, this delivers an integer
;M and S should be in millisecond units also!
(DEFUN LOG-NORMAL-RANDOM-INTEGER (M S)
(TRUNCATE (LOG-NORMAL-RANDOM-VARIABLE M S))
)


;**** RANDOM VARIABLE GENERATORS ***
;this function returns a number in the open interval (0, 1)
;by excluding the zero possibility from (random n)
(DEFUN UNIT-UNIFORM-RANDOM-VARIABLE ()
(LET (X)
(LOOP
  (SETQ X (RANDOM 100000))
  (IF (> X 0) (RETURN)))
(/ (FLOAT X) 100000.)
))

(DEFUN RANDOM-UNIFORM-DEVIATION (R)
(- (* R (UNIT-UNIFORM-RANDOM-VARIABLE)) (/ R 2.))
)

;return a unit normal variate
(DEFUN UNIT-NORMAL-RANDOM-VARIABLE ()
(LET ((SUM 0.))
(DOTIMES (I 12)
  (INCF SUM (UNIT-UNIFORM-RANDOM-VARIABLE)))
(- SUM 6.0)
))

;collect n samples by evaluating dbn-expression, and count them
;in the bins defined by max-value and number-bins.  All values
;above the top bin are included in the top bin.
;assumes that the distribution has a theoretical minimum value of zero, so
;only positive values will be obtained.
;sample:
;  (test-distribution '(LOG-NORMAL-RANDOM-INTEGER 3500 .25) 5000 20 1000) ;note quote!
(defun test-distribution (dbn-expression max-value number-bins n)
(let (bins bin-index step-size x min_x max_x (p 0.) (sum 0.) (totalp 0.))

(setq bins (make-array number-bins :initial-element 0))
(setq step-size (/ max-value number-bins))
(dotimes (i n)
  (setq x (eval dbn-expression))
  (incf sum x)
  (if (or (not min_x) (< x min_x)) (setq min_x x))
  (if (or (not max_x) (> x max_x)) (setq max_x x))
  (setq bin-index (truncate (/ x step-size)))
  (if (> bin-index (1- number-bins)) (setq bin-index (1- number-bins)))
  (incf (aref bins bin-index))
)

(format t "     x    f(x)   F(x)~%")
(setq x step-size)
(dotimes (i number-bins)
  (setq p (/ (aref bins i) n))
  (incf totalp p)
  (format t "   ~4,1f  ~5,3f  ~5,3f~%" x p totalp)
  (setq x (+ x step-size))
)
(format t "~%mean = ~5,2f, min = ~5,2f, max = ~5,2f~%" 
        (/ sum n) min_x max_x)
))

*/
