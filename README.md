# Optical Characters Recognition

EPITA Project
Optical character recognition software written in C.

## Neural Network in /NeuralNetwork
- contain file for build a neural network.
- contain a xor file too.
- We can save and load training from neural network.
- Usage:
```
$ make xor
$ ./xor #training
$ ./xor n #for n training

$ make ocr
$ ./ocr #train by default 1000 iterations
$ ./ocr <path_to_char> #compute the neural network on this char image
```

## Segmentation in /segmentation
- contain file for cut an image in bloc, line and character.
- coutain file for applying grayscale and binarization.
- Usage
```
$ make grayscale
$ ./grayscale <path_to_image>

$ make segmentation
$ ./segmentation <path_to_image>
```

## OCR
The program

```
$ make
$ ./main
```

- step1: choose an image
- step2: load the image (optionnal)
- step3: train the ocr for better performance
- step4: run the OCR
