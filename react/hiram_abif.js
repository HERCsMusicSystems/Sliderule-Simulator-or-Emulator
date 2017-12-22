
var sliderules = new Sliderules ();

var s = new Sliderule (1200); sliderules . sliderules . push (s);

var r = new Rule (); s . rules . push (r);

r . scales . push (new scale_LL03 (24));
r . scales . push (new scale_LL02 (24));
r . scales . push (new scale_LL01 (24));
r . scales . push (new scale_LL00 (24));
r . scales . push (new scale_DF (24));

r = new Rule ({stator: 1}); s . rules . push (r);

r . scales . push (new scale_CF (24));
r . scales . push (new scale_DIF (24));
r . scales . push (new scale_CI (24));
r . scales . push (new scale_C (24));

r = new Rule (); s . rules . push (r);

r . scales . push (new scale_DLL0 (24));
r . scales . push (new scale_LL1 (24));
r . scales . push (new scale_LL2 (24));
r . scales . push (new scale_LL3 (24));
r . scales . push (new scale_L (24));