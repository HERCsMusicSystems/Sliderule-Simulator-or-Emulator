
var la = sliderules . la;

var s = new Sliderule (1200); sliderules . sliderules . push (s);
s . braces . push (new BezierBraces (2, 0.1, 8, 'gold', 'black', 12, 0.08, 40, 140));

var r = new Rule (); s . rules . push (r);
r . scales . push (new spacer (2));
r . scales . push (new scale_Sdec_down (24, {left: 'S', ls: 0.025, la: la}));
r . scales . push (new scale_STdec (24, {left: 'ST', ls: 0.025, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_Tdec_down (24, {left: 'T', ls: 0.025, la: la}));
r . scales . push (new scale_A (24, {left: 'A', ls: 0.025, la: la}));

r = new Rule ({stator: 1}); s . rules . push (r);
r . scales . push (new scale_B (24, {left: 'B', ls: 0.025, la: la}));
r . scales . push (new scale_L (24, {left: 'L', ls: 0.025, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_J (24, {left: 'K', ls: 0.025, la: la}));
r . scales . push (new scale_C (24, {left: 'C', ls: 0.025, la: la}));

r = new Rule (); s . rules . push (r);
r . scales . push (new scale_D (24, {left: 'D', ls: 0.025, la: la}));
r . scales . push (new scale_CI (24, {left: 'DI', ls: 0.025, la: la}));
r . scales . push (new spacer (2));
r . scales . push (new scale_P_down (24, {left: 'Ps', ls: 0.025, la: la, marking_colour: 'red'}));
r . scales . push (new scale_PT (24, {left: 'Pt', ls: 0.025, la: la, marking_colour: 'red', right_extension: 0.01}));
r . scales . push (new spacer (2));

s = new Sliderule (1200); sliderules . sliderules . push (s);

r = new Rule (); s . rules . push (r);
r . scales . push (new spacer (2));
r . scales . push (new scale_LL01_down (24));
r . scales . push (new scale_LL02 (24));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL03_down (24));
r . scales . push (new scale_CF (24));

r = new Rule ({stator: 1}); s . rules . push (r);
r . scales . push (new scale_DF (24));
r . scales . push (new scale_CI (24));
r . scales . push (new spacer (2));
r . scales . push (new scale_ISTd_down (24, {alt: 'black', marking_alt: 'red'}));
r . scales . push (new scale_C (24));

r = new Rule (); s . rules . push (r);
r . scales . push (new scale_D (24));
r . scales . push (new scale_LL3 (24));
r . scales . push (new spacer (2));
r . scales . push (new scale_LL2_down (24));
r . scales . push (new scale_LL1 (24));
r . scales . push (new spacer (2));

