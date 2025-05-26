function isNull(v: any): boolean {
  return v === undefined || v === null;
}

function cast(caster: (v: any) => any, v: any): any {
  if (isNull(v)) return v;
  return caster(v);
}

function castArray(caster: (v: any) => any) {
  return (v?: Array<any>) => {
    if (isNull(v)) return v;
    caster = cast.bind(null, caster); // bind to null-preserving logic in `cast`
    return v!.map(caster);
  };
}

function castMap(caster: (v: any) => any) {
  return (v?: any) => {
    if (isNull(v)) return v;

    caster = cast.bind(null, caster); // bind to null-preserving logic in `cast`
    const newMap: any = {};
    for (const k in v) {
      newMap[k] = caster(v![k]);
    }
    return newMap;
  };
}

function dateToJson(v?: Date): string | undefined | null {
  if (v === undefined || v === null) return v;
  return v.toISOString();
}
function dateFromJson(v?: string): Date | undefined | null {
  if (v === undefined || v === null) return v;
  return new Date(v);
}

function bufferToJson(v?: ArrayBuffer): string | undefined | null {
  if (v === undefined || v === null) return v;
  return Host.arrayBufferToBase64(v);
}
function bufferFromJson(v?: string): ArrayBuffer | undefined | null {
  if (v === undefined || v === null) return v;
  return Host.base64ToArrayBuffer(v);
}

/**
 * Arguments to the user defined wave function
 */
export class WaveArgs {
  /**
   * frequency in hertz
   */
  // @ts-expect-error TS2564
  freq_hz: number;

  /**
   * an array of input numbers
   */
  // @ts-expect-error TS2564
  inputs: Array<number>;

  /**
   * true if the left channel audio should be generated, false if right
   */
  // @ts-expect-error TS2564
  isLeftChannel: boolean;

  /**
   * number between 0..1 representing the current phase
   */
  // @ts-expect-error TS2564
  phase: number;

  /**
   * the raw sample number if you would like to compute the pitch and phase yourself
   */
  // @ts-expect-error TS2564
  sample: number;

  /**
   * the raw sample rate if you would like to compute the pitch and phase yourself
   */
  // @ts-expect-error TS2564
  sampleRate: number;

  static fromJson(obj: any): WaveArgs {
    return {
      ...obj,
    };
  }

  static toJson(obj: WaveArgs): any {
    return {
      ...obj,
    };
  }
}
