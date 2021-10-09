@echo off

Rem Install git submodules
git submodule init
git submodule update

Rem === EASTL ===
SET EASTL_VERSION=3.17.06

Rem EASTL version setup
pushd external-libs\eastl || exit
git checkout %EASTL_VERSION%

Rem Retrieve EASTL submodules
git submodule init
git submodule update

popd || exit

Rem === Boost ===
SET BOOST_VERSION=boost-1.77.0

pushd external-libs\boost || exit
git checkout %BOOST_VERSION%

git submodule init
git submodule update

popd || exit

Rem === GLFW ===
SET GLFW_VERSION=3.3.4

pushd external-libs\glfw || exit
git checkout %GLFW_VERSION%

git submodule init
git submodule update

popd || exit