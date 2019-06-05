
var la = sliderules . la;
var model = sliderules . model;

var front_german_cursors = [], front_us_cursors = [];

var s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22, cursor_top_margin: model === 'pic' ? 0 : 16, cursor_bottom_margin: model === 'pic' ? 0 : 16, cursor_left_extension: 0.075, cursor_right_extension: 0.075, cursor_rounding: 0, cursorHairline: 'black', hairline_top: -2, hairline_bottom: -2, cursor_limit_left: 0.035, cursor_limit_right: 0.035}); sliderules . sliderules . push (s);
if (model === 'pic') {
	s . braces . push (new StaedtlerLeftBrace (4, 0.125, 0.18, 0.22, 0.22, 2, 65, 90, 'tan', 'red', 0, 8));
//	s . braces . push (new StaedtlerLeftBrace (4, 0.133, 0.16, 0.168, 0.188, 12, 78, 90, '#ddddff', 'red'));
	s . braces . push (new StaedtlerRightBrace (4, 0.125, 0.18, 0.22, 0.22, 2, 65, 90, 'tan', 'red', 0, 8));
//	s . braces . push (new StaedtlerRightBrace (4, 0.133, 0.16, 0.168, 0.188, 12, 78, 90, '#ddddff', 'red'));
} else {
	s . braces . push (new BraceSupport (0.09, 0, 298, 40, 8, 'tan', 'black'));
	s . braces . push (new BraceSupport (1.35, 0, 298, 40, 8, 'tan', 'black'));
	s . braces . push (new BezierBraces (0, 0.09, 8, 'gold', 'black', 12, 0.05, 50, 60));
}
switch (model) {
case 'pic':
    front_german_cursors . push (new Cursor (HairlineS, 76, 124, 'black'));
    front_german_cursors . push (new Cursor (HairlinePS, 76, 124, 'black'));
    front_us_cursors . push (new Cursor (HairlineS, 76, 124, 'black'));
    front_us_cursors . push (new Cursor (HairlineHPUS, 76, 124, 'black'));
case 'pic':
	s . cursorBraces . push (new CursorAngledBrace (0.076, 0.076, 18, 0, 30, 'gray'));
    s . cursorBraces . push (new Screw (-0.05, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.05, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (-0.05, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.05, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
	break;
    break;
case 'thornton':
    s . cursorGlassBraces . push (new CursorAngledBrace (0.076, 0.076, 18, -2, 0, 'gray'));
    s . cursorBraces . push (new Screw (-0.065, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.065, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (-0.065, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.065, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
case 'british thornton': s . cursors . push (new Cursor (HairlineS, 76, 124, 'black')); break;
};

var r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
switch (model) {
case 'british thornton':
    r . markings . push (new Engraving ("BRITISH", '11px times', 'left', 'black', -0.1, 18));
    r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 28));
    break;
case 'thornton': r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 18)); break;
case 'pic':
	r . markings . push (new Engraving ("PIC", '28px times', 'right', 'red', -0.073, 36));
	r . markings . push (new VLine (-0.07, 0, 100, 'black'));
	break;
}
r . scales . push (new spacer (2));
r . scales . push (new scale_Sdec_down (24, {left: 'S', ls: 0.03, la: la, right: 'sin', rs: 0.02}));
r . scales . push (new scale_STdec (24, {left: 'ST', ls: 0.03, la: la, right: 'sin tan', rs: 0.02, draw_st_corrections: false}));
r . scales . push (new spacer (2));
r . scales . push (new scale_Tdec_down (24, {left: 'T', ls: 0.03, la: la, right: 'tan', rs: 0.02}));
r . scales . push (new scale_A (24, {left: 'A', ls: 0.03, la: la, right: 'X\u00b2', rs: 0.02, draw_e: false}));

r = new Rule ({stator: 1, left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_B (24, {left: 'B', ls: 0.03, la: la, right: 'X\u00b2', rs: 0.02, draw_e: false}));
r . scales . push (new scale_L (24, {left: 'L', ls: 0.03, la: la, right: 'log x', rs: 0.02}));
r . scales . push (new spacer (2));
r . scales . push (new scale_J (24, {left: 'K', ls: 0.03, la: la, right: 'X\u00b3', rs: 0.02, draw_e: false}));
r . scales . push (new scale_C (24, {left: 'C', ls: 0.03, la: la, right: 'X', rs: 0.02, draw_e: false}));

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
r . scales . push (new scale_D (24, {left: 'D', ls: 0.03, la: la, right: 'X', rs: 0.02, draw_e: false}));
r . scales . push (new scale_CI (24, {left: 'DI', ls: 0.03, la: la, right: '1/x', rs: 0.02, marking_colour: 'red', draw_e: false}));
r . scales . push (new spacer (2));
r . scales . push (new scale_P_down (24, {left: 'Ps', ls: 0.03, la: la, marking_colour: 'red', right: '\u221a1-s\u00b2', rs: 0.02}));
r . scales . push (new scale_PT (24, {left: 'Pt', ls: 0.03, la: la, right_extension: 0.01, right: '\u221a1+t\u00b2', rs: 0.02}));
r . scales . push (new spacer (2));

s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22, cursor_top_margin: model === 'pic' ? 0 : 16, cursor_bottom_margin: model === 'pic' ? 0 : 16, cursor_left_extension: 0.075, cursor_right_extension: 0.075, cursor_rounding: 0, cursorHairline: 'black', hairline_top: -2, hairline_bottom: -2, cursor_limit_left: 0.035, cursor_limit_right: 0.035}); sliderules . sliderules . push (s);
if (model === 'pic') {
	s . braces . push (new StaedtlerLeftBrace (4, 0.125, 0.18, 0.22, 0.22, 2, 65, 90, 'tan', 'red', 0, 8));
//	s . braces . push (new StaedtlerLeftBrace (4, 0.133, 0.16, 0.168, 0.188, 12, 78, 90, '#ddddff', 'red'));
	s . braces . push (new StaedtlerRightBrace (4, 0.125, 0.18, 0.22, 0.22, 2, 65, 90, 'tan', 'red', 0, 8));
//	s . braces . push (new StaedtlerRightBrace (4, 0.133, 0.16, 0.168, 0.188, 12, 78, 90, '#ddddff', 'red'));
} else {
	s . braces . push (new BraceSupport (0.09, 0, 298, 40, 8, 'tan', 'black'));
	s . braces . push (new BraceSupport (1.35, 0, 298, 40, 8, 'tan', 'black'));
	s . braces . push (new BezierBraces (0, 0.09, 8, 'gold', 'black', 12, 0.05, 50, 60));
}

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
switch (model) {
case 'british thornton':
    r . markings . push (new Engraving ("BRITISH", '11px times', 'left', 'black', -0.1, 18));
    r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 28));
    r . markings . push (new Engraving ("AA 010", '11px arial', 'left', 'black', -0.1, 86));
    break;
case 'thornton':
    r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 18));
    r . markings . push (new Engraving ("No P 221", '11px arial', 'left', 'black', -0.1, 86));
    s . cursorGlassBraces . push (new CursorAngledBrace (0.076, 0.076, 18, -2, 0, 'gray'));
    s . cursorBraces . push (new Screw (-0.065, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.065, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (-0.065, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.065, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
    break;
case 'pic':
	r . markings . push (new Engraving ("\u24d2", '26px arial', 'left', 'black', 1.08, 30));
	s . cursorBraces . push (new CursorAngledBrace (0.076, 0.076, 18, 0, 30, 'gray'));
    s . cursorBraces . push (new Screw (-0.05, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.05, -9, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (-0.05, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
    s . cursorBraces . push (new Screw (0.05, 307, 6, Math . random () * Math . PI, 'gold', 'silver'));
	break;
}
if (model === 'pic') {
	r . markings . push (new Engraving ("PIC", '28px times', 'right', 'red', -0.073, 36));
    r . markings . push (new Engraving ("No. 221", '11px arial', 'right', 'red', -0.073, 86));
    r . markings . push (new Engraving ("England", '9px arial', 'right', 'red', -0.073, 96));
	r . markings . push (new VLine (-0.07, 0, 100, 'black'));
} else r . markings . push (new Engraving ("COMPREHENSIVE", '8px arial', 'left', 'black', -0.1, 96));
r . scales . push (new spacer (2));

r . scales . push (new scale_LL01_down (24, {left: 'LL\u2080\u2081', ls: 0.03, la: la, marking_colour: 'red', right: 'e^-0.01x', rs: 0.02}));
r . scales . push (new scale_LL02 (24, {left: 'LL\u2080\u2082', ls: 0.03, la: la, marking_colour: 'red', right: 'e^-0.1x', rs: 0.02}));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL03_down (24, {left: 'LL\u2080\u2083', ls: 0.03, la: la, marking_colour: 'red', right: 'e^x', rs: 0.02}));
r . scales . push (new scale_CF (24, {left: 'DF', ls: 0.03, la: la, right: '\u03c0X', rs: 0.02}));

r = new Rule ({stator: 1, left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_DF (24, {left: 'CF', ls: 0.03, la: la, right: '\u03c0X', rs: 0.02}));
r . scales . push (new scale_CI (24, {left: 'CI', ls: 0.03, la: la, marking_colour: 'red', right: '1/x', rs: 0.02}));
r . scales . push (new spacer (2));
r . scales . push (new scale_ISTd_down (24, {alt: 'black', marking_alt: 'red'}));
r . markings . push (new Engraving ('ISd', '12px arial', 'left', 'red', 0.02, 68));
r . markings . push (new Engraving ('ITd', '12px arial', 'left', 'black', 0.47, 68));
r . markings . push (new Engraving ('Td', '12px arial', 'left', 'red', 0.515, 68));
r . markings . push (new Engraving ('Sd', '12px arial', 'left', 'black', 0.97, 68));
r . scales . push (new scale_C (24, {left: 'C', ls: 0.03, la: la, right: 'X', rs: 0.02}));

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
if (model != 'pic') {
	r . markings . push (new Engraving ("MADE IN ENGLAND", '8px arial', 'left', 'black', -0.1, 96));
	r . markings . push (new Engraving ("\u24d2", '18px arial', 'left', 'black', 1.07, 90));
}
r . scales . push (new scale_D (24, {left: 'D', ls: 0.03, la: la, right: 'X', rs: 0.02}));
r . scales . push (new scale_LL3 (24, {left: 'LL\u2083', ls: 0.03, la: la, right: 'e^x', rs: 0.02}));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL2_down (24, {left: 'LL\u2082', ls: 0.03, la: la, right: 'e^0.1x', rs: 0.02}));
r . scales . push (new scale_LL1 (24, {left: 'LL\u2081', ls: 0.03, la: la, right: 'e^0.01x', rs: 0.02}));
r . scales . push (new spacer (2));

