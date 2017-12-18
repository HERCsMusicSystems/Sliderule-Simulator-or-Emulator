
var logTests = [];

logTests ['Raise e to a power between 1 and 10 test'] = function (message) {
  var power = crnu (Math . floor (Math . random () * 100) / 10);
  var result = crnu (Math . pow (Math . E, power), 5);
  message ("The task: raise <i>e</i> to the power of " + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', power)) {
      message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between 1 and 10 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between 0.1 and 1 test'] = function (message) {
  var power = crnu (Math . floor (Math . random () * 100) / 100);
  var to = crnu (power * 10);
  var result = crnu (Math . pow (Math . E, power), 5);
  message ("The task: raise <i>e</i> to the power of " + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between 0.1 and 1 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between 0.01 and 0.1 test'] = function (message) {
  var power = crnu (Math . floor (Math . random () * 100) / 1000);
  var to = crnu (power * 100);
  var result = crnu (Math . pow (Math . E, power), 5);
  message ("The task: raise <i>e</i> to the power of " + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between 0.01 and 0.1 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between 0.001 and 0.01 test'] = function (message) {
  var power = crnu (Math . floor (Math . random () * 100) / 10000);
  var to = crnu (power * 1000);
  var result = crnu (Math . pow (Math . E, power), 5);
  message ("The task: raise <i>e</i> to the power of " + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between 0.001 and 0.01 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise any number to any power test'] = function (message) {
  var a = 0, b = 0, x = 0;
  while (a <= 1 || b <= 1 || a * b > 10) {
    a = 1 + Math . random () * 10;
    b = crnu (1 + Math . floor (Math . random () * 100) / 10);
    x = Math . pow (Math . E, a);
    if (x < 10) x = crnu (x, 1);
    else if (x <= 100) x = crnu (x, 0);
    else b = 0;
    a = Math . log (x);
  }
  var aa = crnu (a, 5);
  var result = crnu (Math . pow (x, b), 5);
  message ("The task: raise " + x + " to the power of " + b + ".");
  sliderules . objective = function () {
    if (checkValue ('LL3', result) && checkValue ('C', b)) {
      message ("Mission accomplished! " + x + " to the power of " + b + " = " + result + ".");
      increaseCookieResult ('Raise any number to any power test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between -1 and -10 test'] = function (message) {
  var power = 0; while (power <= 1) power = crnu (Math . floor (Math . random () * 100) / 10);
  var result = crnu (Math . pow (Math . E, - power), 5);
  message ("The task: raise <i>e</i> to the power of -" + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', power)) {
      message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between -1 and -10 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between -0.1 and -1 test'] = function (message) {
  var power = 0; while (power <= 0.1) power = crnu (Math . floor (Math . random () * 100) / 100);
  var to = crnu (power * 10);
  var result = crnu (Math . pow (Math . E, - power), 5);
  message ("The task: raise <i>e</i> to the power of -" + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between -0.1 and -1 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between -0.01 and -0.1 test'] = function (message) {
  var power = 0; while (power <= 0.01) power = crnu (Math . floor (Math . random () * 100) / 1000);
  var to = crnu (power * 100);
  var result = crnu (Math . pow (Math . E, - power), 5);
  message ("The task: raise <i>e</i> to the power of -" + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between -0.01 and -0.1 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise e to a power between -0.001 and -0.01 test'] = function (message) {
  var power = 0; while (power <= 0.001) power = crnu (Math . floor (Math . random () * 100) / 10000);
  var to = crnu (power * 1000);
  var result = crnu (Math . pow (Math . E, - power), 5);
  message ("The task: raise <i>e</i> to the power of -" + power + ".");
  sliderules . objective = function () {
    if (checkValue ('D', to)) {
      message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
      increaseCookieResult ('Raise e to a power between -0.001 and -0.01 test');
      return true;
    }
    return false;
  };
};

logTests ['Raise any number to any negative power test'] = function (message) {
  var a = 0, b = 0, x = 0;
  while (a <= 1 || b <= 1 || a * b > 10) {
    a = 1 + Math . random () * 10;
    b = crnu (1 + Math . floor (Math . random () * 100) / 10);
    x = Math . pow (Math . E, a);
    if (x < 10) x = crnu (x, 1);
    else if (x <= 100) x = crnu (x, 0);
    else b = 0;
    a = Math . log (x);
  }
  var aa = crnu (a, 5);
  var result = crnu (Math . pow (x, - b), 5);
  message ("The task: raise " + x + " to the power of -" + b + ".");
  sliderules . objective = function () {
    if (checkValue ('LL03', result) && checkValue ('C', b)) {
      message ("Mission accomplished! " + x + " to the power of -" + b + " = " + result + ".");
      increaseCookieResult ('Raise any number to any negative power test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (logTests);
