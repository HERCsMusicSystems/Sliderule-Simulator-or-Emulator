
var trigonometrySlideLessons = {};

trigonometrySlideLessons ['Sine of an angle using slide'] = function (message) {
  var angle = 1;
  while (angle > 80 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var sine = crnu (Math . sin (angle * Math . PI / 180), 6);
  return [
  {action: function () {ensureSide (["S", "C"]);}, delay: 100},
  {action: function () {message ("The task: find sine of " + angle + " degrees.");}, delay: 500},
  {action: function () {isolate (["S", "C"]), changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + angle + " on the S scale.");}, delay: 3000},
  {action: function () {cursorTo ("S", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (sine * 10) + " on the C scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain value of " + crnu (sine, 5) + ".");}, delay: 2000},
  {action: function () {cursorTo ("C", 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("S", angle)) {
      message ("Mission accomplished! Sine of " + angle + " = " + crnu (sine, 5) + ".");
      increaseCookieResult ('Sine of an angle using slide');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometrySlideLessons ['Tangent of an angle using slide'] = function (message) {
  var angle = 1;
  while (angle > 45 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 6);
  return [
  {action: function () {ensureSide (["T", "C"]);}, delay: 100},
  {action: function () {message ("The task: find tangent of " + angle + " degrees.");}, delay: 500},
  {action: function () {isolate (["T", "C"]), changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + angle + " on the T scale.");}, delay: 3000},
  {action: function () {cursorTo ("T", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (tangent * 10) + " on the C scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain value of " + crnu (tangent, 5) + ".");}, delay: 2000},
  {action: function () {cursorTo ("D", 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("T", angle)) {
      message ("Mission accomplished! Tangent of " + angle + " degrees = " + crnu (tangent, 5) + ".");
      increaseCookieResult ('Tangent of an angle using slide');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometrySlideLessons ['Angle of a sine using slide'] = function (message) {
  var sine = 0;
  while (sine > 0.95 || sine < 0.1) sine = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . asin (sine) * 180 / Math . PI, 5);
  var sinec = crnu (sine * 10);
  return [
  {action: function () {ensureSide (["S", "C"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which sine is " + sine + ".");}, delay: 500},
  {action: function () {isolate (["S", "C"]), changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + sinec + " on the C scale.");}, delay: 3000},
  {action: function () {cursorTo ("C", sine * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the S scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("C", sinec)) {
      message ("Mission accomplished! " + sine + " is the sine of " + angle + " degrees.");
      increaseCookieResult ('Angle of a sine using slide');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometrySlideLessons ['Angle of a tangent using slide'] = function (message) {
  var tangent = 0;
  while (tangent > 0.95 || tangent < 0.1) tangent = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . atan (tangent) * 180 / Math . PI, 5);
  var tangentc = crnu (tangent * 10);
  return [
  {action: function () {ensureSide (["T", "C"]);}, delay: 100},
  {action: function () {message ("The task: find an angle, which tangent is " + tangent + ".");}, delay: 500},
  {action: function () {isolate (["T", "C"]), changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
  {action: function () {message ("Move the cursor to " + tangentc + " on the C scale.");}, delay: 3000},
  {action: function () {cursorTo ("C", tangent * 10);}, delay: 3000},
  {action: function () {message ("Read the value of " + angle + " on the T scale.");}, delay: 3000},
  {action: function () {cursorTo ("D", 1); dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ("C", tangentc)) {
      message ("Mission accomplished! " + tangent + " is the tangent of " + angle + " degrees.");
      increaseCookieResult ('Angle of a tangent using slide');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometrySlideLessons ['Small angle to radians conversion using slide'] = function (message) {
  var angle = 0;
  while (angle > 5.7 || angle < 1) angle = crnu (Math . floor (Math . random () * 100) / 10);
  var radians = crnu (angle * Math . PI / 180, 7);
  var sine = crnu (Math . sin (angle * Math . PI / 180), 5);
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 5);
  return [
  {action: function () {ensureSide (["C", "D"]);}, delay: 100},
  {action: function () {message ("The task: convert " + angle + " degrees to radians.");}, delay: 500},
  {action: function () {isolate (["C", "D"]), changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
  {action: function () {message ("Align the C scale with the D scale, so that the R gauge mark is above 10 and 1 is directly above the q gauge mark.");}, delay: 1000},
  {action: function () {message ('Righ-click on the cursor and enter "10" ....');}, delay: 1000},
  {action: function () {cursorTo ('D', 10);}, delay: 1000},
  {action: function () {message ('.... then righ-click on the C scale and enter "r" ....');}, delay: 4000},
  {action: function () {slideTo ('C', 18 / Math . PI);}, delay: 1000},
  {action: function () {message ("Move the cursor to " + angle + " on the C scale.");}, delay: 3000},
  {action: function () {cursorTo ("C", angle);}, delay: 3000},
  {action: function () {message ("Read the value of " + crnu (radians * 100) + " on the D scale.");}, delay: 3000},
  {action: function () {message ("Adjust the decimal point to obtain the value of " + crnu (radians, 5) + ".");}, delay: 2000},
  {action: function () {message ("Observe that: sine (" + angle + ") = " + sine + " and tangent (" + angle + ") = " + tangent + ".");}, delay: 2000},
  {action: function () {message (sine + " < " + crnu (radians, 5) + " < " + tangent);}, delay: 1000},
  {action: function () {message ("Since the angle is very small, " + crnu (radians, 5) + " is a good approximation of both sine and tangent.");}, delay: 1000},
  {action: function () {cursorTo ('D', 1); slideTo ('C', 1), dimmm (40, 255, 8);
  sliderules . objective = function () {
    if (checkValue ('D', radians * 100) && checkValue ('C', angle)) {
      message ("Mission accomplished! " + angle + " degrees = " + radians + " radians.");
      increaseCookieResult ('Small angle to radians conversion using slide');
      return true;
    }
    return false;
  };}, delay: 6000},
  {action: function () {isolate (); message ("Try these instruction again.");}, delay: 4000}
  ];
};

trigonometrySlideLessons ['Calculate a using slide'] = function (message) {
  var a = 0, c = 0;
  while (a <= 1 || c <= 1 || a >= c || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    c = 1 + Math . floor (Math . random () * 9);
  }
  var alpha = crnu (Math . asin (a / c) * 180 / Math . PI, 5);
  return [
    {action: function () {ensureSide (['C', 'D', 'S']); isolate (['C', 'D', 'S']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: c = " + c + ", \u03b1 = " + alpha + ", find a.");}, delay: 500},
    {action: function () {message ("Move cursor to " + c + " on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ('D', c);}, delay: 1500},
    {action: function () {message ("Align 10 on the the C scale with cursor's hairline.");}, delay: 2000},
    {action: function () {slideTo ('C', 10);}, delay: 2000},
    {action: function () {message ("Move cursor to " + alpha + " on the S scale.");}, delay: 3000},
    {action: function () {cursorTo ('S', alpha);}, delay: 1500},
    {action: function () {message ("Read the value of a = " + a + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('S', alpha) && checkValue ('D', a)) {
        message ("Mission accomplished! a = " + a + ".");
        increaseCookieResult ('Calculate a using slide');
        return true;
      }
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

trigonometrySlideLessons ['Calculate c and \u03b1 using slide'] = function (message) {
  var a = 0, b = 0, c = 0;
  while (a <= 1 || b <= 1 || a > b || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    b = 1 + Math . floor (Math . random () * 9);
    c = crnu (Math . sqrt (a * a + b * b), 5);
  }
  var alpha = crnu (Math . atan (a / b) * 180 / Math . PI, 5);
  return [
    {action: function () {ensureSide (["DI", "C", "S", "T"]);}, delay: 100},
    {action: function () {message ("The task: a = " + a + ", b = " + b + ", find c and \u03b1.");}, delay: 500},
    {action: function () {isolate (["S", "T", "DI", "C"]); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 500},
    {action: function () {message ("Make sure the cursor is over 10 on the DI scale.");}, delay: 1000},
    {action: function () {cursorTo ("DI", 10);}, delay: 1500},
    {action: function () {message ("Align " + a + " on the C scale with cursor's hairline.");}, delay: 2000},
    {action: function () {slideTo ("C", a);}, delay: 1500},
    {action: function () {message ("MoveCursor to " + b + " on the DI scale.");}, delay: 2000},
    {action: function () {cursorTo ("DI", b);}, delay: 1500},
    {action: function () {message ("Read the \u03b1 angle of " + alpha + " degrees from the T scale.");}, delay: 2000},
    {action: function () {message ("Since sine of " + alpha + " degrees is a fractional number....");}, delay: 2000},
    {action: function () {message (".... and " + a + " on the CI scale is already aligned with 10 on the D scale....");}, delay: 2000},
    {action: function () {message (".... " + a + " can be easily divided by sine of " + alpha + " degrees.");}, delay: 2000},
    {action: function () {message ("Simply move the cursor to " + alpha + " degrees on the S scale.");}, delay: 3000},
    {action: function () {cursorTo ("S", alpha);}, delay: 1500},
    {action: function () {message ("The value of c is " + c + " and can be read from the DI scale.");}, delay: 2000},
    {action: function () {cursorTo ("DI", 10); slideTo ("C", 1); dimmm (40, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("S", alpha) && checkValue ("DI", c)) {
        message ("Mission accomplished! c = " + c + " and \u03b1 = " + alpha + ".");
        increaseCookieResult ('Calculate c and \u03b1 using slide');
        return true;
      }
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

slideruleLessons . push (trigonometrySlideLessons);
