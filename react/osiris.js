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

var osiris = new Image (); osiris . src = 'osiris_4.jpg';
var sliderules = new Sliderules ({background: osiris, background_scaling: {x: 2, y: 2}, background_translation: {x: 0, y: -150}});
var oak = new Image (); oak . src = 'oak.png'; oak . onload = function () {sliderules . requireRedraw = true;};
var oak_pattern = createPattern (oak);

var s = new Sliderule (1000); sliderules . sliderules . push (s);

s . cursorBraces . push (new CursorWindow (16, 10, oak_pattern, 'blue'));

var r = new Rule ({rule_colour: oak_pattern}); s . rules . push (r);
r . scales . push (new spacer (24));
r . scales . push (new scale_A (24, {left: "A"}));
r = new Rule ({stator: 1, rule_colour: oak_pattern}); s . rules . push (r);
r . scales . push (new scale_B (24, {left: "B"}));
r . scales . push (new scale_C (24, {left: "C"}));
r = new Rule ({rule_colour: oak_pattern}); s . rules . push (r);
r . scales . push (new scale_D (24, {left: "D"}));
r . scales . push (new spacer (24));
