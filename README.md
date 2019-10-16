# Trantor OCR
Second year student's project (SPE).

```
Student on this project:
    * hugo.moreau (Moreau Hugo)
    * maxime.chardon (CHARDON Maxime)
    * anthony1.truong (TRUONG Anthony)
```

## NeuralNetwork:
    * contain file for build a neural network.
    * contain a xor file too.
    * We can save and load training from neural network.
        - Usage : make xor
               "> ./xor" for training
               "> ./xor n" for n training
               make ocr
               "> ./ocr" for train 1000 times neural network
               "> ./ocr path_to_image" to compute network with image

## Segmentation:
    * contain file for cut an image in bloc, line and character.
    * coutain file for applying grayscale and binarization.
        - Usage : make grayscale
                "> ./grayscale path_to_image" to show the process
                make segmentation
                "> ./segmentation path_to_image" to show the process

## How run the OCR:
```
$ make
$ ./main

    * Choose an image (there is one on the project folder)
    * Proceed load image if you want to see it
    * You can train the ocr if you want
    * End finally run the OCR
```
