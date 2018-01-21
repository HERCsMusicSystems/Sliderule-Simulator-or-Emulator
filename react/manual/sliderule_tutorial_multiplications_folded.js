
var foldedMultiplicationLessons = {};

foldedMultiplicationLessons ['Folded-scale multiplication (CF/DF)'] = function (message) {
  var a = 0, b = 0;
  while (a > Math . PI || a < Math . PI / 10 || a == 1 || b > Math . PI || b < Math . PI / 10 || b == 1 || a * b > Math . PI || a * b < Math . PI / 10) {
    a = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
    b = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
  }
  return [
    {action: function () {ensureSide (["CF", "DF"]);}, delay: 100},
    {action: function () {message ("The task: multiply " + a + " by " + b + " using CF/DF scales.");}, delay: 1000},
    {action: function () {isolate (["CF", "DF"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the DF scale.");}, delay: 2000},
    {action: function () {cursorTo ("DF", a);}, delay: 2000},
    {action: function () {message ("Align the index (1) on CF with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("CF", 1);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + b + " on the CF scale.");}, delay: 4000},
    {action: function () {cursorTo ("CF", b);}, delay: 2000},
    {action: function () {message ("Read the value " + crnu (a * b) + " on the DF scale.");}, delay: 4000},
    {action: function () {cursorTo ("DF", 1); slideTo ("CF", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("DF", a * b) && checkValue ("CF", b)) {
        message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + crnu (a * b));
        increaseCookieResult ('Folded-scale multiplication (CF/DF)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

foldedMultiplicationLessons ['Folded-scale all numbers multiplication (CF/DF)'] = function (message) {
  var a = 0, b = 0;
  while (a > Math . PI || a < Math . PI / 10 || a == 1 || b > Math . PI || b < Math . PI / 10 || b == 1 || a * b > Math . PI || a * b < Math . PI / 10) {
    a = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
    b = crnu (0.32 + Math . floor (Math . random () * 100) / 10);
  }
  var am = 1, bm = 1;
  while (am == 1 && bm == 1) {am = rndlist ([-3, -2, -1, 0, 1, 2, 3]); bm = rndlist ([-3, -2, -1, 0, 1, 2, 3]);}
  var aam = crnu (a * Math . pow (10, am)), bbm = crnu (b * Math . pow (10, bm));
  return [
    {action: function () {ensureSide (["CF", "DF"]);}, delay: 100},
    {
      action: function () {
        message ("The task: multiply " + aam + " by " + bbm + " using CF/DF scales.");
      },
      delay: 1000
    },
    {action: function () {isolate (["CF", "DF"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the DF scale.");}, delay: 2000},
    {action: function () {cursorTo ("DF", a);}, delay: 2000},
    {action: function () {message ("Align the index (1) on CF with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("CF", 1);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + b + " on the CF scale.");}, delay: 4000},
    {action: function () {cursorTo ("CF", b);}, delay: 2000},
    {action: function () {message ("Read the value " + crnu (a * b) + " on the DF scale.");}, delay: 4000},
    {
      action: function () {message ("Adjust the decimal place and obtain the result of " + crnu (a * b * Math . pow (10, am + bm)) + ".");},
      delay: 2000
    },
    {action: function () {cursorTo ("DF", 1); slideTo ("CF", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("DF", a * b) && checkValue ("CF", b)) {
        message ("Mission accomplished! " + aam + " \u00d7 " + bbm + " = " + crnu (aam * bbm));
        increaseCookieResult ('Folded-scale all numbers multiplication (CF/DF)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

foldedMultiplicationLessons ['Folded-scale multiplication (C/D/CF/DF)'] = function (message) {
  var a = 1, b = 1;
  while (a * b < 10 || a * b > Math . PI * 10 || b < Math . PI) {
    a = crnu (Math . floor (11 + Math . random () * 100) / 10);
    b = crnu (Math . floor (11 + Math . random () * 100) / 10);
  }
  return [
    {action: function () {ensureSide (["C", "D", "CF", "DF"]);}, delay: 100},
    {action: function () {message ("The task: multiply " + a + " by " + b + " using C/D and CF/DF scales.");}, delay: 1000},
    {action: function () {isolate (["C", "D", "CF", "DF"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", a);}, delay: 2000},
    {action: function () {message ("Align the left index (1) on C with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("C", 1);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + crnu (b / 10) + " on the CF scale.");}, delay: 4000},
    {action: function () {cursorTo ("CF", b / 10);}, delay: 2000},
    {action: function () {message ("Read the value " + crnu (a * b / 10) + " on the DF scale.");}, delay: 4000},
    {action: function () {message ("Adjust the decimal place and obtain the result of " + crnu (a * b) + ".");}, delay: 2000},
    {action: function () {cursorTo ("D", 1); slideTo ("C", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("DF", a * b / 10) && checkValue ("CF", b / 10)) {
        message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + crnu (a * b));
        increaseCookieResult ('Folded-scale multiplication (C/D/CF/DF)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (foldedMultiplicationLessons);

