#!/bin/bash

# Directory containing the .xcf files
input_dir="."
output_dir="."

# Create the output directory if it doesn't exist
mkdir -p "$output_dir"

# Loop through all the .xcf files in the input directory
for xcf_file in "$input_dir"/*.xcf; do
  # Get the base name of the file (without extension)
  base_name=$(basename "$xcf_file" .xcf)

  # Define the output .png file path
  png_file="$output_dir/$base_name.png"

  # Use GIMP in batch mode to convert .xcf to .png
  gimp -i -b "(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE \"$xcf_file\" \"$xcf_file\"))) \
                    (drawable (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE)))) \
                    (file-png-save RUN-NONINTERACTIVE image drawable \"$png_file\" \"$png_file\" 0 9 0 0 0 0 0))" \
        -b "(gimp-quit 0)"

  echo "Converted: $xcf_file to $png_file"
done

echo "All .xcf files have been converted to .png and saved in the '$output_dir' directory."
