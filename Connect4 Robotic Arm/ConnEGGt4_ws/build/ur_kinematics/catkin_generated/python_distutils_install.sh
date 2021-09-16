#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/mtrn4230/major-project-armee/ConnEGGt4_ws/src/ur_kinematics"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/mtrn4230/major-project-armee/ConnEGGt4_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/mtrn4230/major-project-armee/ConnEGGt4_ws/install/lib/python2.7/dist-packages:/home/mtrn4230/major-project-armee/ConnEGGt4_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/mtrn4230/major-project-armee/ConnEGGt4_ws/build" \
    "/usr/bin/python2" \
    "/home/mtrn4230/major-project-armee/ConnEGGt4_ws/src/ur_kinematics/setup.py" \
     \
    build --build-base "/home/mtrn4230/major-project-armee/ConnEGGt4_ws/build/ur_kinematics" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/mtrn4230/major-project-armee/ConnEGGt4_ws/install" --install-scripts="/home/mtrn4230/major-project-armee/ConnEGGt4_ws/install/bin"
