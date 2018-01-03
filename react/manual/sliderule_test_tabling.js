
var tablingTests = {};

tablingTests ['Staggering calculations test'] = function (message) {
  var a = 0, b = 0, c = 0, d = 0, abc = 0;
  while (a < 1 || a > 10 || b < 0 || b > 10 || c < 1 || c > 10 || d < 1 || d > 10 || a <= c || abc > 10 || abc < d) {
    a = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    b = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    c = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    d = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    abc = a * b / c;
  }
  var result = abc / d;
  message ("The task: calculate (" + a + " * " + b + ") / (" + c + " * " + d + ")");
  sliderules . objective = function () {
    if (checkValue ('C', 1) && checkValue ('D', result)) {
      message ("Mission accomplished! (" + a + " * " + b + ") / (" + c + " * " + d + ") = " + crnu (result, 5) + ".");
      increaseCookieResult ('Staggering calculations test');
      return true;
    }
    return false;
  };
};

tablingTests ['Compounded multiplication test'] = function (message) {
  var a = 0, b = 0, c = 0, abc = 0;
  while (a <= 1 || b <= 1 || c <= 1 || a >= 10 || b >= 10 || c >= 10 || abc < 10 || abc > 100) {
    a = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    b = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    c = 1 + crnu ((Math . floor (Math . random () * 100) * 0.01) * 10);
    abc = crnu (a * b * c);
  }
  message ("The task: multiply " + a + " * " + b + " * " + c);
  sliderules . objective = function () {
    if ((checkValue ('C', c) || checkValue ('C', b) || checkValue ('C', a)) && checkValue ('D', abc / 10)) {
      message ("Mission accomplished! " + a + " * " + b + " * " + c + " = " + abc);
      increaseCookieResult ('Compounded multiplication test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (tablingTests);
