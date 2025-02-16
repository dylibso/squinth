# Assumes you have your RACK_USER_DIR env set to wherever your ".Rack" folder is

(make clean && make install);

# Install to dev location
mkdir -p ${RACK_USER_DIR}/plugins;
cp -r ${RACK_USER_DIR}/plugins-lin-x64/*sQuinth* ${RACK_USER_DIR}/plugins;
