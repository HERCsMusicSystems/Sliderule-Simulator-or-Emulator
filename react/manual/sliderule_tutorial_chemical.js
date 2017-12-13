
var chemicalLessons = {};

chemicalLessons ['Element mass'] = function (message) {
  var element;
  while (! element || element_mass [element . element] < 10 || element_mass [element . element] > 100) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  var masss = crnu (mass / 10);
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + element . element + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', masss);}, delay: 1500},
    {action: function () {message ("Read " + masss + " on the D scale.");}, delay: 3000},
    {action: function () {message ("Adjust decimal point to obtain the molecular mass of " + element . name + " = " + mass + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', masss)) {message ("Mission accomplished!"); increaseCookieResult ('Element mass'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

chemicalLessons ['Light element mass'] = function (message) {
  var element;
  while (! element || element_mass [element . element] > 10) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + element . element + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', mass);}, delay: 1500},
    {action: function () {message ("Read the molecular mass of " + element . name + " = " + mass + ".");}, delay: 3000},
    {action: function () {message ("Since " + element . name + " is a light element, as indicated by underscore....");}, delay: 3000},
    {action: function () {message (".... you do not need to adjust the decimal point.");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', mass)) {message ("Mission accomplished!"); increaseCookieResult ('Light element mass'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

chemicalLessons ['Heavy element mass'] = function (message) {
  var element;
  while (! element || element_mass [element . element] < 100) element = rndlist (periodic_table);
  var mass = element_mass [element . element];
  var masss = crnu (mass / 100);
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + element . name + " (" + element . element + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + element . element + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', masss);}, delay: 1500},
    {action: function () {message ("Read " + masss + " on the D scale.");}, delay: 3000},
    {action: function () {message ("Since " + element . name + " is a heavy element, as indicated by overscore....");}, delay: 3000},
    {action: function () {message (".... you need to adjust the decimal point to obtain the molecular mass of " + element . name + " = " + mass + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', masss)) {message ("Mission accomplished!"); increaseCookieResult ('Heavy element mass'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

chemicalLessons ['Molecular mass of a compound'] = function (message) {
  var compound;
  while (! compound) compound = rndlist (compound_table);
  var mass = element_mass [compound . formulae];
  var masss = mass;
  if (masss > 10) masss = crnu (masss / 10, 5);
  if (masss < 1) masss = crnu (masss * 10, 5);
  var fm = formulae (compound);
  return [
    {action: function () {ensureSide (['A~M', 'N~Z', 'D']); isolate (['A~M', 'N~Z', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100},
    {action: function () {message ("The task: find molecular mass of " + compound . name + " (" + fm + ").");}, delay: 500},
    {action: function () {message ("Right-click on the cursor over the Chemical scale and enter " + compound . formulae + ".");}, delay: 2000},
    {action: function () {cursorTo ('D', masss);}, delay: 1500},
    {action: function () {message ("Read " + masss + " on the D scale.");}, delay: 3000},
    {action: function () {if (mass != masss) message ("Adjust decimal point to obtain mass = " + mass + ".");}, delay: 1000},
    {action: function () {cursorTo ('D', 1); dimmm (80, 255, 8);
    sliderules . objective = function () {
      if (checkValue ('D', masss)) {message ("Mission accomplished!"); increaseCookieResult ('Molecular mass of a compound'); return true;}
      return false;
    };}, delay: 6000},
    {action: function () {isolate (); message ("Try these instructions again.");}, delay: 3000}
  ];
};

chemicalLessons ['Calculate molecular mass of a compound'] = function (message) {
  var compound;
  while (! compound) compound = rndlist (compound_table);
  var fm = formulae (compound);
  var ret = [];
  ret . push ({action: function () {ensureSide (['A~M', 'N~Z', 'C', 'D']); isolate (['A~M', 'N~Z', 'C', 'D']); changeMarkings ('hairline', true); dimmm (255, 80, 8);}, delay: 100});
  ret . push ({action: function () {message ("The task: calculate molecular mass of " + compound . name + " (" + fm + ").");}, delay: 500});
  var total_mass = 0;
  var t1 = "", t2 = "";
  var condition = [];
  for (var ind in compound . elements) {
    var el = compound . elements [ind];
    var mass = element_mass [el . element];
    ret . push ({action: function (element, mass) {return function () {message ("Right-click on the cursor to obtain the mass of " + element + " = " + mass + ".");};} (el . element, mass), delay: 3000});
    ret . push ({action: function (mass) {return function () {cursorTo ('D', mass);};} (getMolecularMass (el . element)), delay: 1000});
    var subcompound = formulae ({elements: [{element: el . element, count: el . count}]});
    if (el . count > 1) {
      mass *= el . count;
      ret . push ({action: function (compound, mass) {return function () {message ("Use the C scale to obtain the mass of " + compound + " = " + mass + ".");};} (subcompound, mass), delay: 3000});
      ret . push ({action: function () {slideTo ('C', 1);}, delay: 1000});
      ret . push ({action: function (count) {return function () {cursorTo ('C', count);};} (el . count), delay: 1000});
    }
    total_mass += mass;
    t1 += (t1 == "" ? "" : " + ") + subcompound;
    t2 += (t2 == "" ? "" : " + ") + mass;
    while (mass > 10) mass /= 10;
    condition . push (mass);
  }
  ret . push ({action: function () {message ("The total mass of " + fm + " = " + t1 + " = " + t2 + " = " + total_mass + ".");}, delay: 3000});
  ret . push ({action: function () {cursorTo ('D', 1); slideTo ('C', 1); dimmm (80, 255, 8);
  sliderules . objective = function () {
    var check = true;
    for (var ind in condition) {if (checkValue ('D', condition [ind])) condition [ind] = 0; if (condition [ind] != 0) check = false;}
    if (check) {message ("Mission accomplished!"); increaseCookieResult ('Calculate molecular mass of a compound'); return true;}
    return false;
  };
  }, delay: 6000});
  ret . push ({action: function () {message ("Try these instructions again.");}, delay: 3000});
  return ret;
};

slideruleLessons . push (chemicalLessons);
