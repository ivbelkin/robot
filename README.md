```bash
sudo apt install tmux python3-pip
sudo python3 -m pip install tmuxp docker-compose jetson-stats
crontab -e
# @reboot /usr/local/bin/tmuxp load /home/user/workspace/base_ws/src/robot/env/autostart.tmuxp.yaml

docker-compose -f docker-compose-flash.yaml run --rm arduino-flash
docker-compose -f setup/docker-compose-build.yaml up --build camera
```
