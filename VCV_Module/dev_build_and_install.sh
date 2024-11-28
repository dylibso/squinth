# assumes you have set you RACK_USER_DIR environment variable to /home/${USER}/.Rack2
# WARNING: this ^^^ is an environment variable that Rack looks at, so changing it to something else will also effect general use
make install && \
cp -r $RACK_USER_DIR/plugins-lin-x64/BD_cnCt* $RACK_USER_DIR/plugins
