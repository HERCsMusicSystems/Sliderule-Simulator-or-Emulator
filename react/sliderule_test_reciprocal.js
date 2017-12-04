
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
    if (checkValue ('D', a) && checkValue ('CI', b)) {message ("Mission accomplished!"); increaseCookieResult ('Divide using CI scale test'); return true;}
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
    if (checkValue ('CI', 1) && checkValue ('D', ab)) {message ("Mission accomplished!"); increaseCookieResult ('Multiply using CI scale test'); return true;}
    return false;
  };
};

slideruleLessons . push (reciprocalTests);
