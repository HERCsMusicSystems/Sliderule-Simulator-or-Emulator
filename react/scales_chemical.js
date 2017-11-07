///////////////////////////////////////////////////////////////////////////////////
//                     Copyright (C) 2017 Dr Robert P. Wolf                      //
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

//////////////////////////////////////////////////////////////////////////////
// Chemical
//////////////////////////////////////////////////////////////////////////////

var element_mass = {
H: 1.0079,
He: 4.0026,
Li: 6.941,
Be: 9.0122,
B: 10.811,
C: 12.0107,
N: 14.0067,
NH: 15.01464,
CH3: 15.035,
O: 15.9994,
NH2: 16.02258,
OH: 17.01,
H2o: 18.01528,
NH4: 18.03846,
F: 18.9984,
Ne: 20.1797,
Na: 22.9897,
Mg: 24.305,
CN: 26.02,
Al: 26.9815,
CO: 28.01,
Si: 28.0855,
Air: 28.97,
C2H5: 30.07,
P: 30.9738,
S: 32.065,
Cl: 35.453,
K: 39.0983,
Ar: 39.948,
Ca: 40.078,
Sc: 44.9559,
NO2: 46.0055,
Ti: 47.867,
V: 50.9415,
Cr: 51.9961,
Mn: 54.938,
Fe: 55.845,
Ni: 58.6934,
Co: 58.9332,
CO3: 60.008,
NO3: 62.0049,
Cu: 63.546,
Zn: 65.39,
Ga: 69.723,
Ge: 72.64,
As: 74.9216,
C6H5: 77.106,
Se: 78.96,
Br: 79.904,
Kr: 83.8,
Rb: 85.4678,
Sr: 87.62,
Y: 88.9059,
COOH: 45.0174,
Zr: 91.224,
Nb: 92.9064,
PO4: 94.971361,
Mo: 95.94,
SO4: 96.06,
Tc: 98,
Ru: 101.07,
Rh: 102.9055,
Pd: 106.42,
Ag: 107.8682,
Cd: 112.411,
In: 114.818,
Sn: 118.71,
Sb: 121.76,
I: 126.9045,
Te: 127.6,
Xe: 131.293,
Cs: 132.9055,
Ba: 137.327,
La: 138.9055,
Ce: 140.116,
Pr: 140.9077,
Nd: 144.24,
Pm: 145,
Sm: 150.36,
Eu: 151.964,
Gd: 157.25,
Tb: 158.9253,
Dy: 162.5,
Ho: 164.9303,
Er: 167.259,
Tm: 168.9342,
Yb: 173.04,
P2O7: 173.9433,
Lu: 174.967,
Hf: 178.49,
Ta: 180.9479,
W: 183.84,
Re: 186.207,
Os: 190.23,
Ir: 192.217,
Pt: 195.078,
Au: 196.9665,
Hg: 200.59,
Tl: 204.3833,
Pb: 207.2,
Bi: 208.9804,
Po: 209,
At: 210,
Rn: 222,
Fr: 223,
Ra: 226,
Ac: 227,
Pa: 231.0359,
Th: 232.0381,
Np: 237,
U: 238.0289,
Am: 243,
Pu: 244,
Cm: 247,
Bk: 247,
Cf: 251,
Es: 252,
Fm: 257,
Md: 258,
No: 259,
Rf: 261,
Lr: 262,
Db: 262,
Bh: 264,
Sg: 266,
Mt: 268,
Rg: 272,
Hs: 277
};

var masses = [];
for (var ind in element_mass) masses [ind . toLowerCase ()] = element_mass [ind];

var scale_Chemical = function (height, elements, options) {
  var s = new spacer (height, options);
  s . elements = [];
  for (var ind in elements) {
    var e = elements [ind];
    if (typeof (e) == 'string') e = {e: e, mass: element_mass [e], align: 'center'};
    else e . mass = element_mass [e . e];
    if (! e . v) e . v = e . e;
    if (! e . align) e . align = 'center';
    if (! e . shift) e . shift = 0;
    if (e . mass < 10) e . decoration = 'underline';
    else if (e . mass < 100) e . mass /= 10;
    else {e . mass /= 100; e . decoration = 'overline';}
    s . elements . push (e);
  }
  s . value = function (location) {return Math . pow (10, location + 1);};
  s . location = function (value) {return Math . log10 (value) - 1;};
  s . read = function (position) {
    var mass = masses [position . toLowerCase ()];
    if (mass != undefined) {
      mass = Math . log10 (mass);
      while (mass > 1) mass -= 1;
      return mass;
    }
    return null;
  };
  s . draw = function (ctx, length) {
    var h5 = s . height * 0.5;
    ctx . translate (0, s . height);
    for (var ind in s . elements) {
      var e = s . elements [ind];
      var location = Math . log10 (e . mass);
      while (location > 1) location -= 1;
      location *= length;
      ctx . textAlign = e . align;
      mmark (ctx, e . v, location + e . shift, h5);
      tick (ctx, location, h5);
      if (e . decoration == 'underline') mmark (ctx, '_' . repeat (e . v . length), location + e . shift, h5);
      if (e . decoration == 'overline') mmark (ctx, '\u00af' . repeat (e . v . length), location + e . shift, h5 + 1);
    }
  };
  return s;
};
