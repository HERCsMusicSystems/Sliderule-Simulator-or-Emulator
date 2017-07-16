
var SlideruleApplication = React . createClass ({
  dragging: false,
  mousePosition: {x: 0, y: 0},
  onContext: function (event) {event . preventDefault ();},
  onMouseDown: function (event) {
    var position = {x: event . clientX, y: event . clientY};
    if (event . button === 2) {
      sliderules . examine (addv (position, {x: 0.5, y: 0}));
      return;
    }
    this . dragging = true;
    this . mousePosition = position;
  },
  onMouseMove: function (event) {
    if (! this . dragging) return;
    var position = {x: event . clientX, y: event . clientY};
    var delta = subv (position, this . mousePosition);
    sliderules . move (delta, position);
    this . mousePosition = position;
  },
  onMouseUp: function (event) {this . dragging = false; this . setState ({});},
  draw: function () {
  	if (sliderules . noChange ()) return;
    var width = window . innerWidth; var height = window . innerHeight;
    this . setState ({width: width, height: height});
    var ctx = this . refs . sliderule . getContext ('2d');
    ctx . setTransform (1, 0, 0, 1, 0, 0);
    ctx . fillStyle = '#99F';
    ctx . fillRect (0, 0, width, height);
    sliderules . draw (ctx);
  },
  componentDidMount: function () {setInterval (this . draw, 20);},
  getInitialState: function () {return {width: 200, height: 100, dragging: false};},
  render: function render() {
    return React.createElement(
      'div',
      { style: { align: 'center' } },
      React.createElement('canvas', { ref: 'sliderule', width: this.state.width, height: this.state.height, style: { display: 'block' },
        onMouseDown: this.onMouseDown,
        onMouseMove: this.onMouseMove,
        onMouseUp: this.onMouseUp,
        onMouseLeave: this.onMouseUp,
        onContextMenu: this.onContext
      })
    );
  }
});
