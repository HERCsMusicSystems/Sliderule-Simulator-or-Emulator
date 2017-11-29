
var multiplicationTests = {};

multiplicationTests ['Simple multiplication test (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  message ("The task : multiply " + a + " by " + b + " using A/B scales.");
  sliderules . objective = function () {
    if (checkValue ("A", a * b) && checkValue ("B", b)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Simple multiplication test (A/B)');
      return true;
    }
    return false;
  };
  return null;
};

multiplicationTests ['Simple multiplication test (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a * b > 10) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  message ("The task: multiply " + a + " by " + b + " using C/D scales.");
  sliderules . objective = function () {
    if (checkValue ("D", a * b) && checkValue ("C", b)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Simple multiplication test (C/D)');
      return true;
    }
    return false;
  };
  return null;
};

multiplicationTests ['Wrap-around multiplication test (C/D)'] = function (message) {
  var a = 1, b = 1;
  while (a * b < 10 || a * b > 100) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  message ("The task: multiply " + a + " by " + b + " using C/D scales.");
  sliderules . objective = function () {
    if (checkValue ("D", a * b / 10) && checkValue ("C", b)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Wrap-around multiplication test (C/D)');
      return true;
    }
    return false;
  };
  return null;
};

slideruleLessons . push (multiplicationTests);

