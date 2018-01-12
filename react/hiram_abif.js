
var sliderules = new Sliderules ({manufacturer: '', name: ''});

var tc = new Image (); tc . src = "templar_cross.png"; tc . onload = function () {sliderules . requireRedraw = true;};

//============================

var s = new Sliderule (1200, {cursor_top_margin: 13, cursor_bottom_margin: 13, hairline_top: -10, hairline_bottom: -10, cursor_limit_left: 0.052, cursor_limit_right: 0.052, markings_background: 'gold', cursor_left_extension: 0.08, cursor_right_extension: 0.08, cursor_markings_shift: 0.005}); sliderules . sliderules . push (s);

s . braces . push (new LeftBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new LeftBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new RightBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new RightBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . cursorGlassBraces . push (new DecilonCursorBrace (1, 0.08, 12, 8, 0.04, 18, 4, 'tan', 'black'));
s . cursorBraces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
s . cursorBraces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
s . cursorBraces . push (new Screw (-0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (-0.06, 427, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (0.06, 427, 6, Math . random () * Math . PI, 'gold', 'silver'));

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
r . scales . push (new scale_P (24, {left: "P", right: "\u221a1-x\u00b2", rs: 0.07, left_extension: 0.05}));
r . scales . push (new scale_K (24, {left: "K", right: "X\u00b3", rs: 0.07, left_extension: 0.052, right_extension: 0.04}));
r . scales . push (new scale_A (24, {left: "A", right: "X\u00b2", rs: 0.07, left_extension: 0.05, right_extension: 0.042}));
r . scales . push (new scale_CF (24, {left: "DF", right: "\u03c0X", rs: 0.07, left_extension: 0.05, right_extension: 0.048, indices: ['>>1<<']}));

r = new Rule ({stator: 1}); s . rules . push (r);

r . markings . push (new Logo (tc, -0.15, 5 * 12 + 1, 0.25));
r . markings . push (new Logo (tc, 1.15, 5 * 12 + 1, 0.25));

r . scales . push (new scale_DF (24, {left: "CF", right: "\u03c0X", rs: 0.07, left_extension: 0.05, right_extension: 0.048, indices: ['>>1<<']}));
r . scales . push (new scale_B (24, {left: "B", right: "X\u00b2", rs: 0.07, left_extension: 0.05, right_extension: 0.042}));
r . scales . push (new scale_CIF (24, {left: "DIF", right: "10/\u03c0X", rs: 0.07, left_extension: 0.042, right_extension: 0.056, marking_colour: 'red', indices: ['>>1<<']}));
r . scales . push (new spacer (2));
r . scales . push (new scale_DI (24, {left: "CI", right: "10/X", rs: 0.07, left_extension: 0.042, right_extension: 0.048, marking_colour: 'red'}));
r . scales . push (new scale_C (24, {left: "C", right: "X", rs: 0.07, left_extension: 0.048, right_extension: 0.042}));

r = new Rule (); s . rules . push (r);

r . scales . push (new scale_D (24, {left: "D", right: "X", rs: 0.07, left_extension: 0.048, right_extension: 0.042}));
r . scales . push (new scale_DI (24, {left: "DI", right: "10/X", rs: 0.07, left_extension: 0.042, right_extension: 0.048, marking_colour: 'red'}));
r . scales . push (new scale_Sdec_down (24, {left: "S", right: "sin X", rs: 0.07, left_extension: 0.06}));
r . scales . push (new scale_TCTdec_down (24, {left: "T1", right: "tan X", rs: 0.07, left_extension: 0.06, right_extension: 0.061}));
r . scales . push (new scale_TCT1dec_down (24, {left: "T2", right: "tan X", rs: 0.07, left_extension: 0.061, right_extension: 0.06}));
r . scales . push (new scale_STdec_down (24, {left_extension: 0.061, right_extension: 0.058, left: "ST", right: "\u2222 arc 0.01X", rs: 0.07}));

r . scales . push (new spacer (4));

//===========================

s = new Sliderule (1200, {cursor_top_margin: 13, cursor_bottom_margin: 13, hairline_top: -10, hairline_bottom: -10, cursor_limit_left: 0.052, cursor_limit_right: 0.052, markings_background: 'gold', cursor_left_extension: 0.08, cursor_right_extension: 0.08, cursor_markings_shift: 0.005}); sliderules . sliderules . push (s);

s . braces . push (new LeftBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new LeftBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . braces . push (new RightBrace (2, 0.066, 8, 'gold', 'black', 74, 1));
s . braces . push (new RightBrace (4, 0.06, 8, 'tan', 'black', 76, 1));
s . cursorGlassBraces . push (new DecilonCursorBrace (1, 0.08, 12, 8, 0.04, 18, 4, 'tan', 'black'));
s . cursorBraces . push (new Engraving (sliderules . manufacturer, '12px arial', 'right', 'red', 0.07, 289));
s . cursorBraces . push (new Engraving (sliderules . name, '12px arial', 'left', 'red', -0.07, 12));
s . cursorBraces . push (new Screw (-0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (0.06, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (-0.06, 427, 6, Math . random () * Math . PI, 'gold', 'silver'));
s . cursorBraces . push (new Screw (0.06, 427, 6, Math . random () * Math . PI, 'gold', 'silver'));

r = new Rule (); s . rules . push (r);

r . scales . push (new spacer (4));

r . scales . push (new scale_L (24, {left: "L", right: "log X", rs: 0.07, left_extension: 0.05, right_extension: 0.05}));
r . scales . push (new scale_LL03 (24, {left: "LL03", marking_colour: 'red', right: "e^-x", left_extension: 0.04, rs: 0.02}));
r . scales . push (new scale_LL02 (24, {left: "LL02", marking_colour: 'red', right: "e^-0.1x", rs: 0.02, left_extension: 0.026, right_extension: 0.01}));
r . scales . push (new scale_LL01 (24, {left: "LL01", marking_colour: 'red', right: "e^-0.01x", rs: 0.02, left_extension: 0.046}));
r . scales . push (new scale_LL00 (24, {left: "LL00", marking_colour: 'red', right: "e^-0.001x", rs: 0.02, left_extension: 0.046, right_extension: 0.004}));
r . scales . push (new scale_R1 (24, {left_extension: 0.092, right_extension: 0.09, left: "W1", ls: 0.13, right: "\u221aX", rs: 0.1, draw_q: true}));

r = new Rule ({stator: 1}); s . rules . push (r);

r . markings . push (new Logo (tc, -0.15, 5 * 12 + 1, 0.25));
r . markings . push (new Logo (tc, 1.15, 5 * 12 + 1, 0.25));

r . scales . push (new scale_W1 (24, {left_extension: 0.092, right_extension: 0.09, left: "W1", ls: 0.13, right: "\u221aX", rs: 0.1, draw_q: true}));
r . scales . push (new scale_Sdec (24, {left: "S", right: "sin X", rs: 0.07, left_extension: 0.06}));
r . scales . push (new spacer (2));
r . scales . push (new scale_TCTdec_down (24, {left: "T", right: "tan X", rs: 0.07, left_extension: 0.06, right_extension: 0.046}));
r . scales . push (new scale_C (24, {left: "C", right: "X", rs: 0.07, left_extension: 0.048, right_extension: 0.042, draw_c: false, draw_r: true, draw_q: true}));
r . scales . push (new scale_R2 (24, {left_extension: 0.106, ls: 0.13, right_extension: 0.084, left: "W2", right: "\u221a10 X\u00b3", rs: 0.1, draw_q: true}));

r = new Rule (); s . rules . push (r);

r . scales . push (new scale_W2 (24, {left_extension: 0.106, ls: 0.13, right_extension: 0.084, left: "W2", right: "\u221a10 X\u00b3", rs: 0.1, draw_q: true}));
r . scales . push (new scale_DLL0 (24, {left: "D", right: "X", rs: 0.07, centre: "e^0.001x", cs: 1.02, ca: 'left', cc: 'red', left_extension: 0.048, draw_c: false, draw_r: true, draw_q: true}));
r . scales . push (new scale_LL1_down (24, {left: "LL1", right: "e^0.01x", rs: 0.02, left_extension: 0.05}));
r . scales . push (new scale_LL2_down (24, {left: "LL2", right: "e^0.1x", rs: 0.02, left_extension: 0.046, right_extension: 0.014}));
r . scales . push (new scale_LL3_down (24, {left: "LL3", right: "e^x", rs: 0.02, left_extension: 0.04, right_extension: 0.014}));
r . scales . push (new scale_LW12 (24, {left_extension: 0.061, right_extension: 0.06, left: "L", right: "\u00bdlg X", rs: 0.07}));

r . scales . push (new spacer (4));

