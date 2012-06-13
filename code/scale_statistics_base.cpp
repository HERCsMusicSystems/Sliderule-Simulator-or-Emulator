///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2010 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "scale_statistics_base.h"

#define _MAXITER  1000
#define _ACCURACY 0.0000000000001
#define _OOBVALUE 1000000000.0

/* GJM -- Statistical scales base */
// Generic Hyper Geometric Function
double StatisticsBase :: GenericHypGeoFunc(int n1, double n1array[], int n2, double n2array[], double x)
{
  double value, prevValue, nextSum;
  double n = 1.0;
  int i;
  value = x;
  for (i=0; i<=n1; i++) value *= n1array[i];
  for (i=0; i<=n2; i++) value /= n2array[i];
  //initial value
  nextSum = value;
  value += 1.0;
  //next sum to be added is previous sum * x * each( n1 array elem) / each (n2array elem)
  do
  {
    nextSum *= x;
    for (i=0; i<=n1; i++) { n1array[i] += 1.0; nextSum *= n1array[i]; }
    for (i=0; i<=n2; i++) { n2array[i] += 1.0; nextSum /= n2array[i]; }
    n += 1.0;
    nextSum /= n;
    prevValue = value;
    value += nextSum;
  } while ((value != prevValue) && (n <= _MAXITER));
  return value;
}
// Hyper Geometric Function 2,1
double StatisticsBase :: HypGeoFunc(double a, double b, double c, double x)
{
  double p1[] = {a, b};
  double p2[] = {c};
  return GenericHypGeoFunc(1, p1, 0, p2, x);
}
// Incomplete Gamma and Inverse Incomplete Gamma functions for Norm, Chi Square, and Gamma distributions
double StatisticsBase :: IncompleteGammaFunction(double a, double x)
{
  double result, sum, sum2, prod;
  bool finished = false;

  result = pow(x, a);
  result *= exp(-x) / exp(LnGammaF(a+1));
  sum = sum2 = prod = 1.0;
  for (int k = 1; !finished; k++)
  {
    prod *= x / (a + (double) k);
    sum += prod;
    if (sum == sum2) finished = true;
    sum2 = sum;
  }
  result *= sum;
  return result;
}
double StatisticsBase :: InvIncompleteGammaFunction(double a, double x)
{
      double low, high, mid, midgma, deltamax, temp;
  int loop;

      if ((x < 0.0) || (x >= 1.0)) return _OOBVALUE;
  if (x == 0.0) return 0.0;
      low = 0.0;
  high = 1.0;
  temp = IncompleteGammaFunction(a, high);
  while (temp < x)
  {
    low = high;
    high = high * 2.0;
    temp = IncompleteGammaFunction(a, high);
  }
      deltamax = high * _ACCURACY;
      double delta = high - low;
  loop = 0;
      while ((abs(delta) > deltamax) && (loop <= _MAXITER))
      {
          mid = (low + high) / 2.0;
          midgma = IncompleteGammaFunction(a, mid);
          if (midgma > x) high = mid;
          else if (midgma < x) low = mid;
          delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
      }
      return mid;
}
// Log Gamma and Inv Log Gamma functions
double StatisticsBase :: LogGammaF(double x) { return LnGammaF(x)/log(10.0); }
double StatisticsBase :: InvLogGammaF(double x) { return InvLnGammaF(x * log(10.0)); }
double StatisticsBase :: LnGammaF(double x) {
      double xx = x;
      double c[] =
      {
         1.0/12.0,
        -1.0/360.0,
        1.0/1260.0,
        -1.0/1680.0,
        1.0/1188.0,
        -691.0/360360.0,
        1.0/156.0,
        -3617.0/122400.0
      };
      int uppity = 0;
      if (x <= 0) return (double) _OOBVALUE;
      if (x < 20) uppity = (int) (20 - x);
      xx = x + uppity;
      double z = 1.0 / (xx * xx);
      double sum = c[7];
      for (int i = 6; i >= 0; i--)
      {
          sum *= z;
          sum += c[i];
      }
      double series = sum / xx;
      double halfLogTwoPi = 0.91893853320467274178032973640562;
      double logGamma = (xx - 0.5) * log(xx) - xx + halfLogTwoPi + series;
      for (int i = uppity; i > 0; i--)
      {
          logGamma -= log(xx - i);
      }
          return logGamma;
  }

double StatisticsBase :: InvLnGammaF(double x)
  { // must be >= 0 [>= LnGamma(2)]
      double low, high, mid, midgma, deltamax;
  int loop;

      if (x < 0) return _OOBVALUE;
      low = 2.0;
      if (x > 30) high = x;
      else high = 30.0;
      deltamax = high * _ACCURACY;
      double delta = 1.0;
  loop = 0;
      while ((abs(delta) > deltamax) && (loop <= _MAXITER))
      {
          mid = (low + high) / 2.0;
          midgma = LnGammaF(mid);
          if (midgma > x) high = mid;
          else if (midgma < x) low = mid;
          delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
      }
      return mid;
  }

double StatisticsBase :: BetaF(double a, double b) { return exp(LnGammaF(a) + LnGammaF(b) - LnGammaF(a+b)); }
// Incomplete Gamma Function based distributions
double StatisticsBase :: ChiSquareDist(double v, double x) { return IncompleteGammaFunction(v/2.0, x/2.0); }
double StatisticsBase :: InvChiSquareDist(double v, double x) { return InvIncompleteGammaFunction(v/2.0, x) * 2.0; }
double StatisticsBase :: ChiSquareDegOfFreedom(double x, double a)
{
      double low, high, mid, midgma, deltamax, temp;
  int loop;

      if ((a < 0.0) || (a > 1.0)) return _OOBVALUE;
  if ((x == 0.0) || (a == 0.0)) return 0.0;
      low = 0.0;
  high = 1.0;
  temp = ChiSquareDist(high, x);
  while (temp > a)
  {
    low = high;
    high = high * 2.0;
    temp = ChiSquareDist(high, x);
  }
      deltamax = high * _ACCURACY;
      double delta = high - low;
  loop = 0;
      while ((abs(delta) > deltamax) && (loop < _MAXITER))
      {
          mid = (low + high) / 2.0;
          midgma = ChiSquareDist(mid, x);
          if (midgma < a) high = mid;
          else if (midgma > a) low = mid;
          delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
      }
      return mid;
}
double StatisticsBase :: StudentsTDegOfFreedom(double x, double a)
{
      double low, high, mid, midst, deltamax, temp;
  int loop;

      if ((a < 0.0) || (a > 1.0)) return _OOBVALUE;
  if ((x == 0.0) || (a == 0.0)) return 0.0;
      low = 0.0;
  high = 1.0;
  temp = StudentsTDist(high, x);
  while (temp < a)
  {
    low = high;
    high = high * 2.0;
    temp = StudentsTDist(high, x);
  }
      deltamax = high * _ACCURACY;
      double delta = high - low;
  loop = 0;
      while ((abs(delta) > deltamax) && (loop < _MAXITER))
      {
          mid = (low + high) / 2.0;
          midst = StudentsTDist(mid, x);
          if (midst > a) high = mid; else low = mid;
          delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
      }
      return mid;
}
double StatisticsBase :: NormalDist(double x) { return IncompleteGammaFunction(0.5, x*x/2.0); }
double StatisticsBase :: InvNormalDist(double x) { return sqrt(InvIncompleteGammaFunction(0.5, x) * 2.0); }
double StatisticsBase :: GammaDist(double a, double b, double x) { return IncompleteGammaFunction(b, a * x); }
double StatisticsBase :: InvGammaDist(double a, double b, double x) { return InvIncompleteGammaFunction(b, x) / a; }
// Incomplete Beta and Invert Incomplete Beta functions for StudentsT, F, and Beta distributions
double StatisticsBase :: IncompleteBetaFunction(double a, double b, double x)
{
  if (x < 0.0) return 0.0;
  else return ( pow(x, a) / a ) * HypGeoFunc(a, 1.0-b, a+1.0, x);
}
double StatisticsBase :: InvIncompleteBetaFunction(double a, double b, double x)
{
      double low, high, mid, midgma, deltamax, temp;
  int loop;

  if (x <= 0.0) return 0.0;
      low = 0.0;
  high = 1.0;
  temp = IncompleteBetaFunction(a, b, high);
  while (temp < x)
  {
    low = high;
    high = high * 2.0;
    temp = IncompleteBetaFunction(a, b, high);
  }
      deltamax = high * _ACCURACY;
      double delta = high - low;
  loop = 0;
      while ((abs(delta) > deltamax) && (loop <= _MAXITER))
      {
          mid = (low + high) / 2.0;
          midgma = IncompleteBetaFunction(a, b, mid);
          if (midgma > x) high = mid;
          else if (midgma < x) low = mid;
          delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
      }
      return mid;
}
// Incomplete Beta Function based distributions
double StatisticsBase :: BetaDist(double a, double b, double x) { if (x >= 1) return 1.0; else return IncompleteBetaFunction(a, b, x) / BetaF(a, b); }
double StatisticsBase :: InvBetaDist(double a, double b, double x) { return InvIncompleteBetaFunction(a, b, x * BetaF(a, b)); }
double StatisticsBase :: FDist(double a, double b, double x) { return IncompleteBetaFunction(a/2.0, b/2.0, (a*x)/(b + a*x)) / BetaF(a/2.0, b/2.0); }
double StatisticsBase :: InvFDist(double a, double b, double x) { double y = InvIncompleteBetaFunction(a/2.0, b/2.0, x * BetaF(a/2.0, b/2.0)); return (b*y)/(a*(1.0-y)); }
double StatisticsBase :: StudentsTDist(double a, double x) { return IncompleteBetaFunction(0.5, a/2.0, (x*x)/(a + x*x)) / BetaF(0.5, a/2.0); }
double StatisticsBase :: InvStudentsTDist(double a, double x) { double y = InvIncompleteBetaFunction(0.5, a/2.0, x * BetaF(0.5, a/2.0)); return sqrt((a*y)/(1.0-y)); }
// constructor
StatisticsBase :: StatisticsBase (int height) : Scale (height) {}
