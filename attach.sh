#!/bin/bash

##
#
# Nekohook Injector
#
##

# add a possible game in the name array, order them in the order of likely ness one should exist over the other, ie: we dont want java first as its common to run
process_names=("csgo_linux64" "hl2_linux")

topstring="__--Nekohook Injector--__"

# Option handler
backtrace=0
repl=0
detach=0
instance=0
pid=0
no_scramble=0
filename="$(realpath libnekohook.so)"
while getopts :hbdi:p:sf:c arg; do
  case $arg in
    \?)
      echo "$(basename "$0"): unrecognized option '$OPTARG'"
      echo "Use \"$(basename "$0") -h\" for a complete list of options."
      exit
      ;;
    h) # help
      echo $topstring
      echo "  Usage:"
      echo "  -h    Gives help info"
      echo "  -b    Injects and backtraces"
      echo "  -g    Let GDB enter REPL"
      echo "  -d    Detaches"
      echo "  -i    Attaches to a specific instance"
      echo "  -p    Attaches to a specific pid(overrides -i)"
      echo "  -s    Prevent name obfuscation and steam vac prevention(needed for detach)"
      echo "  -f    Inject a different file"
      echo "  -c    Spawn a Catbot"
      exit
      ;;
    b)
      backtrace=1
      ;;
    g)
      repl=1
    d)
      detach=1
      ;;
    i)
      instance=$OPTARG
      ;;
    p)
      pid=$OPTARG
      ;;
    s)
      no_scramble=1
      echo "WARNING - VAC PREVENTION DISABLED"
      ;;
    f)
      filename="$(realpath $OPTARG)"
      ;;
    c)
      echo "WMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
      echo "MMMMMMMMMMKOdKXNMMMMMMMMMMMMMMMMMMMMMMMM"
      echo "MMMMMMMWo';lxXMMMMMMMMMMMMMMMMMMMXkolKMM"
      echo "MMMMMMMk ;NMMMW;',;lxKMMMMMMMMWk,':o;kMM"
      echo "MMMMMMMMx'.:dKW.oddl:''l;'...,'.,xdd'XMM"
      echo "MMMMMMMMMMXo'.,';ooo;'.......;cdc,;l'MMM"
      echo "MMMMMMMMMMMMX..,.;;...,;,..'OK0:;,..kMMM"
      echo "MMMMMMMMMMMMX..... ........c0KKo.'..dMMM"
      echo "MMMMMMMMMWX:.....,...,...,.oKKXX'. .cMMM"
      echo "MMMMMMMN,... ....'..';   ,;0XNWMNd,,:MMM"
      echo "MMMMMMMo..    .. ....,;:cxXKKKKXNNNNWMMW"
      echo "MMMMMMMl.. .o;'.....,dO00OOO00kOO0XNNNNW"
      echo "MMMMWMMk...dMM0'.  ..':,,;;;;,;0KWWMMMMM"
      echo "MX0Okx:.  .WMX.....,'...... .. ..c0MMMWN"
      echo "N0K0kd;;ccoWM:...cl:loddoc:;c:,...'oxx0k"
      echo "MNKkkO0NWMNKO'   OWOddxkxdoclOd'....':KK"
      echo "MMMNO0XWWWMW.....xMMMWXXKKXNMMd. ....xK0"
      echo "MMMWXXXXNWW0.....kMMMMMMMMMMMMd.....;00K"
      echo "MMMMWXXXKKKO:,';xMMMMMMMMMMMMMW0xxxKMWNN"
      echo "MMMMMMWNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
      exit
      ;;
  esac
done

# Do a check for root
if [[ $EUID != 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

# Here we need to ensure we have a pid in case user didnt provide us with one
if [ $pid == 0 ]; then
  # Here we attempt to get pids of a game
  for i in "${process_names[@]}"; do
    tmp_pid=$(pidof ${i})
    if [ ! -z $tmp_pid ]; then # we need to check if it anything exists before we use it as an array
      tmp_pid=${tmp_pid[$instance]}
      if [ ! -z $tmp_pid ]; then # check if element from array exists
        pid=$tmp_pid
      fi
      break
    fi
  done
fi

# Last check for pid before we try injecting
if [ $pid == 0 ]; then
  echo "Cant find a running game to inject into!"
  if [ $instance != 0 ]; then
    echo "Try checking instance!"
  fi
  exit 1
fi

# If option s isnt set, we can scramble name and attempt to prevent vac
if [ $no_scramble == 0 ]; then
  # pBypass for crash dumps being sent
  # You may also want to consider using -nobreakpad in your launch options.
  sudo rm -rf /tmp/dumps # Remove if it exists
  sudo mkdir /tmp/dumps # Make it as root
  sudo chmod 000 /tmp/dumps # No permissions
  # Steam doesnt die for some reason
  sudo killall -19 steam
  sudo killall -19 steamwebhelper
  # TODO: Change to a more secure method
  new_filename="/tmp/.gl$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 6 | head -n 1)"
  cp "$filename" "$new_filename"
  filename=$new_filename
fi

echo $topstring
echo Using "$filename" with "$(ps -p $pid -o comm=)"

# Adding commands to gdb dynamicly is tricky with bash, but we can do much simpler with this
GDB_COMMANDS="/tmp/gdb-tmp"
GDB_ARGS = "-n -q --command=$GDB_COMMANDS"

# REPL (Batch Mode)
if [ $repl == 0 ]
  echo "No REPL, using GDB's Batch Mode"
  GDB_ARGS = "$GDB_ARGS -batch"
else
  echo "GDB will enter a REPL"
fi

echo "attach $pid">$GDB_COMMANDS
echo "set \$dlopen = (void*(*)(char*, int)) dlopen">>$GDB_COMMANDS

# Attach
if [ $detach == 0 ]; then
  echo "Attaching..."
  echo "call \$dlopen(\"$filename\", 1)">>$GDB_COMMANDS
# Detach
else
  echo "Detaching..."
  # Dont load lib, just get a handle of a current lib
  echo "set \$library = \$dlopen(\"$filename\", 6)">>$GDB_COMMANDS
  #check for errors with dlopen
  echo "set \$error_check = (char *)dlerror()">>$GDB_COMMANDS
  echo "print (char *) \$error_check">>$GDB_COMMANDS
  #detach
  echo "call \$dlclose(\$library)">>$GDB_COMMANDS
  #check for errors with dlclose
  echo "set \$error_check = (char *)dlerror()">>$GDB_COMMANDS
  echo "print (char *) \$error_check">>$GDB_COMMANDS
fi

# Error checking
echo "set \$error_check = (char *)dlerror()">>$GDB_COMMANDS
echo "print (char *) \$error_check">>$GDB_COMMANDS

# Normal attach
if [ $backtrace == 0 ]; then
  echo "detach">>$GDB_COMMANDS
  echo "quit">>$GDB_COMMANDS
# Backtrace
else
  echo "Backtracing!"
  echo "catch exit exit_group">>$GDB_COMMANDS
  echo "continue">>$GDB_COMMANDS
  echo "backtrace">>$GDB_COMMANDS
fi

# Run the Injector and remove tmp file
gdb $GDB_ARGS
rm $GDB_COMMANDS

# Remove for safety
if [ $no_scramble == 0 ]; then
  rm $filename
fi

sleep 1
sudo killall -18 steamwebhelper
sudo killall -18 steam
