
var trigonometryTests = {};

trigonometryTests ['Sine of an angle test'] = function (message) {
  var angle = 1;
  while (angle > 80 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var sine = crnu (Math . sin (angle * Math . PI / 180), 6);
  message ("The task: find sine of " + angle + " degrees.");
  sliderules . objective = function () {
    if (checkValue ("S", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Sine of an angle test'); return true;}
    return false;
  };
};

trigonometryTests ['Tangent of an angle test'] = function (message) {
  var angle = 1;
  while (angle > 45 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 6);
  message ("The task: find tangent of " + angle + " degrees.");
  sliderules . objective = function () {
    if (checkValue ("T", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Tangent of an angle test'); return true;}
    return false;
  };
};

trigonometryTests ['Angle of a sine test'] = function (message) {
  var sine = 0;
  while (sine > 0.95 || sine < 0.1) sine = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . asin (sine) * 180 / Math . PI, 5);
  message ("The task: find an angle, which sine is " + sine + ".");
  sliderules . objective = function () {
    if (checkValue ("C", sine * 10)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a sine test'); return true;}
    return false;
  };
};

trigonometryTests ['Angle of a tangent test'] = function (message) {
  var tangent = 0;
  while (tangent > 0.95 || tangent < 0.1) tangent = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . atan (tangent) * 180 / Math . PI, 5);
  message ("The task: find an angle, which tangent is " + tangent + ".");
  sliderules . objective = function () {
    if (checkValue ("C", tangent * 10)) {message ("Mission accomplished!"); increaseCookieResult ('Angle of a tangent test'); return true;}
    return false;
  };
};

trigonometryTests ['Small angle to radians conversion test'] = function (message) {
  var angle = 0;
  while (angle > 5.7 || angle < 0.58) angle = crnu (Math . floor (Math . random () * 100) / 10);
  var radians = crnu (angle * Math . PI / 180, 7);
  message ("The task: convert " + angle + " degrees to radians.");
  sliderules . objective = function () {
    if (checkValue ("ST", angle)) {message ("Mission accomplished!"); increaseCookieResult ('Small angle to radians conversion test'); return true;}
    return false;
  };
};

trigonometryTests ['Calculate a test'] = function (message) {
  var a = 0, c = 0;
  while (a <= 1 || c <= 1 || a >= c || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    c = 1 + Math . floor (Math . random () * 9);
  }
  var alpha = crnu (Math . asin (a / c) * 180 / Math . PI, 5);
  message ("The task: c = " + c + ", \u03b1 = " + alpha + ", find a.");
  sliderules . objective = function () {
    if (checkValue ('S', alpha) && checkValue ('C', a)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Calculate a test');
      return true;
    }
    return false;
  };
};

trigonometryTests ['Calculate c and \u03b1 test'] = function (message) {
  var a = 0, b = 0, c = 0;
  while (a <= 1 || b <= 1 || a > b || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    b = 1 + Math . floor (Math . random () * 9);
    c = crnu (Math . sqrt (a * a + b * b), 5);
  }
  var alpha = crnu (Math . atan (a / b) * 180 / Math . PI, 5);
  message ("The task: a = " + a + ", b = " + b + ", find c and \u03b1.");
  sliderules . objective = function () {
    if (checkValue ("S", alpha) && checkValue ("CI", c)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Calculate c and \u03b1 test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (trigonometryTests);
