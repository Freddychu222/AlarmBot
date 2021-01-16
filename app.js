(function(window,document,undefined){
  function $(id) {
    return document.getElementById(id);
  }

  class Clock extends Date {
    constructor(element) {
      super(); //call parent Date class
      var self = this; //only used for keeping setInterval in Clock namespace
      this.element = element; //html clock element
      this.now = new Date();
      this.currentTime = {
        s: this.getSeconds(),
        m: this.getMinutes(),
        h: this.formatHours(this.getHours()) //format current hour to 12-hour clock
      };
      this.timeOutput = $('time-output'); //Clock label
      this.timeOutput.innerHTML = this.now.toLocaleTimeString();
      this.build();
      this.timer = setInterval(function() {
        self.update();
      }, 1000);
    }
  }

  Clock.prototype.formatHours = function formatHours(hours) {
    var formatted = hours > 12 ? hours - 12 : hours; //if hour is greater than 12, subtract 12. Else keep the same.
    return formatted;
  };

  Clock.prototype.update = function update() {
    this.now = new Date();
    this.timeOutput.innerHTML = this.now.toLocaleTimeString();
    this.tickSeconds();
    if (this.minuteChanged) {
      this.tickMinutes();
    }
    if (this.hourChanged) {
      this.tickHours();
    }
  };

  Clock.prototype.build = function build() {
    this.step = { //step rotation in degrees
      s: 6, //seconds
      m: 6, //minutes
      h: 30 //hours
    };
    this.rotation = {
      s: this.getSeconds() * this.step.s,
      m: this.getMinutes() * this.step.m,
      h: this.formatHours(this.getHours()) * this.step.h
    };
    this.ticker = { //html 'hand' elements
      second: this.element.getElementsByClassName('second')[0],
      minute: this.element.getElementsByClassName('minute')[0],
      hour: this.element.getElementsByClassName('hour')[0]
    };
    this.tickColor = {
      s: '#9cf', //blue
      m: '#9fc', //green
      h: '#c9f', //purple
      n: 'transparent'
    };
    this.tickList = $('tick-marks'); //html ul element surrounding clock face
    this.tickMarks = [];
    this.ticker.second.style.transform = 'rotate(' + this.rotation.s + 'deg)'; //rotate to current time values
    this.ticker.minute.style.transform = 'rotate(' + this.rotation.m + 'deg)';
    this.ticker.hour.style.transform = 'rotate(' + this.rotation.h + 'deg)';
    var diameter = this.element.offsetWidth,
      radius = diameter * 0.5, //radius of circle for tick marks to be placed in
      angle = 0,
      step = (2 * Math.PI) / 60, //radians
      tickMark, offset, x, y;
    for (var i = 0; i < 60; i++) {
      offset = i % 5 === 0 ? 7 : 4; //every 5th tick mark is offset to compensate for larger size
      x = Math.round((radius * Math.cos(angle)) + radius - offset); //calculate cosine of current angle with radius of tickmarks circle
      y = Math.round((radius * Math.sin(angle)) + radius - offset); //sine
      angle += step; //increment angle by 6deg (2pi/60 radians)
      tickMark = document.createElement('li'); //create and style tick mark
      tickMark.id = 't' + (i + 1);
      tickMark.className = 'tick-mark';
      tickMark.style.left = x + 'px';
      tickMark.style.top = y + 'px';
      this.tickMarks.push(tickMark);
      this.tickList.appendChild(tickMark);
    }
    this.highlight();
  };

  Clock.prototype.tickSeconds = function tickSeconds() {
    this.currentTime.s = this.now.getSeconds();
    this.rotation.s += this.step.s;
    this.ticker.second.style.transform = 'rotate(' + this.rotation.s + 'deg)';
    if (this.now.getSeconds() === 0) {
      this.minuteChanged = true;
    }
    this.highlight();
  };

  Clock.prototype.tickMinutes = function tickMinutes() {
    this.currentTime.m = this.now.getMinutes();
    this.rotation.m += this.step.m;
    this.ticker.minute.style.transform = 'rotate(' + this.rotation.m + 'deg)';
    if (this.now.getMinutes() === 0) {
      this.hourChanged = true;
    }
    this.highlight();
    this.minuteChanged = false;
  };

  Clock.prototype.tickHours = function tickHours() {
    this.currentTime.h = this.formatHours(this.now.getHours());
    this.rotation.h += this.step.h;
    this.ticker.hour.style.transform = 'rotate(' + this.rotation.h + 'deg)';
    this.highlight();
    this.hourChanged = false;
  };

  Clock.prototype.highlight = function highlight() {
    var curS = this.currentTime.s, //variables for tickmarks indexes
      preS = curS === 0 ? 59 : curS - 1;

    var curM = this.currentTime.m,
      preM = curM === 0 ? 59 : curM - 1;

    var curH = this.currentTime.h * 5 == 60 ? 0 : this.currentTime.h * 5,
      preH = curH === 0 ? 55 : curH - 5;

    this.tickMarks[curM].style.transitionDuration = this.tickMarks[preM].style.transitionDuration = '600ms';
    this.tickMarks[curH].style.transitionDuration = this.tickMarks[preH].style.transitionDuration = '1000ms';

    this.tickMarks[preS].style.backgroundColor = this.tickColor.n;
    this.tickMarks[curS].style.backgroundColor = this.tickColor.s;

    if (this.minuteChanged) {
      this.tickMarks[preM].style.backgroundColor = this.tickColor.n;
    }
    this.tickMarks[curM].style.backgroundColor = this.tickColor.m;

    if (this.hourChanged) {
      this.tickMarks[preH].style.backgroundColor = this.tickColor.n;
    }
    this.tickMarks[curH].style.backgroundColor = this.tickColor.h;
  };

  window.onload = function(){
    return new Clock($('clock'));
  };
}(window,document));
