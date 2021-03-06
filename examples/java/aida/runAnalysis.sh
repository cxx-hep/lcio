#!/bin/sh -f
########################################################################
#
# Script to run the LCIO/AIDA example using JAIDA
#
########################################################################

CLASSPATH="${CLASSPATH}:${LCIO}/lib/lcio.jar:${LCIO}/tools/sio.jar"

JARS="aida-dev aida bcel freehep-base freehep-hep jas-plotter jel openide-lookup optimizers"

for jar in ${JARS}; do
  CLASSPATH="${CLASSPATH}:${AIDA}/lib/${jar}.jar"
done

exec java -classpath ${CLASSPATH} LCIOAnalysis $*