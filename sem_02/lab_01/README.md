## To start daemon:

`sudo ./a.out`

## List all daemons:

`ps -ef | awk '$3 == 1'`

## Get created daemon pid:

`cat /var/run/daemon.pid`

## Get daemon logs (Manjaro Linux):

`journalctl -e _PID=pid`

## To kill daemon:

`sudo kill -9 <pid>`

## Выводим:

`ps -ajx | head -1 && ps -ajx | grep pid`

## To send SIGHUP to daemon:

`sudo kill -S HUP <pid>`
