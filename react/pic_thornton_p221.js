
var la = sliderules . la;

var s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22}); sliderules . sliderules . push (s);
s . braces . push (new BraceSupport (0.09, 0, 298, 40, 8, 'tan', 'black'));
s . braces . push (new BraceSupport (1.35, 0, 298, 40, 8, 'tan', 'black'));
s . braces . push (new BezierBraces (0, 0.09, 8, 'gold', 'black', 12, 0.05, 50, 60));

var r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
r . markings . push (new Engraving ("BRITISH", '11px times', 'left', 'black', -0.1, 18));
r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 28));
r . scales . push (new spacer (2));
r . scales . push (new scale_Sdec_down (24, {left: 'S', ls: 0.03, la: la}));
r . scales . push (new scale_STdec (24, {left: 'ST', ls: 0.03, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_Tdec_down (24, {left: 'T', ls: 0.03, la: la}));
r . scales . push (new scale_A (24, {left: 'A', ls: 0.03, la: la}));

r = new Rule ({stator: 1, left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_B (24, {left: 'B', ls: 0.03, la: la}));
r . scales . push (new scale_L (24, {left: 'L', ls: 0.03, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_J (24, {left: 'K', ls: 0.03, la: la}));
r . scales . push (new scale_C (24, {left: 'C', ls: 0.03, la: la}));

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
r . scales . push (new scale_D (24, {left: 'D', ls: 0.03, la: la}));
r . scales . push (new scale_CI (24, {left: 'DI', ls: 0.03, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_P_down (24, {left: 'Ps', ls: 0.03, la: la, marking_colour: 'red'}));
r . scales . push (new scale_PT (24, {left: 'Pt', ls: 0.03, la: la, marking_colour: 'red', right_extension: 0.01}));
r . scales . push (new spacer (2));

s = new Sliderule (1200, {left_margin: 0.22, right_margin: 0.22}); sliderules . sliderules . push (s);
s . braces . push (new BraceSupport (0.09, 0, 298, 40, 8, 'tan', 'black'));
s . braces . push (new BraceSupport (1.35, 0, 298, 40, 8, 'tan', 'black'));
s . braces . push (new BezierBraces (0, 0.09, 8, 'gold', 'black', 12, 0.05, 50, 60));

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
r . markings . push (new Engraving ("BRITISH", '11px times', 'left', 'black', -0.1, 18));
r . markings . push (new Engraving ("THORNTON", '11px times', 'left', 'black', -0.1, 28));
r . markings . push (new Engraving ("AA 010", '11px arial', 'left', 'black', -0.1, 86));
r . markings . push (new Engraving ("COMPREHENSIVE", '8px arial', 'left', 'black', -0.1, 96));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL01_down (24, {left: 'LL\u2080\u2081', ls: 0.03, la: la, marking_colour: 'red'}));
r . scales . push (new scale_LL02 (24, {left: 'LL\u2080\u2082', ls: 0.03, la: la, marking_colour: 'red'}));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL03_down (24, {left: 'LL\u2080\u2083', ls: 0.03, la: la, marking_colour: 'red'}));
r . scales . push (new scale_CF (24, {left: 'DF', ls: 0.03, la: la}));

r = new Rule ({stator: 1, left_margin: 0.22, right_margin: 0.22}); s . rules . push (r);
r . scales . push (new scale_DF (24, {left: 'CI', ls: 0.03, la: la}));
r . scales . push (new scale_CI (24));
r . scales . push (new spacer (2));
r . scales . push (new scale_ISTd_down (24, {alt: 'black', marking_alt: 'red'}));
r . scales . push (new scale_C (24));

r = new Rule ({left_margin: 0.13, right_margin: 0.13}); s . rules . push (r);
r . markings . push (new Engraving ("MADE IN ENGLAND", '8px arial', 'left', 'black', -0.1, 96));
r . markings . push (new Engraving ("\u24d2", '18px arial', 'left', 'black', 1.06, 92));
r . scales . push (new scale_D (24));
r . scales . push (new scale_LL3 (24));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL2_down (24));
r . scales . push (new scale_LL1 (24));
r . scales . push (new spacer (2));

