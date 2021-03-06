// source: signaling.proto
/**
 * @fileoverview
 * @enhanceable
 * @suppress {missingRequire} reports error on implicit type usages.
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!
/* eslint-disable */
// @ts-nocheck

goog.provide('proto.signaling.Signaling');

goog.require('jspb.BinaryReader');
goog.require('jspb.BinaryWriter');
goog.require('jspb.Message');
goog.require('proto.transfer.File');
goog.require('proto.transfer.Record');
goog.require('proto.transfer.User');

/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.signaling.Signaling = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.signaling.Signaling.repeatedFields_, null);
};
goog.inherits(proto.signaling.Signaling, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.signaling.Signaling.displayName = 'proto.signaling.Signaling';
}

/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.signaling.Signaling.repeatedFields_ = [3,4,5];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.signaling.Signaling.prototype.toObject = function(opt_includeInstance) {
  return proto.signaling.Signaling.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.signaling.Signaling} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.signaling.Signaling.toObject = function(includeInstance, msg) {
  var f, obj = {
    command: jspb.Message.getFieldWithDefault(msg, 1, ""),
    text: jspb.Message.getFieldWithDefault(msg, 2, ""),
    usersList: jspb.Message.toObjectList(msg.getUsersList(),
    proto.transfer.User.toObject, includeInstance),
    filesList: jspb.Message.toObjectList(msg.getFilesList(),
    proto.transfer.File.toObject, includeInstance),
    recordsList: jspb.Message.toObjectList(msg.getRecordsList(),
    proto.transfer.Record.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.signaling.Signaling}
 */
proto.signaling.Signaling.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.signaling.Signaling;
  return proto.signaling.Signaling.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.signaling.Signaling} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.signaling.Signaling}
 */
proto.signaling.Signaling.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {string} */ (reader.readString());
      msg.setCommand(value);
      break;
    case 2:
      var value = /** @type {string} */ (reader.readString());
      msg.setText(value);
      break;
    case 3:
      var value = new proto.transfer.User;
      reader.readMessage(value,proto.transfer.User.deserializeBinaryFromReader);
      msg.addUsers(value);
      break;
    case 4:
      var value = new proto.transfer.File;
      reader.readMessage(value,proto.transfer.File.deserializeBinaryFromReader);
      msg.addFiles(value);
      break;
    case 5:
      var value = new proto.transfer.Record;
      reader.readMessage(value,proto.transfer.Record.deserializeBinaryFromReader);
      msg.addRecords(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.signaling.Signaling.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.signaling.Signaling.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.signaling.Signaling} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.signaling.Signaling.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getCommand();
  if (f.length > 0) {
    writer.writeString(
      1,
      f
    );
  }
  f = message.getText();
  if (f.length > 0) {
    writer.writeString(
      2,
      f
    );
  }
  f = message.getUsersList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      3,
      f,
      proto.transfer.User.serializeBinaryToWriter
    );
  }
  f = message.getFilesList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      4,
      f,
      proto.transfer.File.serializeBinaryToWriter
    );
  }
  f = message.getRecordsList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      5,
      f,
      proto.transfer.Record.serializeBinaryToWriter
    );
  }
};


/**
 * optional string command = 1;
 * @return {string}
 */
proto.signaling.Signaling.prototype.getCommand = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 1, ""));
};


/**
 * @param {string} value
 * @return {!proto.signaling.Signaling} returns this
 */
proto.signaling.Signaling.prototype.setCommand = function(value) {
  return jspb.Message.setProto3StringField(this, 1, value);
};


/**
 * optional string text = 2;
 * @return {string}
 */
proto.signaling.Signaling.prototype.getText = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 2, ""));
};


/**
 * @param {string} value
 * @return {!proto.signaling.Signaling} returns this
 */
proto.signaling.Signaling.prototype.setText = function(value) {
  return jspb.Message.setProto3StringField(this, 2, value);
};


/**
 * repeated transfer.User users = 3;
 * @return {!Array<!proto.transfer.User>}
 */
proto.signaling.Signaling.prototype.getUsersList = function() {
  return /** @type{!Array<!proto.transfer.User>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.transfer.User, 3));
};


/**
 * @param {!Array<!proto.transfer.User>} value
 * @return {!proto.signaling.Signaling} returns this
*/
proto.signaling.Signaling.prototype.setUsersList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 3, value);
};


/**
 * @param {!proto.transfer.User=} opt_value
 * @param {number=} opt_index
 * @return {!proto.transfer.User}
 */
proto.signaling.Signaling.prototype.addUsers = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 3, opt_value, proto.transfer.User, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.signaling.Signaling} returns this
 */
proto.signaling.Signaling.prototype.clearUsersList = function() {
  return this.setUsersList([]);
};


/**
 * repeated transfer.File files = 4;
 * @return {!Array<!proto.transfer.File>}
 */
proto.signaling.Signaling.prototype.getFilesList = function() {
  return /** @type{!Array<!proto.transfer.File>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.transfer.File, 4));
};


/**
 * @param {!Array<!proto.transfer.File>} value
 * @return {!proto.signaling.Signaling} returns this
*/
proto.signaling.Signaling.prototype.setFilesList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 4, value);
};


/**
 * @param {!proto.transfer.File=} opt_value
 * @param {number=} opt_index
 * @return {!proto.transfer.File}
 */
proto.signaling.Signaling.prototype.addFiles = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 4, opt_value, proto.transfer.File, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.signaling.Signaling} returns this
 */
proto.signaling.Signaling.prototype.clearFilesList = function() {
  return this.setFilesList([]);
};


/**
 * repeated transfer.Record records = 5;
 * @return {!Array<!proto.transfer.Record>}
 */
proto.signaling.Signaling.prototype.getRecordsList = function() {
  return /** @type{!Array<!proto.transfer.Record>} */ (
    jspb.Message.getRepeatedWrapperField(this, proto.transfer.Record, 5));
};


/**
 * @param {!Array<!proto.transfer.Record>} value
 * @return {!proto.signaling.Signaling} returns this
*/
proto.signaling.Signaling.prototype.setRecordsList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 5, value);
};


/**
 * @param {!proto.transfer.Record=} opt_value
 * @param {number=} opt_index
 * @return {!proto.transfer.Record}
 */
proto.signaling.Signaling.prototype.addRecords = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 5, opt_value, proto.transfer.Record, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.signaling.Signaling} returns this
 */
proto.signaling.Signaling.prototype.clearRecordsList = function() {
  return this.setRecordsList([]);
};


