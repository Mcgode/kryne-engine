
# Install git submodules
git submodule init
git submodule update


# === EASTL ===
EASTL_VERSION="3.17.06"

# EASTL version setup
pushd external-libs/eastl || exit
git checkout $EASTL_VERSION

# Retrieve EASTL submodules
git submodule init
git submodule update

popd || exit


# === Boost ===
BOOST_VERSION="boost-1.77.0"

# Boost version setup
pushd external-libs/boost || exit
git checkout $BOOST_VERSION

# Retrieve Boost submodules
git submodule init
git submodule update

popd || exit


# === Boost ===
GLFW_VERSION="3.3.4"

# GLFW version setup
pushd external-libs/glfw || exit
git checkout $GLFW_VERSION

# Retrieve GLFW submodules
git submodule init
git submodule update

popd || exit