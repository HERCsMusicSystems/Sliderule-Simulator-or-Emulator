
var chemicalTests = {};

chemicalTests ['Element mass test'] = function (message) {
  var element;
  while (! element || element_mass [element . element] < 10 || element_mass [element . element] > 100) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  var masss = crnu (mass / 10);
  message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");
  sliderules . objective = function () {
    if (checkValue ('D', masss)) {
      message ("Mission accomplished! The mass of " + element . name + " = " + mass + ".");
      increaseCookieResult ('Element mass test');
      return true;
    }
    return false;
  };
};

chemicalTests ['Light element mass test'] = function (message) {
  var element;
  while (! element || element_mass [element . element] > 10) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");
  sliderules . objective = function () {
    if (checkValue ('D', mass)) {
      message ("Mission accomplished! The mass of " + element . name + " = " + mass + ".");
      increaseCookieResult ('Light element mass test');
      return true;
    }
    return false;
  };
};

chemicalTests ['Heavy element mass test'] = function (message) {
  var element;
  while (! element || element_mass [element . element] < 100) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  var masss = crnu (mass / 100);
  message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");
  sliderules . objective = function () {
    if (checkValue ('D', masss)) {
      message ("Mission accomplished! The mass of " + element . name + " = " + mass + ".");
      increaseCookieResult ('Heavy element mass test');
      return true;
    }
    return false;
  };
};

chemicalTests ['Molecular mass of a compound test'] = function (message) {
  var compound;
  while (! compound) compound = rndlist (compound_table);
  var mass = element_mass [compound . formulae];
  var masss = mass;
  if (masss > 10) masss = crnu (masss / 10, 5);
  if (masss < 1) masss = crnu (masss * 10, 5);
  var fm = formulae (compound);
  message ("The task: find molecular mass of " + compound . name + " (" + fm + ").");
  sliderules . objective = function () {
    if (checkValue ('D', masss)) {
      message ("Mission accomplished! The mass of " + compound . name + " = " + mass + ".");
      increaseCookieResult ('Molecular mass of a compound test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (chemicalTests);
