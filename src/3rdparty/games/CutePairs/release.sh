#!/bin/sh
. ~/bin/s60env
. ./project.sh
. ../keys.sh
make clean
qmake



sed -i s/"createpackage.bat"/"createpackage.sh"/g Makefile
sed -i s/"createpackage.sh"/"createpackage.sh -u"/g Makefile

make debug-gcce #release-gcce

make sis QT_SIS_CERTIFICATE=$CERT QT_SIS_KEY=$KEY

sed -i s/"#{\"$APPNAME installer\"},(".*/"#{\"$APPNAME installer\"},(0x2002CCCF),$APPVERSION"/g $APPINSTpkg
sed -i s/"\"c:"/"\"\!:"/g $APPINSTpkg

echo "**** installer file fixed ****"

make installer_sis QT_SIS_CERTIFICATE=$CERT QT_SIS_KEY=$KEY
