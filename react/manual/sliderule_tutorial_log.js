
var logLessons = [];

logLessons ['Raise e to a power between 1 and 10'] = function (message) {
  var power = 0; while (power <= 1) power = crnu (Math . floor (Math . random () * 100) / 10);
  var result = crnu (Math . pow (Math . E, power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL3']); isolate (['D', 'LL3']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of " + power + ".");}, delay: 1000},
    {action: function () {message ("Move the cursor to " + power + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', power);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL3 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', power)) {
        message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between 1 and 10');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between 0.1 and 1'] = function (message) {
  var power = 0; while (power <= 0.1) power = crnu (Math . floor (Math . random () * 100) / 100);
  var to = crnu (power * 10);
  var result = crnu (Math . pow (Math . E, power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL2']); isolate (['D', 'LL2']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of " + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL2 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between 0.1 and 1');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between 0.01 and 0.1'] = function (message) {
  var power = 0; while (power <= 0.01) power = crnu (Math . floor (Math . random () * 100) / 1000);
  var to = crnu (power * 100);
  var result = crnu (Math . pow (Math . E, power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL1']); isolate (['D', 'LL1']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of " + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL1 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between 0.01 and 0.1');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between 0.001 and 0.01'] = function (message) {
  var power = 0; while (power <= 0.001) power = crnu (Math . floor (Math . random () * 100) / 10000);
  var to = crnu (power * 1000);
  var result = crnu (Math . pow (Math . E, power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL3']); isolate (['D']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of " + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the value of " + power + " on the D scale.");}, delay: 4000},
    {action: function () {message ("Observe the additional red markings for log correction along the D scale.");}, delay: 2000},
    {action: function () {message ("Adjust the decimal point and add 1 to obtain the result of " + result + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of " + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between 0.001 and 0.01');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise any number to any power'] = function (message) {
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
  return [
    {action: function () {ensureSide (['D', 'C', 'LL3']); isolate (['D', 'C', 'LL3']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise " + x + " to the power of " + b + ".");}, delay: 500},
    {action: function () {message ("Move cursor to " + x + " on the LL3 scale.");}, delay: 3000},
    {action: function () {cursorTo ('LL3', x);}, delay: 1000},
    {action: function () {message ("The natural logarithm of " + x + " is " + aa + " and can be found on the D scale.");}, delay: 3000},
    {action: function () {message ("Now multiply " + aa + " by " + b + ".");}, delay: 2000},
    {action: function () {message ("Align 1 on the C scale with the cursor");}, delay: 1000},
    {action: function () {slideTo ('C', 1);}, delay: 1000},
    {action: function () {message ("Move cursor to " + b + " on the C scale.");}, delay: 3000},
    {action: function () {cursorTo ('C', b);}, delay: 1000},
    {action: function () {message ("Read the result: " + x + " to the power of " + b + " = " + result + ".");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('LL3', result) && checkValue ('C', b)) {
        message ("Mission accomplished! " + x + " to the power of " + b + " = " + result + ".");
        increaseCookieResult ('Raise any number to any power');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between -1 and -10'] = function (message) {
  var power = 0; while (power <= 1) power = crnu (Math . floor (Math . random () * 100) / 10);
  var result = crnu (Math . pow (Math . E, - power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL03']); isolate (['D', 'LL03']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of -" + power + ".");}, delay: 1000},
    {action: function () {message ("Move the cursor to " + power + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', power);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL03 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', power)) {
        message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between -1 and -10');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between -0.1 and -1'] = function (message) {
  var power = 0; while (power <= 0.1) power = crnu (Math . floor (Math . random () * 100) / 100);
  var to = crnu (power * 10);
  var result = crnu (Math . pow (Math . E, - power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL02']); isolate (['D', 'LL02']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of -" + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL02 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between -0.1 and -1');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between -0.01 and -0.1'] = function (message) {
  var power = 0; while (power <= 0.01) power = crnu (Math . floor (Math . random () * 100) / 1000);
  var to = crnu (power * 100);
  var result = crnu (Math . pow (Math . E, - power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL01']); isolate (['D', 'LL01']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of -" + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL01 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between -0.01 and -0.1');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise e to a power between -0.001 and -0.01'] = function (message) {
  var power = 0; while (power <= 0.001) power = crnu (Math . floor (Math . random () * 100) / 10000);
  var to = crnu (power * 1000);
  var result = crnu (Math . pow (Math . E, - power), 5);
  return [
    {action: function () {ensureSide (['D', 'LL00']); isolate (['D', 'LL00']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise <i>e</i> to the power of -" + power + ".");}, delay: 1000},
    {action: function () {message ("Adjust the decimal point and move the cursor to " + to + " on the D scale.");}, delay: 3000},
    {action: function () {cursorTo ('D', to);}, delay: 1000},
    {action: function () {message ("Read the result of " + result + " on the LL00 scale.");}, delay: 4000},
    {action: function () {cursorTo ('D', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('D', to)) {
        message ("Mission accomplished! <i>e</i> to the power of -" + power + " is " + result + ".");
        increaseCookieResult ('Raise e to a power between -0.001 and -0.01');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

logLessons ['Raise any number to any negative power'] = function (message) {
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
  return [
    {action: function () {ensureSide (['D', 'C', 'LL3', 'LL03']); isolate (['D', 'C', 'LL3', 'LL03']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
    {action: function () {message ("The task: raise " + x + " to the power of -" + b + ".");}, delay: 500},
    {action: function () {message ("Move cursor to " + x + " on the LL3 scale.");}, delay: 3000},
    {action: function () {cursorTo ('LL3', x);}, delay: 1000},
    {action: function () {message ("The natural logarithm of " + x + " is " + aa + " and can be found on the D scale.");}, delay: 3000},
    {action: function () {message ("Now multiply " + aa + " by " + b + ".");}, delay: 2000},
    {action: function () {message ("Align 1 on the C scale with the cursor");}, delay: 1000},
    {action: function () {slideTo ('C', 1);}, delay: 1000},
    {action: function () {message ("Move cursor to " + b + " on the C scale.");}, delay: 3000},
    {action: function () {cursorTo ('C', b);}, delay: 1000},
    {action: function () {message ("Read the result: " + x + " to the power of -" + b + " = " + result + ".");}, delay: 3000},
    {action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (40, 255, 12);
    sliderules . objective = function () {
      if (checkValue ('LL03', result) && checkValue ('C', b)) {
        message ("Mission accomplished! " + x + " to the power of -" + b + " = " + result + ".");
        increaseCookieResult ('Raise any number to any negative power');
        return true;
      }
      return false;
    };}, delay: 7000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

slideruleLessons . push (logLessons);
