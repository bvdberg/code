#pc
#SERVER=192.168.178.18
# macbook
#SERVER=192.168.178.220
#SERVER=192.168.178.190
SERVER=192.168.178.18
#SERVER=127.0.0.1
mate-terminal --geometry 86x14+50+50 --command "./client ${SERVER}" --title "Client1"
mate-terminal --geometry 86x14+800+50 --command "./client ${SERVER}" --title "Client2"
mate-terminal --geometry 86x14+1550+50 --command "./client ${SERVER}" --title "Client3"
mate-terminal --geometry 86x14+50+350 --command "./client ${SERVER}" --title "Client4"
mate-terminal --geometry 86x14+800+350 --command "./client ${SERVER}" --title "Client5"
mate-terminal --geometry 86x14+1550+350 --command "./client ${SERVER}" --title "Client6"
mate-terminal --geometry 86x14+50+650 --command "./client ${SERVER}" --title "Client7"
mate-terminal --geometry 86x14+800+650 --command "./client ${SERVER}" --title "Client8"
mate-terminal --geometry 86x14+1550+650 --command "./client ${SERVER}" --title "Client9"
mate-terminal --geometry 86x14+50+950 --command "./client ${SERVER}" --title "Client10"

#./server

