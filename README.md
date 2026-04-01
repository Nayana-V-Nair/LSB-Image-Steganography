# LSB-Image-Steganography
The goal of this project was to hide a secret text file inside a BMP image. The program stores the size of the text and then hides the text data in the least significant bits of the image bytes. During decoding, the size is read first and then the hidden text is extracted bit by bit. The final result is the original secret text recovered from the image.
