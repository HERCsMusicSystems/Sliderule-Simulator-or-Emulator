
var chemicalTests = {};

var show_table = function () {
  for (var ind in periodic_table) {
    console . log (Number (ind) + 1, periodic_table [ind] . element, periodic_table [ind] . name, element_mass [periodic_table [ind] . element]);
  }
};

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
  {name: 'Air', formulae: 'Air', elements: [{element: 'N', count: 8}, {element: 'O', count: 4}]},
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

slideruleLessons . push (chemicalTests);
