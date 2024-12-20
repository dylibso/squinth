import * as main from "./main";

import { WaveArgs } from "./pdk";

export function wave(): number {
  const untypedInput = JSON.parse(Host.inputString());
  const input = WaveArgs.fromJson(untypedInput);

  const output = main.waveImpl(input);

  Host.outputBytes(output);

  return 0;
}
