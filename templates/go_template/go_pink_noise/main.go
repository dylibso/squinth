// Note: run `go doc -all` in this package to see all of the types and functions available.
// ./pdk.gen.go contains the domain types from the host where your plugin will run.
package main

import (
	"math"
	"math/rand/v2"
	"unsafe"

	pdk "github.com/extism/go-pdk"
)

func LogBase(value, base float64) float64 {
	return math.Log(value) / math.Log(base)
}

func MaxInt(numOne int32, numTwo int32) int32 {
	if numOne > numTwo {
		return numOne
	}
	return numTwo
}

func BatchComputeWf() (int32, error) {
	// BatchComputeWF processes the input buffer and outputs the user-defined wave
	input := pdk.Input()
	// _ = input
	// if len(input)%4 != 0 { // || len(input)/4 != 7 {
	// 	return -1, errors.New(fmt.Sprintf("Improper Input Size, %d", len(input)))
	// }

	var params []float32 = unsafe.Slice((*float32)(unsafe.Pointer(&input[0])), len(input)/4)

	// var sampleTime float32 = params[0]
	// var freqHz float32 = params[1]
	// var currPhase float32 = params[2]

	var numSamples uint32 = uint32(*(*int32)(unsafe.Pointer(&input[3*4])))

	var inputOne float32 = params[4]
	// var inputTwo float32 = params[5]
	// var inputThree float32 = params[6]
	// var inputFour float32 = params[7]

	// var isLeftChannel bool = *(*int32)(unsafe.Pointer(&input[8*4])) > 0

	// sample := *(*int64)(unsafe.Pointer(&input[9*4]))
	// sampleRate := params[11]

	// var args WaveArgs = WaveArgs{
	// 	FreqHz:        freqHz,
	// 	Inputs:        []float32{inputOne, inputTwo, inputThree, inputFour},
	// 	Phase:         currPhase,
	// 	IsLeftChannel: isLeftChannel,
	// 	Sample:        sample,
	// 	SampleRate:    sampleRate,
	// }

	outBuf := make([]float32, numSamples)

	num_noise_vals := MaxInt(int32(inputOne)+10, 1) // ensure >= 1
	noise_vals := make([]float32, num_noise_vals)
	// determine the exponent coefficient based on the number of voices
	exp_coef := 1.0 / LogBase(math.Pow(2.0, float64(num_noise_vals)), 512)

	// pdk.Log(pdk.LogError, "goop")
	for sampleNo := 0; sampleNo < len(outBuf); sampleNo++ {
		outBuf[sampleNo] = 0.0
		for noise_index := 0; noise_index < len(noise_vals); noise_index++ {
			if (sampleNo % int(math.Exp2(exp_coef*float64(noise_index)))) == 0 {
				noise_vals[noise_index] = (rand.Float32() * 20.0) - 10.0 // generate new noise sample
			}
			outBuf[sampleNo] += noise_vals[noise_index]
		}
		outBuf[sampleNo] /= float32(len(noise_vals) + 1)
	}

	pdk.Output(unsafe.Slice((*byte)(unsafe.Pointer(&outBuf[0])), len(outBuf)*4))

	return 0, nil
}
