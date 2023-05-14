```bash
sudo apt install tmux python3-pip
sudo python3 -m pip install tmuxp docker-compose jetson-stats
crontab -e
# @reboot /usr/local/bin/tmuxp load /home/user/workspace/base_ws/src/robot/env/autostart.tmuxp.yaml
```
