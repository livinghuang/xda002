// Decode decodes an array of bytes into an object.
//  - fPort contains the LoRaWAN fPort number
//  - bytes is an array of bytes, e.g. [225, 230, 255, 0]
//  - variables contains the device variables e.g. {"calibration": "3.5"} (both the key / value are of type string)
// The function must return an object, e.g. {"temperature": 22.5}
function toHexString(bytes) {
    return bytes.map(function(byte) {
        return ("00" + (byte & 0xFF).toString(16)).slice(-2)
      }).join('')
}

function copy_8bytes(bytes, start) {
  var copy = [0, 0, 0, 0, 0, 0, 0, 0];
  for (var i = 0; i < 8; i++) {
    copy[i] = bytes[start+i];
  }
  return copy;
}
function copy_4bytes(bytes, start) {
  var copy = [ 0, 0, 0, 0];
  for (var i = 0; i < 4; i++) {
    copy[i] = bytes[start+i];
  }
  return copy;
}

function copy_2bytes(bytes, start) {
  var copy = [ 0, 0];
  for (var i = 0; i < 2; i++) {
    copy[i] = bytes[start+i];
  }
  return copy;
}
function decodeInt32FromBytes(bytes) {

  var value = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;

    // Convert to a signed int32 if necessary
  // If the highest bit (bit 31) is set, the number is negative
  // if (value & 0x80000000) {
  //     value -= 0x100000000;
  // }

    return value;
}


function decodeInt64FromBytes(bytes) {

  var value = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24 | bytes[4] << 32 | bytes[5] << 40 | bytes[6] << 48 | bytes[7] << 56;

    // Convert to a signed int64 if necessary
    // If the highest bit (bit 63) is set, the number is negative
    if (value & 0x8000000000000000) {
        value -= 0x10000000000000000;
    }

    return value;
}

function convertAsFloat(value, point)
{

  for (var i = 0; i < point; i++)
  {
    value = value / 10;  
  }

    return value;
}

function convertAsInt16(bytes)
{
    return bytes[0] | bytes[1] << 8;
}


function Decode(fPort, bytes,variables) {
    var decoded = {};
    decoded.Payload = toHexString(bytes)
    decoded.LoraBattery = (bytes[0] / 254) * 1.2 + 3.0;
    decoded.MeterBattery = (bytes[1] / 254) * 1.2 + 3.0;
  
  // decoded.totalAccumulationValue = toHexString(copy_8bytes(bytes, 2));
  decoded.totalAccumulation=convertAsFloat(decodeInt64FromBytes(copy_8bytes(bytes, 2)),bytes[10]);
  decoded.totalAccumulationPoint = bytes[10];
  
  // decoded.instantaneousFlowValue = toHexString(copy_4bytes(bytes, 11));
  decoded.instantaneousFlow=convertAsFloat(decodeInt32FromBytes(copy_8bytes(bytes, 11)),bytes[15]);
  decoded.instantaneousFlowPoint = bytes[15];

  // decoded.forwardAccumulationValue = toHexString(copy_8bytes(bytes, 16));
  decoded.forwardAccumulation=convertAsFloat(decodeInt64FromBytes(copy_8bytes(bytes, 16)),bytes[24]);
  decoded.forwardAccumulationPoint = bytes[24];

  // decoded.reverseAccumulationValue = toHexString(copy_8bytes(bytes, 25));
  decoded.reverseAccumulation=convertAsFloat(decodeInt64FromBytes(copy_8bytes(bytes, 25)),bytes[33]);
  decoded.reverseAccumulationPoint = bytes[33];
  
  decoded.leakDays = convertAsInt16(copy_2bytes(bytes, 34)); // toHexString(copy_2bytes(bytes, 34));
  decoded.loadDays = convertAsInt16(copy_2bytes(bytes, 36)); //  toHexString(copy_2bytes(bytes, 36));
  decoded.noFlowDays = convertAsInt16(copy_2bytes(bytes, 38)); // toHexString(copy_2bytes(bytes, 38));
  decoded.reverseDays = convertAsInt16(copy_2bytes(bytes, 40)); // toHexString(copy_2bytes(bytes, 40));
  decoded.magneticInfluenceDays = convertAsInt16(copy_2bytes(bytes, 42)); // toHexString(copy_2bytes(bytes, 42));
  decoded.powerInsufficiencyDays = convertAsInt16(copy_2bytes(bytes, 44)); // toHexString(copy_2bytes(bytes, 44));
  return decoded;
}