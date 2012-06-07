CPP_FILE_LIST=`find -name '*.cc' -print`
xgettext -d logsim -s --keyword=_ --keyword=s_ -p ./po -o logsim.pot $CPP_FILE_LIST