var console = require('console');
var http = require('http');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
var RuntimeAPI = { _current_app: undefined };

RuntimeAPI._removeCurrentApp = function () {
  this._current_app = undefined;
};

RuntimeAPI.setCurrentApp = function (onInitialize, onStart, onStop) {
  if (!(onInitialize instanceof Function) || !(onStart instanceof Function) ||
    !(onStop instanceof Function)) {
    return RESULT_FAILED;
  }
  this._current_app = new App(onInitialize, onStart, onStop);

  return RESULT_SUCCESS;
};

RuntimeAPI.getCurrentApp = function () {
  return this._current_app;
};

/* App start */
function App(onInitialize, onStart, onStop) {
  this.state = this.STATE.IDLE;
  this.onInitialize = onInitialize;
  this.onStart = onStart;
  this.onStop = onStop;

  this.onInitialize();
}

App.prototype.STATE = {};
App.prototype.STATE.IDLE = 0;
App.prototype.STATE.RUNNING = 1;

App.prototype.start = function () {
  if (this.state == this.STATE.RUNNING) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.RUNNING;
  this.onStart();
  return RESULT_SUCCESS;
};

App.prototype.stop = function () {
  if (this.state == this.STATE.IDLE) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.IDLE;
  this.onStop();
  return RESULT_SUCCESS;
};

App.prototype.getState = function () {
  if (this.state == this.STATE.IDLE) {
    return 'Idle';
  } else if (this.state == this.STATE.RUNNING) {
    return 'Running';
  } else {
    return 'Unknown';
  }
};

App.prototype.getInfo = function () {
  var appInfo = { 'state': this.getState() };
  return appInfo;
};
/* App end */

/** Runtime API end **/


/** Stream API start **/
// TODO: native-side Stream API implementation is required.
function StreamAPI() { }
StreamAPI.createPipeline = function (pipeline_name) {
  return new Pipeline(pipeline_name);
};
StreamAPI.createElement() = function (element_name) {
  return new Element(element_name);
};

function Pipeline(name) {
  this.name = name;
  this.elements = [];
}
Pipeline.prototype.add = function (element) {
  // TODO: type check for element is required
  this.elements.push(element);
}

function Element(name) {
  this.name = name;
  this.properties = {};
  this.isSinkElement = false;
  this.handlers = {};
}
Element.prototype.setProperty = function (key, value) {
  // TODO: type check for key, value is required
  // TODO: check for existing property is required
  // TODO: success/failed result is required
  this.properties[key] = value;
}
Element.prototype.setSinkElement = function (isSinkElement) {
  this.isSinkElement = isSinkElement;
}
Element.prototype.connect = function (detailedSignal, handler) {
  // TODO: type check for detailedSignal and handler is required
  // TODO: check for existing handler is required
  // TODO: success/failed result is required
  this.handlers[detailedSignal] = handler;
}

/** Stream API end **/

/** Companion API start **/
function CompanionAPI() {
  this._companionHost = undefined;
  this._companionPort = undefined;
  this._companionPath = undefined;
  this._handler = undefined;
}

CompanionAPI._setCompanionAddress = function (
  companionHost, companionPort, companionPath) {
  this._companionHost = companionHost;
  this._companionPort = companionPort;
  this._companionPath = companionPath;
  return true;
};

CompanionAPI._onReceiveMessageFromCompanion = function (message) {
  this._handler(message);
};

CompanionAPI.sendMessage = function (message) {
  if (this._companionPath === undefined) {
    console.log('Error: failed to send message due to no companion address');
    return false;
  }

  var options = {
    method: 'POST',
    host: this._companionHost,
    port: this._companionPort,
    path: this._companionPath,
    headers: { 'Content-Length': message.length },
  };
  var client_request = http.request(options);
  client_request.write(message);
  client_request.end();
  return true;
};

CompanionAPI.setOnReceiveMessage = function (handler) {
  this._handler = handler;
};
/** Companion API end **/


/** Compression Server API start **/
function CompressionServerAPI() {
  this.ipAddress = undefined;
}
CompressionServerAPI.connect = function (ipAddress) {
  this.ipAddress = ipAddress;
};
CompressionServerAPI.downloadModel = function (modelId) {
  if (this.ipAddress === undefined)
    return undefined;
};
CompressionServerAPI.downloadKernel = function (kernelId) {
  if (this.ipAddress === undefined)
    return undefined;
};
/** Compression Server API end **/


/** ANT main object start **/
function ANT() { }

ANT.prototype.runtime = RuntimeAPI;
ANT.prototype.stream = StreamAPI;
ANT.prototype.companion = CompanionAPI;
ANT.prototype.compressionServer = CompressionServerAPI;

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;