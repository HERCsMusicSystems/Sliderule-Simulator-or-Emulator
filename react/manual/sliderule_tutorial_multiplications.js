
var multiplicationLessons = {};

multiplicationLessons ['Simple multiplication (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  return [
    {action: function () {ensureSide (["A", "B"]);}, delay: 100},
    {action: function () {message ("The task : multiply " + a + " by " + b + " using A/B scales.");}, delay: 1000},
    {action: function () {isolate (["A", "B"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the A scale.");}, delay: 2000},
    {action: function () {cursorTo ("A", a);}, delay: 2000},
    {action: function () {message ("Align the left index (1) on B with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("B", 1);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + b + " on the B scale.");}, delay: 4000},
    {action: function () {cursorTo ("B", b);}, delay: 1000},
    {action: function () {message ("Then read the result " + (a * b) + " on the A scale.");}, delay: 4000},
    {action: function () {cursorTo ("A", 1); slideTo ("B", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("A", a * b) && checkValue ("B", b)) {
        message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + (a * b));
        increaseCookieResult ('Simple multiplication (A/B)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

multiplicationLessons ['Simple division (A/B)'] = function (message) {
  var a = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var b = rndlist ([2, 3, 4, 5, 6, 7, 8, 9, 10]);
  var ab = a * b;
  return [
    {action: function () {ensureSide (["A", "B"]);}, delay: 100},
    {action: function () {message ("The task : divide " + ab + " by " + b + " using A/B scales.");}, delay: 1000},
    {action: function () {isolate (["A", "B"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + ab + " on the A scale.");}, delay: 2000},
    {action: function () {cursorTo ("A", ab);}, delay: 2000},
    {action: function () {message ("Align " + b + " on the B scale with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("B", b);}, delay: 2000},
    {action: function () {message ("Move the cursor to 1 on the B scale.");}, delay: 4000},
    {action: function () {cursorTo ("B", 1);}, delay: 1000},
    {action: function () {message ("Then read the result " + a + " on the A scale.");}, delay: 4000},
    {action: function () {cursorTo ("A", 1); slideTo ("B", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("A", a) && checkValue ("B", 1)) {
        message ("Mission accomplished! " + ab + " / " + b + " = " + a);
        increaseCookieResult ('Simple division (A/B)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

multiplicationLessons ['Simple multiplication (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a > 10 || b > 10 || a * b > 10) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  return [
    {action: function () {ensureSide (["C", "D"]);}, delay: 100},
    {action: function () {message ("The task: multiply " + a + " by " + b + " using C/D scales.");}, delay: 1000},
    {action: function () {isolate (["C", "D"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", a);}, delay: 2000},
    {action: function () {message ("Align the left index (1) on C with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("C", 1);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + b + " on the C scale.");}, delay: 4000},
    {action: function () {cursorTo ("C", b);}, delay: 1000},
    {action: function () {message ("Read the result " + crnu (a * b) + " on the D scale.");}, delay: 4000},
    {action: function () {cursorTo ("D", 1); slideTo ("C", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("D", a * b) && checkValue ("C", b)) {
        message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + crnu (a * b));
        increaseCookieResult ('Simple multiplication (C/D)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

multiplicationLessons ['Simple division (C/D)'] = function (message) {
  var a = 9, b = 9;
  while (a > 10 || b > 10 || a * b > 10) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  var ab = crnu (a * b);
  return [
    {action: function () {ensureSide (["C", "D"]);}, delay: 100},
    {action: function () {message ("The task: divide " + ab + " by " + b + " using C/D scales.");}, delay: 1000},
    {action: function () {isolate (["C", "D"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + ab + " on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", ab);}, delay: 2000},
    {action: function () {message ("Align " + b + " on the C scale with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("C", b);}, delay: 2000},
    {action: function () {message ("Move the cursor to 1 on the C scale.");}, delay: 4000},
    {action: function () {cursorTo ("C", 1);}, delay: 1000},
    {action: function () {message ("Read the result " + a + " on the D scale.");}, delay: 4000},
    {action: function () {cursorTo ("D", 1); slideTo ("C", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("D", a) && checkValue ("C", 1)) {
        message ("Mission accomplished! " + ab + " / " + b + " = " + a);
        increaseCookieResult ('Simple division (C/D)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

multiplicationLessons ['Wrap-around multiplication (C/D)'] = function (message) {
  var a = 1, b = 1;
  while (a > 10 || b > 10 || a * b < 10 || a * b > 100) {a = crnu (Math . floor (11 + Math . random () * 100) / 10); b = crnu (Math . floor (11 + Math . random () * 100) / 10);}
  return [
    {action: function () {ensureSide (["C", "D"]);}, delay: 100},
    {action: function () {message ("The task: multiply " + a + " by " + b + " using C/D scales.");}, delay: 1000},
    {action: function () {isolate (["C", "D"]); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + a + " on the D scale.");}, delay: 2000},
    {action: function () {cursorTo ("D", a);}, delay: 2000},
    {action: function () {message ("Align the right index (10) on C with the cursor.");}, delay: 4000},
    {action: function () {slideTo ("C", 10);}, delay: 2000},
    {action: function () {message ("Move the cursor to " + b + " on the C scale.");}, delay: 4000},
    {action: function () {cursorTo ("C", b);}, delay: 1000},
    {action: function () {message ("Read the value " + crnu (a * b / 10) + " on the D scale.");}, delay: 4000},
    {action: function () {message ("Adjust the decimal place and obtain the result of " + crnu (a * b) + ".");}, delay: 2000},
    {action: function () {cursorTo ("D", 1); slideTo ("C", 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ("D", a * b / 10) && checkValue ("C", b)) {
        message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + crnu (a * b));
        increaseCookieResult ('Wrap-around multiplication (C/D)');
        return true;
      }
      return false;};}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 4000}
  ];
};

slideruleLessons . push (multiplicationLessons);

