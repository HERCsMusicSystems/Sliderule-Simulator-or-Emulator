
var pythagoreanTests = {};

pythagoreanTests ['Find b on the P scale test'] = function (message) {
  var a = 0;
  while (a >= 10 || a < 2) a = Math . floor (Math . random () * 11);
  aa = crnu (a / 10);
  var b = crnu (Math . sqrt (1 - aa * aa), 5);
  message ("The task: c = 1, a = " + aa + ", find b on the P scale.");
  sliderules . objective = function () {
    if (checkValue ('P', b)) {message ("Mission accomplished! b = " + b); increaseCookieResult ('Find b on the P scale test'); return true;}
    return false;
  };
};

pythagoreanTests ['Find b on the D scale test'] = function (message) {
  var a = 0;
  while (a >= 10 || a < 2) a = Math . floor (Math . random () * 11);
  aa = crnu (a / 10);
  var b = crnu (Math . sqrt (1 - aa * aa), 6);
  var bb = crnu (Math . sqrt (1 - aa * aa) * 10, 5);
  message ("The task: c = 1, a = " + aa + ", find b on the D scale.");
  sliderules . objective = function () {
    if (checkValue ('D', bb)) {message ("Mission accomplished! b = " + b); increaseCookieResult ('Find b on the D scale test'); return true;}
    return false;
  };
};

slideruleLessons . push (pythagoreanTests);
