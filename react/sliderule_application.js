
var SlideruleApplication = React . createClass ({
  dragging: false,
  mousePosition: {x: 0, y: 0},
  onContext: function (event) {event . preventDefault ();},
  onMouseDown: function (event) {
    var position = subv ({x: event . clientX, y: event . clientY}, this . refs . sliderule . getBoundingClientRect ());
    if (event . button === 2) return sliderules . synchroniseTarget (addv (position, {x: 0.5, y: 0}));
    this . dragging = true;
    this . mousePosition = position;
  },
  onMouseMove: function (event) {
    if (! this . dragging) return;
    var position = subv ({x: event . clientX, y: event . clientY}, this . refs . sliderule . getBoundingClientRect ());
    var delta = subv (position, this . mousePosition);
    sliderules . synchroniseMove (delta, position);
    this . mousePosition = position;
  },
  onMouseUp: function (event) {this . dragging = false; this . setState ({});},
  SM: function (event) {
    for (var ind in sliderules . sliderules) sliderules . sliderules [ind] . static_markings = event . target . checked;
    sliderules . requireRedraw = true;
  },
  CM: function (event) {
    for (var ind in sliderules . sliderules) sliderules . sliderules [ind] . cursor_markings = event . target . checked;
    sliderules . requireRedraw = true;
  },
  EM: function (event) {
    for (var ind in sliderules . sliderules) sliderules . sliderules [ind] . extra_cursor_markings = event . target . checked;
    sliderules . requireRedraw = true;
  },
  draw: function () {
  	if (sliderules . noChange ()) return;
    var width = window . innerWidth; var height = window . innerHeight;
    var bound = this . refs . sliderule . getBoundingClientRect ();
    this . setState ({width: width - bound . x * 4, height: height - bound . y * 4});
    var ctx = this . refs . sliderule . getContext ('2d');
    sliderules . draw (ctx, width, height);
  },
  componentDidMount: function () {setInterval (this . draw, 20);},
  getInitialState: function () {return {width: 200, height: 100, dragging: false};},
  render: function render() {
    return React.createElement(
      'div',
      { style: { align: 'center', position: 'relative' } }, [
      React.createElement('canvas', { key: 1, ref: 'sliderule', width: this.state.width, height: this.state.height, style: { display: 'block' },
        onMouseDown: this.onMouseDown,
        onMouseMove: this.onMouseMove,
        onMouseUp: this.onMouseUp,
        onMouseLeave: this.onMouseUp,
        onContextMenu: this.onContext
      }),
      React . createElement ('input', {key: 2, type: 'checkbox', onChange: this . SM, style: {position: 'absolute', left: '0px', top: '0px'}}),
      React . createElement ('input', {key: 3, type: 'checkbox', onChange: this . CM, style: {position: 'absolute', left: '0px', top: '15px'}}),
      React . createElement ('input', {key: 4, type: 'checkbox', onChange: this . EM, style: {position: 'absolute', left: '0px', top: '30px'}})
      ]
    );
  }
});
