var events = require('events');
var eventEmitter = new events.EventEmitter();

//Create an event handler:
var myEventHandler = function () {
  console.log('I hear a scream!');
}

//Assign the event handler to an event:
eventEmitter.on('scream', myEventHandler);

// Add second listener to same event
eventEmitter.on('scream', () => {
  console.log("  I hear a stream too!");
});

eventEmitter.on('scream2', () => {
  console.log("I hear a second scream!");
});

eventEmitter.on('scream3', function(a, b) {
  console.log("I hear a third scream!");
  console.log(a, b, this, this == eventEmitter);
});

// NOTE: only called once
eventEmitter.once('scream4', () => {
  console.log("I hear a fourth scream!");
});

//Fire the 'scream' event:
eventEmitter.emit('scream');
eventEmitter.emit('scream2');
eventEmitter.emit('scream3');
eventEmitter.emit('scream3', 'foo', 'bar');
eventEmitter.emit('scream');
eventEmitter.emit('scream4');
eventEmitter.emit('scream4');
