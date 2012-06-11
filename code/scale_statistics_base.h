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

#include "scale_base.h"
/* GJM -- Statistical scales base */
class StatisticsBase : public Scale {
public:
  // Generic Hyper Geometric Function
  double GenericHypGeoFunc(int n1, double n1array[], int n2, double n2array[], double x);
  // Hyper Geometric Function 2,1
  double HypGeoFunc(double a, double b, double c, double x);
  // Incomplete Gamma and Inverse Incomplete Gamma functions for Norm, Chi Square, and Gamma distributions
  double IncompleteGammaFunction(double a, double x);
  double InvIncompleteGammaFunction(double a, double x);
  // Log Gamma and Inv Log Gamma functions
  double LogGammaF(double x);
  double InvLogGammaF(double x);
  double LnGammaF(double x);
  double InvLnGammaF(double x);
  double BetaF(double a, double b);
  // Incomplete Gamma Function based distributions
  double ChiSquareDist(double v, double x);
  double InvChiSquareDist(double v, double x);
  double ChiSquareDegOfFreedom(double x, double a);
  double StudentsTDegOfFreedom(double x, double a);
  double NormalDist(double x);
  double InvNormalDist(double x);
  double GammaDist(double a, double b, double x);
  double InvGammaDist(double a, double b, double x);
  // Incomplete Beta and Invert Incomplete Beta functions for StudentsT, F, and Beta distributions
  double IncompleteBetaFunction(double a, double b, double x);
  double InvIncompleteBetaFunction(double a, double b, double x);
  // Incomplete Beta Function based distributions
  double BetaDist(double a, double b, double x);
  double InvBetaDist(double a, double b, double x);
  double FDist(double a, double b, double x);
  double InvFDist(double a, double b, double x);
  double StudentsTDist(double a, double x);
  double InvStudentsTDist(double a, double x);
  // constructor
  StatisticsBase (int height);
};
