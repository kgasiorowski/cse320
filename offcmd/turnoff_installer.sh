#!/bin/bash

echo "Copying files..."
cp turnoff.sh off
echo "Installing files..."
sudo mv off /usr/local/bin/
echo "Verifying permissions..."
sudo chmod u+x /usr/local/bin/off
echo "Done!"
echo "Type \"off\" to turn off this vm now. Enjoy!"