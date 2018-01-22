
var reciprocalTests = {};

reciprocalTests ['Divide using CI scale test'] = function (message) {
  var a = 0, b = 0, ab = 0;
  while (ab > 10 || a <= 1 || b <= 1) {
    a = 1 + crnu (Math . floor (Math . random () * 100) / 10);
    b = 1 + crnu (Math . floor (Math . random () * 100) / 10);
    ab = crnu (a * b);
  }
  message ("The task: divide " + ab + " by " + b + " using CI scale.");
  sliderules . objective = function () {
    if (checkValue ('D', a) && checkValue ('CI', b)) {
      message ("Mission accomplished! " + ab + " / " + b + " = " + a);
      increaseCookieResult ('Divide using CI scale test');
      return true;
    }
    return false;
  };
};

reciprocalTests ['Multiply using CI scale test'] = function (message) {
  var a = 0, b = 0, ab = 0;
  while (ab > 10 || a <= 1 || b <= 1) {
    a = 1 + crnu (Math . floor (Math . random () * 100) / 10);
    b = 1 + crnu (Math . floor (Math . random () * 100) / 10);
    ab = crnu (a * b);
  }
  message ("The task: multiply " + a + " by " + b + " using CI scale.");
  sliderules . objective = function () {
    if (checkValue ('CI', 1) && checkValue ('D', ab)) {
      message ("Mission accomplished! " + a + " \u00d7 " + b + " = " + ab);
      increaseCookieResult ('Multiply using CI scale test');
      return true;
    }
    return false;
  };
};

reciprocalTests ['Divide by fraction using CI scale test'] = function (message) {
  var a = 0, b = 0;
  while (a <= 1 || b <= 1 || a > 9 || b > 9 || b < a) {
    a = Math . floor (Math . random () * 11);
    b = Math . floor (Math . random () * 11);
  }
  var bb = crnu (b / 10);
  var ab = crnu (a / bb, 5);
  message ("The task: divide " + a + " by " + bb + " starting from the CI scale.");
  sliderules . objective = function () {
    if (checkValue ('CI', ab) && checkValue ('D', b)) {
      message ("Mission accomplished! " + a + " / " + bb + " = " + ab);
      increaseCookieResult ('Divide by fraction using CI scale test');
      return true;
    }
    return false;
  };
};
slideruleLessons . push (reciprocalTests);
