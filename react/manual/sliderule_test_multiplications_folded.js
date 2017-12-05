
var foldedMultiplicationTests = {};

foldedMultiplicationTests ['Folded-scale multiplication test (CF/DF)'] = function (message) {
  var a = 0, b = 0;
  while (a > Math . PI || a < Math . PI / 10 || a == 1 || b > Math . PI || b < Math . PI / 10 || b == 1 || a * b > Math . PI || a * b < Math . PI / 10) {
    a = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
    b = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
  }
  message ("The task: multiply " + a + " by " + b + " using CF/DF scales.");
  sliderules . objective = function () {
    if (checkValue ("DF", a * b) && checkValue ("CF", b)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Folded-scale multiplication test (CF/DF)');
      return true;
    }
    return false;
  };
  return null;
};

foldedMultiplicationTests ['Folded-scale all numbers multiplication test (CF/DF)'] = function (message) {
  var a = 0, b = 0;
  while (a > Math . PI || a < Math . PI / 10 || a == 1 || b > Math . PI || b < Math . PI / 10 || b == 1 || a * b > Math . PI || a * b < Math . PI / 10) {
    a = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
    b = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
  }
  var am = 1, bm = 1;
  while (am == 1 && bm == 1) {am = rndlist ([-3, -2, -1, 0, 1, 2, 3]); bm = rndlist ([-3, -2, -1, 0, 1, 2, 3]);}
  message ("The task: multiply " + crnu (a * Math . pow (10, am)) + " by " + crnu (b * Math . pow (10, bm)) + " using CF/DF scales.");
  sliderules . objective = function () {
    if (checkValue ("DF", a * b) && checkValue ("CF", b)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Folded-scale all numbers multiplication test (CF/DF)');
      return true;
    }
    return false;
  };
  return null;
};

foldedMultiplicationTests ['Folded-scale multiplication test (C/D/CF/DF)'] = function (message) {
  var a = 1, b = 1;
  while (a * b < 10 || a * b > Math . PI * 10 || b < Math . PI) {
    a = crnu (Math . floor (11 + Math . random () * 100) / 10);
    b = crnu (Math . floor (11 + Math . random () * 100) / 10);
  }
  message ("The task: multiply " + a + " by " + b + " using C/D and CF/DF scales.");
  sliderules . objective = function () {
    if (checkValue ("DF", a * b / 10) && checkValue ("CF", b / 10)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Folded-scale multiplication test (C/D/CF/DF)');
      return true;
    }
    return false;
  };
  return null;
};

slideruleLessons . push (foldedMultiplicationTests);

