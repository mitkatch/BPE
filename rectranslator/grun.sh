APP_HOME=/opt/mikhail/development/Canpx
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/expat/lib:/usr/local/BerkeleyDB.4.1/lib:/opt/reuters/SOFTWARE/tibrv_7.2/lib:/opt/reuters/SOFTWARE/rfa5.1.1.L1.solaris.rrg/sun4_SunOS_58/Common/Libs:/opt/reuters/SOFTWARE/rfa5.1.1.L1.solaris.rrg/sun4_SunOS_58/Config/Libs:/opt/reuters/SOFTWARE/rfa5.1.1.L1.solaris.rrg/sun4_SunOS_58/Logger/Libs:/opt/reuters/SOFTWARE/rfa5.1.1.L1.solaris.rrg/sun4_SunOS_58/SessionLayer/Libs:/opt/reuters/SOFTWARE/rfa5.1.1.L1.solaris.rrg/sun4_SunOS_58/AnsiPage/Libs:$APP_HOME/SunOS/lib
export LD_LIBRARY_PATH
RFA_MESSAGE_ROOT=/opt/reuters/rfa/sun4_SunOS_58/Logger/MessageFiles
#RFA_MESSAGE_ROOT=/devtree/sun4-56/rfa/sun4_SunOS_58/Logger/MessageFiles
export RFA_MESSAGE_ROOT

#TODAY=`date +%d`
#APP_HOME='/opt/canpx/projects/Canpx/ml2tfeed'
#LOGFILE="logs/out_ml2t.$TODAY"
#(cd $APP_HOME; ./SunOS/ml2tfeed -V 4 -c file://ml2t.txt > $LOGFILE&)
dbx ./SunOS/rectranslator -c file://goc_translate.cfg
