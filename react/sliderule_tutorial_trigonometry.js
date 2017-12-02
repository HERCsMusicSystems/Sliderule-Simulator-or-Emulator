
var trigonometryLessons = {};

trigonometryLessons ['Sine of an angle'] = function (message) {
  var angle = 1;
  while (angle > 80 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var sine = crnu (Math . sin (angle * Math . PI / 180), 6);
  return [
  {action: function () {ensureSide (["S", "D"]);}, delay: 100},
  {action: function () {message ("The task: find sine of " + angle + " degrees.");}, delay: 500},
  {action: function () {isolate (["S", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + angle + " on the S scale.");}, delay: 3000},
  {action: function () {cursorTo ("S", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (sine * 10) + " on the D scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain value of " + crnu (sine, 5) + ".");}, delay: 2000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("S", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Sine of an angle'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometryLessons ['Tangent of an angle'] = function (message) {
  var angle = 1;
  while (angle > 45 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 6);
  return [
  {action: function () {ensureSide (["T", "D"]);}, delay: 100},
  {action: function () {message ("The task: find tangent of " + angle + " degrees.");}, delay: 500},
  {action: function () {isolate (["T", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + angle + " on the T scale.");}, delay: 3000},
  {action: function () {cursorTo ("T", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (tangent * 10) + " on the D scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain value of " + crnu (tangent, 5) + ".");}, delay: 2000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("T", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Tangent of an angle'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometryLessons ['Angle of a sine'] = function (message) {
  var sine = 0;
  while (sine > 0.95 || sine < 0.1) sine = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . asin (sine) * 180 / Math . PI, 5);
  return [
  {action: function () {ensureSide (["S", "D"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which sine is " + sine + ".");}, delay: 500},
  {action: function () {isolate (["S", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + sine * 10 + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", sine * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the S scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("D", sine * 10)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a sine'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometryLessons ['Angle of a tangent'] = function (message) {
  var tangent = 0;
  while (tangent > 0.95 || tangent < 0.1) tangent = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . atan (tangent) * 180 / Math . PI, 5);
  return [
  {action: function () {ensureSide (["T", "D"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which tangent is " + tangent + ".");}, delay: 500},
  {action: function () {isolate (["T", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + tangent * 10 + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", tangent * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the T scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("D", tangent * 10)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a tangent'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometryLessons ['Small angle to radians conversion'] = function (message) {
  var angle = 0;
  while (angle > 5.7 || angle < 0.58) angle = crnu (Math . floor (Math . random () * 100) / 10);
  var radians = crnu (angle * Math . PI / 180, 7);
  var sine = crnu (Math . sin (angle * Math . PI / 180), 5);
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 5);
  return [
  {action: function () {ensureSide (["S", "D"]);}, delay: 100},
  {action: function () {message ("The task: convert " + angle + " degrees to radians.");}, delay: 500},
  {action: function () {isolate (["ST", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + angle + " on the ST scale.");}, delay: 3000},
  {action: function () {cursorTo ("ST", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (radians * 100) + " on the D scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain value of " + crnu (radians, 5) + ".");}, delay: 2000},
  {action: function () {message ("Observe that: sine (" + angle + ") = " + sine + " and tangent (" + angle + ") = " + tangent + ".");}, delay: 2000},
  {action: function () {message (sine + " < " + crnu (radians, 5) + " < " + tangent);}, delay: 1000},
  {action: function () {message ("Since the angle is very small, " + crnu (radians, 5) + " is a good approximation of both sine and tangent.");}, delay: 1000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("ST", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Small angle to radians conversion'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

slideruleLessons . push (trigonometryLessons);
