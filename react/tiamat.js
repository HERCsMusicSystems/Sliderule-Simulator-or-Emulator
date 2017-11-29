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

var sliderules = new Sliderules ({background_colour: 'tan'});

var s = new Sliderule (1000); sliderules . sliderules . push (s);

s . cursorBraces . push (new CursorWindow (16, 10, oak, 'blue'));

s . backBraces . push (new FlatFloor (0.21, 0.21, -18, -18, 10, 'tan', clay1));

var r = new Rule ({rule_colour: clay2}); s . rules . push (r);
r . markings . push (new Screw (-0.175, 24, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . markings . push (new Screw (1.175, 24, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . scales . push (new spacer (24));
r . scales . push (new scale_CF (24, {left: "DF", ls: 0.05}));
r = new Rule ({stator: 1, rule_colour: clay2}); s . rules . push (r);
r . scales . push (new scale_DF (24, {left: "CF", ls: 0.05}));
r . scales . push (new scale_C (24, {left: "C", ls: 0.05}));
r = new Rule ({rule_colour: clay2}); s . rules . push (r);
r . markings . push (new Screw (-0.175, 24, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . markings . push (new Screw (1.175, 24, 6, Math . random () * Math . PI, '#b5a642', '#605321'));
r . scales . push (new scale_D (24, {left: "D", ls: 0.05}));
r . scales . push (new spacer (24));

