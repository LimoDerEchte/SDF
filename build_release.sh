
# Build
cmake -DCMAKE_BUILD_TYPE=Game__Shipping__Win64 -G Ninja -S . -B ./cmake-build-release
cmake --build ./cmake-build-release --target SDF

# Create Files
mkdir SDF
mkdir SDF/dlls
mkdir SDF/license
touch SDF/enabled.txt

# Move Files
cp ./cmake-build-release/SDF.dll SDF/dlls/main.dll
cp LICENSE SDF/license/LICENSE
cp libs/tomlplusplus/LICENSE SDF/license/LICENSE_tomlplusplus

# Finalize Package
rm SDF.zip
zip -r SDF.zip SDF/
rm -rf SDF/
