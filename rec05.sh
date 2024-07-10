#Emilio Pena - ebp0053

#!/bin/bash

# Define a function called `curve` that takes two arguments: the curve amount and the array of grades.
curve() {
  # Assign the curve amount to the variable `incr`.
  incr=$1

  # Shift all of the arguments to the left by one position.
  shift

  # Assign the rest of the arguments to the array `arr`.
  arr=("$@")

  # Create a variable called `index` and assign it the value 0.
  index=0

  # Iterate over the elements of the array `arr`.
  for i in "${arr[@]}"; do
    # Curve the grade at the current index.
    let grades[$index]=$i+$incr

    # Increment the index.
    index=$((index + 1))
  done
}

# Check if the correct number of command-line arguments were passed in.
if [ $# -ne 1 ]; then
  echo "Usage: rec05.sh <curve amount>"
  exit 1
fi

# Get the curve amount from the command-line argument.
curve_amount=$1

# Declare an array to store the quiz grades.
grades=()

# Prompt the user to enter their quiz grades.
for i in $(seq 1 5); do
  echo "Enter your grade for quiz $i: "
  read grade

  # Add the grade to the array.
  grades[$i - 1]=$grade
done

# Curve the quiz grades.
curve $curve_amount "${grades[@]}"

# Print out the curved quiz grades.
echo "Curved Grades:"
for i in $(seq 1 5); do
  echo "Quiz $i: ${grades[$i - 1]}"
done