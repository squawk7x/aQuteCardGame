#!/bin/bash

INPUT_FILE="$1"
OUTPUT_DIR="./output_images"
mkdir -p "$OUTPUT_DIR"

declare -A RESOLUTIONS=(
    [mdpi]=48
    [hdpi]=72
    [xhdpi]=96
    [xxhdpi]=144
    [xxxhdpi]=192
)

for RES in "${!RESOLUTIONS[@]}"; do
    SIZE=${RESOLUTIONS[$RES]}
    gimp -i -b "(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE \"$INPUT_FILE\" \"$INPUT_FILE\"))) \
                      (drawable (car (gimp-image-get-active-layer image)))) \
                 (gimp-image-scale image $SIZE $SIZE) \
                 (gimp-file-save RUN-NONINTERACTIVE image drawable \"$OUTPUT_DIR/${INPUT_FILE%.xcf}_${RES}.png\" \"$OUTPUT_DIR/${INPUT_FILE%.xcf}_${RES}.png\") \
                 (gimp-image-delete image))" \
         -b "(gimp-quit 0)"
done
