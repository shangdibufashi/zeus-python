

export PYTHON_ARCH=arm64
export MACOS_MIN_VERSION=12.0

rm -rf release
unzip -q zeus-macos12m1_release.zip
cp -rf release/* src/zeus/
ls -alh src/zeus/
cd src
./scripts/bdist_single.sh

