
var multiplicationTests = {};

multiplicationTests ['Simple multiplication test (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  message ("The task : multiply " + a + " by " + b + " using A/B scales.");
  sliderules . objective = function () {
    if (checkValue ("A", a * b) && checkValue ("B", b)) {
      message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + (a * b));
      increaseCookieResult ('Simple multiplication test (A/B)');
      return true;
    }
    return false;
  };
  return null;
};

multiplicationTests ['Simple division test (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var ab = a * b;
  message ("The task : divide " + ab + " by " + b + " using A/B scales.");
  sliderules . objective = function () {
    if (checkValue ("A", a) && checkValue ("B", 1)) {
      message ("Mission accomplished! " + ab + " / " + b + " = " + a);
      increaseCookieResult ('Simple division test (A/B)');
      return true;
    }
  return false;};
};

multiplicationTests ['Simple multiplication test (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a > 10 || b > 10 || a * b > 10) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  message ("The task: multiply " + a + " by " + b + " using C/D scales.");
  sliderules . objective = function () {
    if (checkValue ("D", a * b) && checkValue ("C", b)) {
      message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + crnu (a * b));
      increaseCookieResult ('Simple multiplication test (C/D)');
      return true;
    }
    return false;
  };
  return null;
};

multiplicationTests ['Simple division test (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a > 10 || b > 10 || a * b > 10) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  var ab = crnu (a * b);
  message ("The task: divide " + ab + " by " + b + " using C/D scales.");
  sliderules . objective = function () {
    if (checkValue ("D", a) && checkValue ("C", 1)) {
      message ("Mission accomplished! " + ab + " / " + b + " = " + a);
      increaseCookieResult ('Simple division test (C/D)');
      return true;
    }
    return false;
  };
};

multiplicationTests ['Wrap-around multiplication test (C/D)'] = function (message) {
  var a = 1, b = 1;
  while (a > 10 || b > 10 || a * b < 10 || a * b > 100) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  message ("The task: multiply " + a + " by " + b + " using C/D scales.");
  sliderules . objective = function () {
    if (checkValue ("D", a * b / 10) && checkValue ("C", b)) {
      message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + (a * b));
      increaseCookieResult ('Wrap-around multiplication test (C/D)');
      return true;
    }
    return false;
  };
  return null;
};

slideruleLessons . push (multiplicationTests);

