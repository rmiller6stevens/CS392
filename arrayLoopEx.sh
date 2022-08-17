array=(1 2 "three" 4 "five")

#Iterating over Indices
for i in ${!array[@]}; do
echo "[${i}]: ${array[$i]}"
done
# Iterating over elements
for elem in ${array[@]}; do
echo ${elem}
done

# Iterating for a specific number
for i in {0..10}; do
echo "[${i}]: ${array[$i]}"
done