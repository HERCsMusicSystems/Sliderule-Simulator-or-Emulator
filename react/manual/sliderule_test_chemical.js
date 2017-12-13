
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

chemicalTests ['Calculate molecular mass of a compound test'] = function (message) {
  var compound;
  while (! compound) compound = rndlist (compound_table);
  var fm = formulae (compound);
  message ("The task: calculate molecular mass of " + compound . name + " (" + fm + ").");
  var total_mass = 0;
  var t1 = "", t2 = "";
  var condition = [];
  for (var ind in compound . elements) {
    var el = compound . elements [ind];
    var mass = element_mass [el . element];
    var subcompound = formulae ({elements: [{element: el . element, count: el . count}]});
    if (el . count > 1) {
      mass *= el . count;
    }
    total_mass += mass;
    t1 += (t1 == "" ? "" : " + ") + subcompound;
    t2 += (t2 == "" ? "" : " + ") + mass;
    while (mass > 10) mass /= 10;
    condition . push ({compound: subcompound, mass: mass});
  }
  sliderules . objective = function () {
    var check = true;
    for (var ind in condition) {
      if (condition [ind] != null && checkValue ('D', condition [ind] . mass)) {
        message ("Yes, the mass of " + condition [ind] . compound + " = " + condition [ind] . mass + ".");
        condition [ind] = null;
      }
      if (condition [ind] != null) check = false;
    }
    if (check) {
      message ("Mission accomplished!");
      message ("The total mass of " + fm + " = " + t1 + " = " + t2 + " = " + total_mass + ".");
      increaseCookieResult ('Calculate molecular mass of a compound test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (chemicalTests);
