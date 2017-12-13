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
  {name: 'Hydrogen', element: 'H'},
  {name: 'Helium', element: 'He'},
  {name: 'Lithium', element: 'Li'},
  {name: 'Berylium', element: 'Be'},
  {name: 'Boron', element: 'B'},
  {name: 'Carbon', element: 'C'},
  {name: 'Nitrogen', element: 'N'},
  {name: 'Oxygene', element: 'O'},
  {name: 'Fluorine', element: 'F'},
  {name: 'Neon', element: 'Ne'},
  {name: 'Sodium', element: 'Na'},
  {name: 'Magnesium', element: 'Mg'},
  {name: 'Aluminium', element: 'Al'},
  {name: 'Silicon', element: 'Si'},
  {name: 'Phosphorus', element: 'P'},
  {name: 'Sulfur', element: 'S'},
  {name: 'Chlorine', element: 'Cl'},
  {name: 'Argon', element: 'Ar'},
  {name: 'Potassium', element: 'K'},
  {name: 'Calcium', element: 'Ca'},
  {name: 'Scandium', element: 'Sc'},
  {name: 'Titanium', element: 'Ti'},
  {name: 'Vanadium', element: 'V'},
  {name: 'Chromium', element: 'Cr'},
  {name: 'Manganese', element: 'Mn'},
  {name: 'Iron', element: 'Fe'},
  {name: 'Cobalt', element: 'Co'},
  {name: 'Nickel', element: 'Ni'},
  {name: 'Copper', element: 'Cu'},
  {name: 'Zinc', element: 'Zn'},
  {name: 'Gallium', element: 'Ga'},
  {name: 'Germanium', element: 'Ge'},
  {name: 'Arsenic', element: 'As'},
  {name: 'Selenium', element: 'Se'},
  {name: 'Bromine', element: 'Br'},
  {name: 'Krypton', element: 'Kr'},
  {name: 'Rubidium', element: 'Rb'},
  {name: 'Strontium', element: 'Sr'},
  {name: 'Yttrium', element: 'Y'},
  {name: 'Zirconium', element: 'Zr'},
  {name: 'Niobium', element: 'Nb'},
  {name: 'Molybdenum', element: 'Mo'},
  {name: 'Technetium', element: 'Tc'},
  {name: 'Ruthenium', element: 'Ru'},
  {name: 'Rhodium', element: 'Rh'},
  {name: 'Palladium', element: 'Pd'},
  {name: 'Silver', element: 'Ag'},
  {name: 'Cadmium', element: 'Cd'},
  {name: 'Indium', element: 'In'},
  {name: 'Tin', element: 'Sn'},
  {name: 'Antimony', element: 'Sb'},
  {name: 'Tellurium', element: 'Te'},
  {name: 'Iodine', element: 'I'},
  {name: 'Xeon', element: 'Xe'},
  {name: 'Caesium', element: 'Cs'},
  {name: 'Barium', element: 'Ba'},
  {name: 'Lanthanum', element: 'La'},
  {name: 'Cerium', element: 'Ce'},
  {name: 'Praseodymium', element: 'Pr'},
  {name: 'Neodymium', element: 'Nd'},
  {name: 'Promethium', element: 'Pm'},
  {name: 'Samarium', element: 'Sm'},
  {name: 'Europium', element: 'Eu'},
  {name: 'Gadolinium', element: 'Gd'},
  {name: 'Terbium', element: 'Tb'},
  {name: 'Dysprosium', element: 'Dy'},
  {name: 'Holmium', element: 'Ho'},
  {name: 'Erbium', element: 'Er'},
  {name: 'Thulium', element: 'Tm'},
  {name: 'Ytterbium', element: 'Yb'},
  {name: 'Lutetium', element: 'Lu'},
  {name: 'Hafnium', element: 'Hf'},
  {name: 'Tantalum', element: 'Ta'},
  {name: 'Tungsten', element: 'W'},
  {name: 'Rhenium', element: 'Re'},
  {name: 'Osmium', element: 'Os'},
  {name: 'Iridium', element: 'Ir'},
  {name: 'Platinum', element: 'Pt'},
  {name: 'Gold', element: 'Au'},
  {name: 'Mercury', element: 'Hg'},
  {name: 'Thallium', element: 'Tl'},
  {name: 'Lead', element: 'Pb'},
  {name: 'Bismuth', element: 'Bi'},
  {name: 'Polonium', element: 'Po'},
  {name: 'Astatine', element: 'At'},
  {name: 'Radon', element: 'Rn'},
  {name: 'Francium', element: 'Fr'},
  {name: 'Radium', element: 'Ra'},
  {name: 'Actinum', element: 'Ac'},
  {name: 'Thorium', element: 'Th'},
  {name: 'Protactinum', element: 'Pa'},
  {name: 'Uranium', element: 'U'},
  {name: 'Neptunium', element: 'Np'},
  {name: 'Plutonium', element: 'Pu'},
  {name: 'Americum', element: 'Am'},
  {name: 'Curium', element: 'Cm'},
  {name: 'Berkelium', element: 'Bk'},
  {name: 'Californium', element: 'Cf'},
  {name: 'Einsteinium', element: 'Es'},
  {name: 'Fermium', element: 'Fm'},
  {name: 'Mendelevium', element: 'Md'},
  {name: 'Nobelium', element: 'No'},
  {name: 'Lawrencium', element: 'Lr'},
  {name: 'Rutherfordium', element: 'Rf'},
  {name: 'Dubnium', element: 'Db'},
  {name: 'Seaborgium', element: 'Sg'},
  {name: 'Bohrium', element: 'Bh'},
  {name: 'Hassium', element: 'Hs'},
  {name: 'Meitnerium', element: 'Mt'},
  {name: 'Darmstadtium', element: 'Ds'},
  {name: 'Roentgenium', element: 'Rg'},
  {name: 'Copernicium', element: 'Cn'},
  {name: 'Nihonium', element: 'Nh'},
  {name: 'Flerovium', element: 'Fl'},
  {name: 'Moscovium', element: 'Mc'},
  {name: 'Livermorium', element: 'Lv'},
  {name: 'Tennessine', element: 'Ts'},
  {name: 'Oganesson', element: 'Og'}
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
