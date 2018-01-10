
var sliderules = new Sliderules ();

var tc = new Image (); tc . src = "templar_cross.png"; tc . onload = function () {sliderules . requireRedraw = true;};

//============================

var s = new Sliderule (1200, {markings_colour: 'gold', markings_background: 'black', cursor_markings_shift: 0.002}); sliderules . sliderules . push (s);

s . cursors . push (new Cursor (HairlineS, 100, 124, 'red', {marking_shift: 0.002}));
s . cursors . push (new Cursor (HairlineS, 172, 196, 'red', {marking_shift: 0.002}));
s . cursors . push (new Cursor (HairlineD, 246, 294, 'red', {marking_shift: 0.002}));
s . cursors . push (new Cursor (Hairline360, 124, 172, 'red', {marking_shift: 0.002}));

var r = new Rule (); s . rules . push (r);

r . scales . push (new spacer (4));
r . scales . push (new scale_Chemical (24,
    ['H', {e: 'Ag', align: 'right'}, {e: 'B', align: 'left'}, 'Cd', 'C', 'I', 'Cs', 'Ba', {e: 'CH3', v: 'CH\u2083'}, 'O', 'OH', {e: 'H2O', v: 'H\u2082O'}, 'F',
      {e: 'Au', shift: -3}, {e: 'Hg', shift: 3}, 'Bi',
      'Mg', 'CN', 'Al', 'CO', {e: 'C2H5', v: 'C\u2082H\u2085'}, 'Cl', {e: 'K', shift: -2},
      {e: 'He', shift: 2}, 'COOH', 'Cr', {e: 'Mn', shift: -4}, {e: 'Fe', shift: 4},
      {e: 'Co', shift: -4}, {e: 'CO3', v: 'CO\u2083', shift: 7}, 'Cu', 'Li',
      {e: 'As', align: 'right'}, {e: 'C6H5', v: 'C\u2086H\u2085'}, {e: 'Br', align: 'left'}, 'Kr',
      'Be', 'Mo'
      ],
    {left: 'A~M', lines: [{left: Math . log10 (masses . H), right: Math . log10 (masses . Mo) - 1, height: 1}]}));
//r . scales . push (new spacer (4));
r . scales . push (new scale_Chemical (24,
    [{e: 'Sn', shift: -2}, {e: 'Sb', shift: 2}, 'Xe', 'N', 'NH', {e: 'NH2', v: 'NH\u2082'},
    {e: 'P2O7', v: 'P\u2082O\u2087', align: 'right'}, {e: 'NH4', v: 'NH\u2084', shift: -4}, {e: 'W', shift: 4},
    'Pt', 'Ne', {e: 'Pb', shift: 3}, {e: 'Ra', shift: -4}, {e: 'Na', shift: 4}, 'U',
    'Si', 'Air', 'P', 'S', {e: 'Ar', align: 'right'}, {e: 'Ca', align: 'left'}, {e: 'NO2', v: 'NO\u2082'}, 'Ti', 'V',
    'Ni', {e: 'NO3', v: 'NO\u2083'}, 'Zn', 'Se', 'Rb', {e: 'Zr', shift: -4}, {e: 'PO4', v: 'PO\u2084', shift: -6}, {e: 'SO4', v: 'SO\u2084', align: 'left'}
    ],
    {left: 'N~Z', lines: [{left: Math . log10 (masses . Sn) - 2, right: Math . log10 (masses . SO4) - 1, height: 1}]}));
r . scales . push (new scale_P (24, {left: "P"}));
r . scales . push (new scale_K (24, {left: "K"}));
r . scales . push (new scale_A (24, {left: "A"}));
r . scales . push (new scale_CF (24, {left: "DF"}));

r = new Rule ({stator: 1}); s . rules . push (r);

r . markings . push (new Logo (tc, -0.15, 5 * 12 + 1, 0.25));
r . markings . push (new Logo (tc, 1.15, 5 * 12 + 1, 0.25));

r . scales . push (new scale_DF (24, {left: "CF"}));
r . scales . push (new scale_B (24, {left: "B"}));
r . scales . push (new scale_CIF (24, {left: "DIF", colour: 'red'}));
r . scales . push (new spacer (2));
r . scales . push (new scale_DI (24, {left: "CI", colour: 'red'}));
r . scales . push (new scale_C (24, {left: "C"}));

r = new Rule (); s . rules . push (r);

r . scales . push (new scale_D (24, {left: "D"}));
r . scales . push (new scale_DI (24, {left: "DI", colour: 'red'}));
r . scales . push (new scale_Sdec_down (24, {left: "S"}));
r . scales . push (new scale_TCTdec_down (24, {left: "T1"}));
r . scales . push (new scale_TCT1dec_down (24, {left: "T2"}));
r . scales . push (new scale_STdec_down (24, {left: "ST"}));

r . scales . push (new spacer (4));

//===========================

s = new Sliderule (1200, {markings_colour: 'gold', markings_background: 'black', cursor_markings_shift: 0.002}); sliderules . sliderules . push (s);

r = new Rule (); s . rules . push (r);

r . scales . push (new spacer (4));

r . scales . push (new scale_L (24, {left: "L", colour: 'red'}));
r . scales . push (new scale_LL03 (24, {left: "LL03", colour: 'red'}));
r . scales . push (new scale_LL02 (24, {left: "LL02", colour: 'red'}));
r . scales . push (new scale_LL01 (24, {left: "LL01", colour: 'red'}));
r . scales . push (new scale_LL00 (24, {left: "LL00", colour: 'red'}));
r . scales . push (new scale_R1 (24, {left: "W1"}));

r = new Rule ({stator: 1}); s . rules . push (r);

r . markings . push (new Logo (tc, -0.15, 5 * 12 + 1, 0.25));
r . markings . push (new Logo (tc, 1.15, 5 * 12 + 1, 0.25));

r . scales . push (new scale_W1 (24, {left: "W1"}));
r . scales . push (new scale_Sdec (24, {left: "S"}));
r . scales . push (new spacer (2));
r . scales . push (new scale_Tdec_down (24, {left: "T"}));
r . scales . push (new scale_C (24, {left: "C", draw_r: true, draw_q: true}));
r . scales . push (new scale_R2 (24, {left: "W2"}));

r = new Rule (); s . rules . push (r);

r . scales . push (new scale_W2 (24, {left: "W2"}));
r . scales . push (new scale_DLL0 (24, {left: "D", draw_r: true, draw_q: true}));
r . scales . push (new scale_LL1_down (24, {left: "LL1"}));
r . scales . push (new scale_LL2_down (24, {left: "LL2"}));
r . scales . push (new scale_LL3_down (24, {left: "LL3"}));
r . scales . push (new scale_LW12 (24, {left: "L"}));

r . scales . push (new spacer (4));

