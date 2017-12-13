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

var oak = new Image (); oak . src = 'oak.png'; oak . onload = function () {sliderules . requireRedraw = true;};
var clay1 = new Image (); clay1 . src = 'clay_01.jpg'; clay1 . onload = function () {sliderules . requireRedraw = true;};
var clay2 = new Image (); clay2 . src = 'clay_03.jpg'; clay2 . onload = function () {sliderules . requireRedraw = true;};

var sliderules = new Sliderules ();

var s = new Sliderule (1200, {cursor_markings_shift: 0.002}); sliderules . sliderules . push (s);

s . cursorBraces . push (new CursorWindow (14, 10, oak, 'blue'));

s . backBraces . push (new FlatFloor (0.21, 0.21, -18, -18, 10, 'tan', oak));

var r = new Rule ({rule_colour: oak}); s . rules . push (r);
r . markings . push (new Screw (-0.175, 50, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . markings . push (new Screw (1.175, 50, 6, Math . random () * Math . PI, '#b5a642', '#605321'));


r . scales . push (new spacer (12));
r . scales . push (new scale_Chemical (24,
    ['H', {e: 'Ag', align: 'right'}, {e: 'B', align: 'left'}, 'Cd', 'C', 'I', 'Cs', 'Ba', {e: 'CH3', v: 'CH\u2083'}, 'O', 'OH', {e: 'H2O', v: 'H\u2082O'}, 'F',
      {e: 'Au', shift: -3}, {e: 'Hg', shift: 3}, 'Bi',
      'Mg', 'CN', 'Al', 'CO', {e: 'C2H5', v: 'C\u2082H\u2085'}, 'Cl', {e: 'K', shift: -2},
      {e: 'He', shift: 2}, 'COOH', 'Cr', {e: 'Mn', shift: -4}, {e: 'Fe', shift: 4},
      {e: 'Co', shift: -4}, {e: 'CO3', v: 'CO\u2083', shift: 7}, 'Cu', 'Li',
      {e: 'As', align: 'right'}, {e: 'C6H5', v: 'C\u2086H\u2085'}, {e: 'Br', align: 'left'}, 'Kr',
      'Be', 'Mo'
      ],
    {left: 'A~M', ls: 0.01, la: 'right', lines: [{left: Math . log10 (masses . H), right: Math . log10 (masses . Mo) - 1, height: 1}]}));
r . scales . push (new spacer (4));
r . scales . push (new scale_Chemical (24,
    [{e: 'Sn', shift: -2}, {e: 'Sb', shift: 2}, 'Xe', 'N', 'NH', {e: 'NH2', v: 'NH\u2082'},
    {e: 'P2O7', v: 'P\u2082O\u2087', align: 'right'}, {e: 'NH4', v: 'NH\u2084', shift: -4}, {e: 'W', shift: 4},
    'Pt', 'Ne', {e: 'Pb', shift: 3}, {e: 'Ra', shift: -4}, {e: 'Na', shift: 4}, 'U',
    'Si', 'Air', 'P', 'S', {e: 'Ar', align: 'right'}, {e: 'Ca', align: 'left'}, {e: 'NO2', v: 'NO\u2082'}, 'Ti', 'V',
    'Ni', {e: 'NO3', v: 'NO\u2083'}, 'Zn', 'Se', 'Rb', {e: 'Zr', shift: -4}, {e: 'PO4', v: 'PO\u2084', shift: -6}, {e: 'SO4', v: 'SO\u2084', align: 'left'}
    ],
    {left: 'N~Z', ls: 0.01, la: 'right', lines: [{left: Math . log10 (masses . Sn) - 2, right: Math . log10 (masses . SO4) - 1, height: 1}]}));
r . scales . push (new spacer (12));



r . scales . push (new scale_A (24, {left: "A", ls: 0.05}));
r = new Rule ({stator: 1, rule_colour: oak}); s . rules . push (r);
r . scales . push (new scale_B (24, {left: "B", ls: 0.05}));
r . scales . push (new scale_CI (24, {left: "CI", ls: 0.05}));
r . scales . push (new scale_C (24, {left: "C", ls: 0.05}));
r = new Rule ({rule_colour: oak}); s . rules . push (r);
r . markings . push (new Screw (-0.175, 30, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . markings . push (new Screw (1.175, 30, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . scales . push (new scale_D (24, {left: "D", ls: 0.05}));
r . scales . push (new scale_DI (24, {left: "DI", ls: 0.05}));
r . scales . push (new spacer (12));

