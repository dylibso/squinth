import * as main from "./main";

import { WaveArgs } from "./pdk";

export function wave(): number {
  const untypedInput = JSON.parse(Host.inputString());
  const input = WaveArgs.fromJson(untypedInput);

  const output = main.waveImpl(input);

  Host.outputBytes(Float32Array.of(output).buffer);

  return 0;
}

export function batch_compute_wf(): number {
  var input_buffer = new Float32Array(Host.inputBytes());
  var int_input_buffer = new Int32Array(Host.inputBytes());

  var sampleTime = input_buffer[0];
  var freqHz = input_buffer[1];
  var currPhase = input_buffer[2];

  var numSamples = int_input_buffer[3];

  var inputOne = input_buffer[4];
  var inputTwo = input_buffer[5];
  var inputThree = input_buffer[6];
  var inputFour = input_buffer[7];

  var isLeftChannel = int_input_buffer[3] > 0;

  var sample = 0;
  var sampleRate = input_buffer[11];

  var floatbuf = new Float32Array(numSamples);

  var args: WaveArgs = {
    freq_hz: freqHz,
    inputs: [inputOne, inputTwo, inputThree, inputFour], // Example input array
    phase: currPhase,
    isLeftChannel: isLeftChannel,
    sample,
    sampleRate,
  };

  for (let sampleNo = 0; sampleNo < floatbuf.length; sampleNo++) {
    args.phase = (currPhase + args.freq_hz * sampleTime * sampleNo) % 1;
    args.sample = args.sample + 1;
    floatbuf[sampleNo] = main.waveImpl(args);
  }

  Host.outputBytes(floatbuf.buffer);

  return 0;
}
