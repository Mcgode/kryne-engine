
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