// Note: run `go doc -all` in this package to see all of the types and functions available.
// ./pdk.gen.go contains the domain types from the host where your plugin will run.
package main

import (
	"errors"
	"fmt"
	"math"
	"math/rand/v2"
	"unsafe"

	pdk "github.com/extism/go-pdk"
)

func BatchComputeWf() (int32, error) {
	// BatchComputeWF processes the input buffer and outputs the user-defined wave
	input := pdk.Input()
	// _ = input
	if len(input)%4 != 0 { // || len(input)/4 != 7 {
		return -1, errors.New(fmt.Sprintf("Improper Input Size, %d", len(input)))
	}

	var params []float32 = unsafe.Slice((*float32)(unsafe.Pointer(&input[0])), len(input)/4)

	var sampleTime float32 = params[0]
	var freqHz float32 = params[1]
	var currPhase float32 = params[2]

	var numSamples uint32 = uint32(*(*int32)(unsafe.Pointer(&input[3*4])))

	var inputOne float32 = params[4]
	var inputTwo float32 = params[5]
	var inputThree float32 = params[6]
	var inputFour float32 = params[7]

	var isLeftChannel bool = *(*int32)(unsafe.Pointer(&input[8*4])) > 0

	sample := *(*int64)(unsafe.Pointer(&input[9*4]))
	sampleRate := params[11]

	var args WaveArgs = WaveArgs{
		FreqHz:        freqHz,
		Inputs:        []float32{inputOne, inputTwo, inputThree, inputFour},
		Phase:         currPhase,
		IsLeftChannel: isLeftChannel,
		Sample:        sample,
		SampleRate:    sampleRate,
	}

	outBuf := make([]float32, numSamples)

	var noise_vals []float32 = make([]float32, 6)

	// pdk.Log(pdk.LogError, "goop")
	for sampleNo := 0; sampleNo < len(outBuf); sampleNo++ {
		args.Phase = float32(math.Mod(float64(currPhase+(args.FreqHz*sampleTime*float32(sampleNo))), 1.0))
		args.Sample = args.Sample + 1
		outBuf[sampleNo] = 0.0
		for noise_index := 0; noise_index < len(noise_vals); noise_index++ {
			if (sampleNo%noise_index + 1) == 0 {
				noise_vals[sampleNo] = rand.Float32() // generate new noise sample
			}
			outBuf[sampleNo] += noise_vals[noise_index]
		}
		outBuf[sampleNo] /= float32(len(noise_vals))
	}

	pdk.Output(unsafe.Slice((*byte)(unsafe.Pointer(&outBuf[0])), len(outBuf)*4))

	return 0, nil
}
