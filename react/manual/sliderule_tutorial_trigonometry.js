
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
  var sinec = crnu (sine * 10);
  return [
  {action: function () {ensureSide (["S", "D"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which sine is " + sine + ".");}, delay: 500},
  {action: function () {isolate (["S", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + sinec + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", sine * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the S scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("D", sinec)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a sine'); return true;}
    return false;
  }}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometryLessons ['Angle of a tangent'] = function (message) {
  var tangent = 0;
  while (tangent > 0.95 || tangent < 0.1) tangent = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . atan (tangent) * 180 / Math . PI, 5);
  var tangentc = crnu (tangent * 10);
  return [
  {action: function () {ensureSide (["T", "D"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which tangent is " + tangent + ".");}, delay: 500},
  {action: function () {isolate (["T", "D"]), changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + tangentc + " on the D scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", tangent * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the T scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("D", tangentc)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a tangent'); return true;}
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

trigonometryLessons ['Calculate a'] = function (message) {
  var a = 0, c = 0;
  while (a <= 1 || c <= 1 || a >= c || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    c = 1 + Math . floor (Math . random () * 9);
  }
  var alpha = crnu (Math . asin (a / c) * 180 / Math . PI, 5);
  return [
    {action: function () {ensureSide (['C', 'D', 'S']); isolate (['C', 'D', 'S']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: c = " + c + ", \u03b1 = " + alpha + ", find a.");}, delay: 500},
    {action: function () {message ("Move cursor to 10 on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ('D', 10);}, delay: 1500},
    {action: function () {message ("Align " + c + " on the the C scale with cursor's hairline.");}, delay: 2000},
    {action: function () {slideTo ('C', c);}, delay: 2000},
    {action: function () {message ("Move cursor to " + alpha + " on the S scale.");}, delay: 3000},
    {action: function () {cursorTo ('S', alpha);}, delay: 1500},
    {action: function () {message ("Read the value of a = " + a + " on the C scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('S', alpha) && checkValue ('C', a)) {
        message ("Mission accomplished!");
        increaseCookieResult ('Calculate a');
        return true;
      }
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

trigonometryLessons ['Calculate c and \u03b1'] = function (message) {
  var a = 0, b = 0, c = 0;
  while (a <= 1 || b <= 1 || a > b || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    b = 1 + Math . floor (Math . random () * 9);
    c = crnu (Math . sqrt (a * a + b * b), 5);
  }
  var alpha = crnu (Math . atan (a / b) * 180 / Math . PI, 5);
  return [
    {action: function () {ensureSide (["CI", "D", "S", "T"]);}, delay: 100},
    {action: function () {message ("The task: a = " + a + ", b = " + b + ", find c and \u03b1.");}, delay: 500},
    {action: function () {isolate (["S", "T", "CI", "D"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 500},
    {action: function () {message ("Move cursor to " + a + " on the D scale.");}, delay: 1000},
    {action: function () {cursorTo ("D", a);}, delay: 1500},
    {action: function () {message ("Align 10 on the CI scale with cursor's hairline.");}, delay: 2000},
    {action: function () {slideTo ("CI", 10);}, delay: 1500},
    {action: function () {message ("MoveCursor to " + b + " on the CI scale.");}, delay: 2000},
    {action: function () {cursorTo ("CI", b);}, delay: 1500},
    {action: function () {message ("Read the \u03b1 angle of " + alpha + " degrees from the T scale.");}, delay: 2000},
    {action: function () {message ("Since sine of " + alpha + " degrees is a fractional number....");}, delay: 2000},
    {action: function () {message (".... and " + a + " on the CI scale is already aligned with 10 on the D scale....");}, delay: 2000},
    {action: function () {message (".... " + a + " can be easily divided by sine of " + alpha + " degrees.");}, delay: 2000},
    {action: function () {message ("Simply move the cursor to " + alpha + " degrees on the S scale.");}, delay: 3000},
    {action: function () {cursorTo ("S", alpha);}, delay: 1500},
    {action: function () {message ("The value of c is " + c + " and can be read from the CI scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", 1); slideTo ("CI", 10); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("S", alpha) && checkValue ("CI", c)) {message ("Mission accomplished!"); increaseCookieResult ('Calculate c and \u03b1'); return true;}
      return false;
    }}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

slideruleLessons . push (trigonometryLessons);
