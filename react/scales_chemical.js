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
H: 1.00794,
He: 4.002602,
Li: 6.941,
Be: 9.012182,
B: 10.811,
C: 12.0107,
N: 14.0067,
NH: 15.01464,
CH3: 15.035,
O: 15.9994,
NH2: 16.02258,
OH: 17.01,
H2O: 18.01528,
NH4: 18.03846,
F: 18.9984032,
Ne: 20.1797,
Na: 22.98976928,
Mg: 24.305,
CN: 26.02,
Al: 26.9815386,
CO: 28.01,
Si: 28.0855,
Air: 28.97,
N8O2: 28.97,
C2H5: 30.07,
P: 30.973762,
S: 32.065,
Cl: 35.453,
K: 39.0983,
Ar: 39.948,
Ca: 40.078,
Sc: 44.955912,
NO2: 46.0055,
Ti: 47.867,
V: 50.9415,
Cr: 51.9961,
Mn: 54.938045,
Fe: 55.845,
Ni: 58.6934,
Co: 58.933195,
CO3: 60.008,
NO3: 62.0049,
Cu: 63.546,
Zn: 65.38,
Ga: 69.723,
Ge: 72.64,
As: 74.9216,
C6H5: 77.106,
Se: 78.96,
Br: 79.904,
Kr: 83.798,
Rb: 85.4678,
Sr: 87.62,
Y: 88.90585,
COOH: 45.0174,
Zr: 91.224,
Nb: 92.90638,
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
Hs: 277,
Ds: 281.16206,
Cn: 285,
Nh: 286,
Fl: 289,
Mc: 288,
Lv: 293,
Ts: 294,
Og: 294
};

var periodic_table = [
  {name: 'Hydrogen', element: 'H', mass: 1.00794, tolerance: 0.00001},
  {name: 'Helium', element: 'He', mass: 4.002602, tolerance: 0.000002},
  {name: 'Lithium', element: 'Li', mass: 6.941, tolerance: 0.002},
  {name: 'Berylium', element: 'Be', mass: 9.012182, tolerance: 0.000003},
  {name: 'Boron', element: 'B', mass: 10.811, tolerance: 0.007},
  {name: 'Carbon', element: 'C', mass: 12.0107, tolerance: 0.0008},
  {name: 'Nitrogen', element: 'N', mass: 14.0067, tolerance: 0.0001},
  {name: 'Oxygene', element: 'O', mass: 15.9994, tolerance: 0},
  {name: 'Fluorine', element: 'F', mass: 18.998403, tolerance: 0.0000005},
  {name: 'Neon', element: 'Ne', mass: 20.1797, tolerance: 0.0006},
  {name: 'Sodium', element: 'Na', mass: 22.989769, tolerance: 0.00000002},
  {name: 'Magnesium', element: 'Mg', mass: 24.305, tolerance: 0.0006},
  {name: 'Aluminium', element: 'Al', mass: 26.981539, tolerance: 0.0000008},
  {name: 'Silicon', element: 'Si', mass: 28.0855, tolerance: 0.0003},
  {name: 'Phosphorus', element: 'P', mass: 30.973752, tolerance: 0.000002},
  {name: 'Sulfur', element: 'S', mass: 32.065, tolerance: 0.005},
  {name: 'Chlorine', element: 'Cl', mass: 35.453, tolerance: 0.002},
  {name: 'Argon', element: 'Ar', mass: 39.948, tolerance: 0.001},
  {name: 'Potassium', element: 'K', mass: 39.0983, tolerance: 0.0001},
  {name: 'Calcium', element: 'Ca', mass: 40.078, tolerance: 0.004},
  {name: 'Scandium', element: 'Sc', mass: 44.955912, tolerance: 0.000006},
  {name: 'Titanium', element: 'Ti', mass: 47.867, tolerance: 0.001},
  {name: 'Vanadium', element: 'V', mass: 50.9415, tolerance: 0.0001},
  {name: 'Chromium', element: 'Cr', mass: 51.9961, tolerance: 0.0006},
  {name: 'Manganese', element: 'Mn', mass: 54.938044, tolerance: 0.0003},
  {name: 'Iron', element: 'Fe', mass: 55.845, tolerance: 0.002},
  {name: 'Cobalt', element: 'Co', mass: 58.933195, tolerance: 0.000005},
  {name: 'Nickel', element: 'Ni', mass: 58.6934, tolerance: 0.0002},
  {name: 'Copper', element: 'Cu', mass: 63.546, tolerance: 0.003},
  {name: 'Zinc', element: 'Zn', mass: 65.38, tolerance: 0.002},
  {name: 'Gallium', element: 'Ga', mass: 69.723, tolerance: 0.001},
  {name: 'Germanium', element: 'Ge', mass: 72.64, tolerance: 0.01},
  {name: 'Arsenic', element: 'As', mass: 74.9216, tolerance: 0.00002},
  {name: 'Selenium', element: 'Se', mass: 78.96, tolerance: 0.03},
  {name: 'Bromine', element: 'Br', mass: 79.904, tolerance: 0.001},
  {name: 'Krypton', element: 'Kr', mass: 83.798, tolerance: 0},
  {name: 'Rubidium', element: 'Rb', mass: 85.4678, tolerance: 0.0003},
  {name: 'Strontium', element: 'Sr', mass: 87.62, tolerance: 0.01},
  {name: 'Yttrium', element: 'Y', mass: 88.90585, tolerance: 0.00002},
  {name: 'Zirconium', element: 'Zr', mass: 91.224, tolerance: 0.002},
  {name: 'Niobium', element: 'Nb', mass: 92.90638, tolerance: 0.00002},
  {name: 'Molybdenum', element: 'Mo', mass: 95.94, tolerance: 0.02},
  {name: 'Technetium', element: 'Tc', mass: 98, tolerance: 0},
  {name: 'Ruthenium', element: 'Ru', mass: 101.07, tolerance: 0.02},
  {name: 'Rhodium', element: 'Rh', mass: 102.9055, tolerance: 0.00002},
  {name: 'Palladium', element: 'Pd', mass: 106.42, tolerance: 0.01},
  {name: 'Silver', element: 'Ag', mass: 107.8682, tolerance: 0},
  {name: 'Cadmium', element: 'Cd', mass: 112.411, tolerance: 0.008},
  {name: 'Indium', element: 'In', mass: 114.818, tolerance: 0.003},
  {name: 'Tin', element: 'Sn', mass: 118.71, tolerance: 0},
  {name: 'Antimony', element: 'Sb', mass: 121.76, tolerance: 0.001},
  {name: 'Tellurium', element: 'Te', mass: 127.6, tolerance: 0.03},
  {name: 'Iodine', element: 'I', mass: 126.90447, tolerance: 0},
  {name: 'Xeon', element: 'Xe', mass: 131.293, tolerance: 0.006},
  {name: 'Caesium', element: 'Cs', mass: 132.90545, tolerance: 0.0000002},
  {name: 'Barium', element: 'Ba', mass: 137.327, tolerance: 0.007},
  {name: 'Lanthanum', element: 'La', mass: 138.90547, tolerance: 0.00007},
  {name: 'Cerium', element: 'Ce', mass: 140.116, tolerance: 0.001},
  {name: 'Praseodymium', element: 'Pr', mass: 140.90765, tolerance: 0.00002},
  {name: 'Neodymium', element: 'Nd', mass: 144.242, tolerance: 0.003},
  {name: 'Promethium', element: 'Pm', mass: 145, tolerance: 0},
  {name: 'Samarium', element: 'Sm', mass: 150.36, tolerance: 0.02},
  {name: 'Europium', element: 'Eu', mass: 151.964, tolerance: 0.001},
  {name: 'Gadolinium', element: 'Gd', mass: 157.25, tolerance: 0.03},
  {name: 'Terbium', element: 'Tb', mass: 158.92535, tolerance: 0.00002},
  {name: 'Dysprosium', element: 'Dy', mass: 162.5, tolerance: 0.001},
  {name: 'Holmium', element: 'Ho', mass: 164.93032, tolerance: 0.00002},
  {name: 'Erbium', element: 'Er', mass: 167.259, tolerance: 0.003},
  {name: 'Thulium', element: 'Tm', mass: 168.93421, tolerance: 0.00002},
  {name: 'Ytterbium', element: 'Yb', mass: 173.04, tolerance: 0.03},
  {name: 'Lutetium', element: 'Lu', mass: 174.967, tolerance: 0.001},
  {name: 'Hafnium', element: 'Hf', mass: 178.49, tolerance: 0.02},
  {name: 'Tantalum', element: 'Ta', mass: 180.94788, tolerance: 0.00002},
  {name: 'Tungsten', element: 'W', mass: 183.84, tolerance: 0.01},
  {name: 'Rhenium', element: 'Re', mass: 186.207, tolerance: 0.001},
  {name: 'Osmium', element: 'Os', mass: 190.23, tolerance: 0.03},
  {name: 'Iridium', element: 'Ir', mass: 192.217, tolerance: 0.003},
  {name: 'Platinum', element: 'Pt', mass: 195.084, tolerance: 0},
  {name: 'Gold', element: 'Au', mass: 196.96657, tolerance: 0.000004},
  {name: 'Mercury', element: 'Hg', mass: 200.59, tolerance: 0.02},
  {name: 'Thallium', element: 'Tl', mass: 204.3833, tolerance: 0.0002},
  {name: 'Lead', element: 'Pb', mass: 207.2, tolerance: 0.1},
  {name: 'Bismuth', element: 'Bi', mass: 208.9804, tolerance: 0.00001},
  {name: 'Polonium', element: 'Po', mass: 209, tolerance: 0},
  {name: 'Astatine', element: 'At', mass: 210, tolerance: 0},
  {name: 'Radon', element: 'Rn', mass: 222, tolerance: 0},
  {name: 'Francium', element: 'Fr', mass: 223, tolerance: 0},
  {name: 'Radium', element: 'Ra', mass: 226, tolerance: 0},
  {name: 'Actinum', element: 'Ac', mass: 227, tolerance: 0},
  {name: 'Thorium', element: 'Th', mass: 232.03806, tolerance: 0.00002},
  {name: 'Protactinum', element: 'Pa', mass: 231.03588, tolerance: 0.00002},
  {name: 'Uranium', element: 'U', mass: 238.02891, tolerance:  0.00003},
  {name: 'Neptunium', element: 'Np', mass: 237, tolerance: 0},
  {name: 'Plutonium', element: 'Pu', mass: 244, tolerance: 0},
  {name: 'Americium', element: 'Am', mass: 243, tolerance: 0},
  {name: 'Curium', element: 'Cm', mass: 247, tolerance: 0},
  {name: 'Berkelium', element: 'Bk', mass: 247, tolerance: 0},
  {name: 'Californium', element: 'Cf', mass: 251, tolerance: 0},
  {name: 'Einsteinium', element: 'Es', mass: 252, tolerance: 0},
  {name: 'Fermium', element: 'Fm', mass: 257, tolerance: 0},
  {name: 'Mendelevium', element: 'Md', mass: 258, tolerance: 0},
  {name: 'Nobelium', element: 'No', mass: 259, tolerance: 0},
  {name: 'Lawrencium', element: 'Lr', mass: 262, tolerance: 0},
  {name: 'Rutherfordium', element: 'Rf', mass: 261, tolerance: 0},
  {name: 'Dubnium', element: 'Db', mass: 262, tolerance: 0},
  {name: 'Seaborgium', element: 'Sg', mass: 266, tolerance: 0},
  {name: 'Bohrium', element: 'Bh', mass: 264, tolerance: 0},
  {name: 'Hassium', element: 'Hs', mass: 277, tolerance: 0},
  {name: 'Meitnerium', element: 'Mt', mass: 268, tolerance: 0},
  {name: 'Darmstadtium', element: 'Ds', mass: 281, tolerance: 0},
  {name: 'Roentgenium', element: 'Rg', mass: 272, tolerance: 0},
  {name: 'Copernicium', element: 'Cn', mass: 285, tolerance: 0},
  {name: 'Nihonium', element: 'Nh', mass: 286, tolerance: 0},
  {name: 'Flerovium', element: 'Fl', mass: 289, tolerance: 0},
  {name: 'Moscovium', element: 'Mc', mass: 288, tolerance: 0},
  {name: 'Livermorium', element: 'Lv', mass: 292, tolerance: 0},
  {name: 'Tennessine', element: 'Ts', mass: 294, tolerance: 0},
  {name: 'Oganesson', element: 'Og', mass: 294, tolerance: 0}
];

var compound_table = [
  {name: 'Nitrogen monohydride', formulae: 'NH', elements: [{element: 'N', count: 1}, {element: 'H', count: 1}]},
  {name: 'Methyl', formulae: 'CH3', elements: [{element: 'C', count: 1}, {element: 'H', count: 3}]},
  {name: 'Amidogen', formulae: 'NH2', elements: [{element: 'N', count: 1}, {element: 'H', count: 2}]},
  {name: 'Hydroxide', formulae: 'OH', elements: [{element: 'O', count: 1}, {element: 'H', count: 1}]},
  {name: 'Water', formulae: 'H2O', elements: [{element: 'H', count: 2}, {element: 'O', count: 1}]},
  {name: 'Amonium', formulae: 'NH4', elements: [{element: 'N', count: 1}, {element: 'H', count: 4}]},
  {name: 'Cyanide', formulae: 'CN', elements: [{element: 'C', count: 1}, {element: 'N', count: 1}]},
  {name: 'Carbon monoxide', formulae: 'CO', elements: [{element: 'C', count: 1}, {element: 'O', count: 1}]},
  {name: 'Air', formulae: 'Air', elements: [{element: 'N', count: 8}, {element: 'O', count: 2}]},
  {name: 'Ethyl radical', formulae: 'C2H5', elements: [{element: 'C', count: 2}, {element: 'H', count: 5}]},
  {name: 'Nitrogen dioxide', formulae: 'NO2', elements: [{element: 'N', count: 1}, {element: 'O', count: 2}]},
  {name: 'Carbonate', formulae: 'CO3', elements: [{element: 'C', count: 1}, {element: 'O', count: 3}]},
  {name: 'Nitrate', formulae: 'NO3', elements: [{element: 'N', count: 1}, {element: 'O', count: 3}]},
  {name: 'Phenyl', formulae: 'C6H5', elements: [{element: 'C', count: 6}, {element: 'H', count: 5}]},
  {name: 'Carboxyl radical', formulae: 'COOH', elements: [{element: 'C', count: 1}, {element: 'O', count: 1}, {element: 'O', count: 1}, {element: 'H', count: 1}]},
  {name: 'Phosphate', formulae: 'PO4', elements: [{element: 'P', count: 1}, {element: 'O', count: 4}]},
  {name: 'Sulphate', formulae: 'SO4', elements: [{element: 'S', count: 1}, {element: 'O', count: 4}]},
  {name: 'Pyrophosphate', formulae: 'P2O7', elements: [{element: 'P', count: 2}, {element: 'O', count: 7}]}
];

var formulae = function (compound) {
  var elements = compound . elements;
  var f = "";
  for (var ind in elements) {
    f += elements [ind] . element;
    switch (elements [ind] . count) {
      case 2: f += '\u2082'; break;
      case 3: f += '\u2083'; break;
      case 4: f += '\u2084'; break;
      case 5: f += '\u2085'; break;
      case 6: f += '\u2086'; break;
      case 7: f += '\u2087'; break;
      case 8: f += '\u2088'; break;
      case 9: f += '\u2089'; break;
      default: break;
    }
  }
  return f;
};

var masses = [];
for (var ind in element_mass) {
  var id = ind . toLowerCase ();
  if (! masses [id]) masses [id] = element_mass [ind];
}
for (var ind in periodic_table) {masses [periodic_table [ind] . name . toLowerCase ()] = element_mass [periodic_table [ind] . element];}
for (var ind in compound_table) {masses [compound_table [ind] . name . toLowerCase ()] = element_mass [compound_table [ind] . formulae];}

var getMolecularMass = function (el) {
  var mass = masses [el . toLowerCase ()];
  while (mass > 10) mass /= 10;
  while (mass < 1) mass *= 10;
  return mass;
};

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
    var mass = element_mass [position];
    if (mass == undefined) mass = masses [position . toLowerCase ()];
    if (mass != undefined) {
      while (mass > 100) mass /= 10;
      while (mass < 10) mass *= 10;
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
